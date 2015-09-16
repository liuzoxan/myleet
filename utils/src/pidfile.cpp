#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>

#include "utils/log.h"
#include "utils/pidfile.h"

namespace utils
{
// $ kill -0 2841
// bash: kill: (2841) - No such process
// $ kill -0 28417
// bash: kill: (28416) - Operation not permitted

static const char* LOCK_FILE_EXT = ".lock";
static const int ERRNO_BUF_SIZE = 1024;
static const int ERR_BUF_SIZE = 2048;
static const int PID_READ_BUF = sizeof(pid_t) * 3 + 1;

PidFile::PidFile(const char* filePath, int mode, int uid, int gid)
{
    assert(NULL != filePath);
    // TODO: Make m_filePath absolute!
    m_PIDFilePath = strdup(filePath);
    size_t lockFilePathStrlen = strlen(m_PIDFilePath) + strlen(LOCK_FILE_EXT);
    m_lockFilePath = static_cast<char*>(malloc(lockFilePathStrlen + 1));
    memcpy(m_lockFilePath, m_PIDFilePath, strlen(m_PIDFilePath));
    memcpy(m_lockFilePath + strlen(m_PIDFilePath), LOCK_FILE_EXT, strlen(LOCK_FILE_EXT));
    m_lockFilePath[lockFilePathStrlen] = '\0';

    m_lastErr = "";

    m_lockCounter = 0; // Not locked yet

    if (0 == mode)
    {
        m_mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    }
    else
    {
        m_mode = mode | S_IRUSR | S_IWUSR;
    }

    if (uid != -1)
    {
        m_uid = uid;
    }
    else
    {
        m_uid = geteuid();
    }

    if (gid != -1)
    {
        m_gid = gid;
    }
    else
    {
        m_gid = getegid();
    }

    m_pid = 0;

    m_fd = -1;
    m_acquired = false;
}

PidFile::~PidFile()
{
    if (getpid() == m_pid and m_acquired)
    {
        if (PID_OK != release())
        {
            Log::fatal("Can not release the PID file on exit!");
        }
    }
    if (isLocked())
    {
        if (PID_OK != unlock())
        {
            Log::fatal("Can not unlock the PID file on exit!");
        }
    }
    free(m_PIDFilePath);
    free(m_lockFilePath);
}

void PidFile::storeErrStr(const char* msg, int report_errno)
{
    m_lastErr = "";

    char errnobuf[ERRNO_BUF_SIZE];
    char errbuf[ERR_BUF_SIZE];

    if (-1 == report_errno)
    {
        snprintf(errbuf, sizeof(errbuf), "%s, PID file '%s'",
                msg, m_PIDFilePath);
    }
    else
    {
        snprintf(errbuf, sizeof(errbuf), "%s: %s, PID file '%s'",
                msg, strerror_r(report_errno, errnobuf, sizeof(errnobuf)), m_PIDFilePath);
    }
    m_lastErr = strdup(errbuf);
}

PidStatus PidFile::acquire(bool cleanStale)
{
    m_lastErr = "";

    if (0 != m_pid)
    {
        if (m_pid == getpid())
        {
            storeErrStr("Trying to acquire the PID file twice");
        }
        else
        {
            storeErrStr("Trying to acquire the PID file after a child/parent did acquire it");
        }
        return PID_USAGE_ERROR;
    }

    PidStatus lock_state = rlock();
    if (lock_state != PID_OK)
    {
        return lock_state;
    }
    assert(-1 != m_fd);

    PidStatus check_state = check();
    if (PID_OTHER_ACQUIRED == check_state)
    {
        if (PID_OK != runlock())
        {
            Log::fatal("%s", m_lastErr.c_str());
        }
        storeErrStr("PID is already acquired by another process");
        return PID_OTHER_ACQUIRED;
    }

    if (cleanStale and check_state == PID_STALE)
    {
        PidStatus state = removeStale();
        if (state != PID_OK and state != PID_FREE)
        {
            std::string tmp(m_lastErr);
            if (PID_OK != runlock())
            {
                Log::fatal("%s", m_lastErr.c_str());
            }
            storeErrStr(tmp.c_str());
            return state;
        }
        check_state = check();
    }

    if (check_state == PID_STALE)
    {
        if (PID_OK != runlock())
        {
            Log::fatal("%s", m_lastErr.c_str());
        }
        storeErrStr("Can not acquire stale PID without permission to remove the stale file");
        return PID_STALE;
    }

    if (check_state != PID_FREE)
    {
        std::string err = m_lastErr;
        if (PID_OK != runlock())
        {
            Log::fatal("%s", m_lastErr.c_str());
        }
        m_lastErr = err;
        return check_state;
    }

    m_pid = getpid();
    char buf[PID_READ_BUF];
    snprintf(buf, sizeof(buf) - 1, "%i", m_pid);
    strncat(buf, "\n", sizeof(buf) - 1 - strlen(buf));

    if (static_cast<ssize_t>(strlen(buf)) != write(m_fd, buf, strlen(buf)))
    {
        m_pid = 0;
        if (PID_OK != runlock())
        {
            Log::fatal("%s", m_lastErr.c_str());
        }
        storeErrStr("Can not write PID lock file: write()", errno);
        return PID_IO_ERROR;
    }

    if (-1 == link(m_lockFilePath, m_PIDFilePath))
    {
        m_pid = 0;
        int link_error = errno;
        if (PID_OK != runlock())
        {
            Log::fatal("%s", m_lastErr.c_str());
        }
        storeErrStr("Can not link lock to PID file: link()", link_error);
        Log::error("Can not link '%s' to '%s': %s!", m_lockFilePath, m_PIDFilePath, m_lastErr.c_str());
        return PID_IO_ERROR;
    }

    if (PID_OK != unlock())
    {
        Log::fatal("%s", m_lastErr.c_str());
        m_lastErr = "";
    }
    m_acquired = true;
    return PID_OK;
}

PidStatus PidFile::release()
{
    m_lastErr = "";

    if (not m_acquired)
    {
        storeErrStr("Calling release without acquiring the PID first");
        return PID_USAGE_ERROR;
    }

    if (0 != m_pid)
    {
        if (-1 == unlink(m_PIDFilePath))
        {
            storeErrStr("Can not unlink PID file: unlink()", errno);
            return PID_IO_ERROR;
        }
        m_pid = 0;
        m_acquired = false;
        return PID_OK;
    }

    storeErrStr("Calling release child/parent that did not acquire the PID file");
    return PID_USAGE_ERROR;
}

PidStatus PidFile::check(pid_t* pidPtr)
{
    m_lastErr = "";

    if (NULL != pidPtr)
    {
        *pidPtr = 0;
    }

    PidStatus lockstate = rlock();
    if (lockstate != PID_OK)
    {
        return lockstate;
    }

    char buf[PID_READ_BUF];
    ssize_t sz;
    {
        int fd = open(m_PIDFilePath, O_NOATIME | O_NOFOLLOW | O_RDONLY, 0);
        if (-1 == fd)
        {
            if (PID_OK != runlock())
            {
                Log::fatal("%s", m_lastErr.c_str());
                m_lastErr = "";
            }
            if (errno == ENOENT)
            {
                return PID_FREE;
            }
            storeErrStr("Can not open PID file: open()", errno);
            return PID_IO_ERROR;
        }

        sz = read(fd, buf, sizeof(buf) - 1);
        if (-1 == sz)
        {
            int read_errno = errno;
            if (PID_OK != runlock())
            {
                Log::fatal("%s", m_lastErr.c_str());
                m_lastErr = "";
            }
            if (-1 == close(fd))
            {
                Log::fatal("Error closing the PID lock file!");
            }
            storeErrStr("Can not read PID file: read()", read_errno);
            return PID_IO_ERROR;
        }

        if (-1 == close(fd))
        {
            // TODO: Log error message
            assert(false);
        }
    }
    if (PID_OK != runlock())
    {
        Log::fatal("%s", m_lastErr.c_str());
        m_lastErr = "";
    }

    if (sz < 0 or sz > static_cast<ssize_t>(sizeof(buf) - 1)) // Just making sure...
    {
        storeErrStr("Error reading PID file: read()", errno);
        return PID_IO_ERROR;
    }
    buf[sz] = '\0';

    for (ssize_t i = 0; i < sz; ++i)
    {
        // Stop on new line (all flavors) or null character
        if (buf[i] == '\r' or buf[i] == '\n' or buf[i] == '\0')
        {
            buf[i] = '\0';
            sz = i;
            break;
        }
        // Check for normal digits
        if (buf[i] > '9' or buf[i] < '0')
        {
            storeErrStr("Invalid PID file");
            return PID_INVALID;
        }
    }

    if (sz < 1)
    {
        storeErrStr("Invalid/empty PID file");
        return PID_INVALID;
    }

    pid_t fpid = atol(buf);
    if (1 > fpid)
    {
        storeErrStr("Invalid PID file contents");
        return PID_INVALID;
    }

    if (NULL != pidPtr)
    {
        *pidPtr = fpid;
    }
    if (fpid == m_pid)
    {
        // TODO: Check if the PID was created after we started... (could be stale and we could be lucky)
        return PID_ACQUIRED;
    }
    int ret = kill(fpid, 0);
    if (ret == 0) // Same user or root
    {
        // TODO: Check if the other PID is running from before the PID file was created, if not - stale!
        return PID_OTHER_ACQUIRED;
    }

    switch (errno)
    {
    case 0: // No error, we have permission and the other process is running
        // TODO: Check if the other PID is running from before the PID file was created, if not - stale!
        return PID_OTHER_ACQUIRED;
        break;

    case EPERM:
        // If we are running as user and the process that acquired the lock is another user we'll get error...
        // Can we check if the other PID is running from before the PID file was created? No? (or report stale)
        return PID_OTHER_ACQUIRED;
        break;

    case ESRCH:
        // The process with PID from the file has exited but did not clean the PID file.
        return PID_STALE;
        break;
    }
    // TODO: Log the error here, should not happen
    return PID_ERROR;
}

PidStatus PidFile::removeStale()
{
    m_lastErr = "";

    if (getpid() == m_pid)
    {
        storeErrStr("Calling removeStale() while holding the PID acquired");
        return PID_USAGE_ERROR;
    }

    PidStatus lockstate = rlock();
    if (lockstate != PID_OK)
    {
        return lockstate;
    }

    PidStatus check_state = check();
    if (PID_STALE != check_state)
    {
        std::string tmp(m_lastErr);
        if (PID_OK != runlock())
        {
            Log::fatal("%s", m_lastErr.c_str());
        }
        storeErrStr(tmp.c_str());
        return check_state;
    }

    if (0 != unlink(m_PIDFilePath))
    {
        if (PID_OK != runlock())
        {
            Log::fatal("%s", m_lastErr.c_str());
        }
        storeErrStr("Can not remove stale PID file: unlink()", errno);
        return PID_IO_ERROR;
    }

    return runlock();
}

PidStatus PidFile::lock()
{
    m_lastErr = "";

    if (isLocked())
    {
        storeErrStr("Trying to lock locked file");
        return PID_ERROR;
    }

    m_fd = open(m_lockFilePath, O_CREAT | O_EXCL | O_NOATIME | O_NOFOLLOW | O_RDWR, m_mode);
    if (-1 == m_fd)
    {
        if (errno == EEXIST)
        {
            storeErrStr("PID file locked by other process");
            std::string msg = m_lastErr;
            msg = msg + ", lock file: '" + this->m_lockFilePath + "'";
            m_lastErr = "";
            m_lastErr = strdup(msg.c_str());
            return PID_LOCKED;
        }

        storeErrStr("Can not create lock file: open()", errno);
        return PID_IO_ERROR;
    }

    if (geteuid() == 0)
    {
        // Running as root, set both UID and GID
        if (-1 == fchown(m_fd, m_uid, m_gid))
        {
            close(m_fd);
            m_fd = -1;
            unlink(m_lockFilePath);
            storeErrStr("Error chown-ing PID file - fchown()", errno);
            return PID_IO_ERROR;
        }
    }
    else
    {
        // Can't set the UID, set the GID only
        if (-1 == fchown(m_fd, geteuid(), m_gid))
        {
            close(m_fd);
            m_fd = -1;
            unlink(m_lockFilePath);
            storeErrStr("Error chown-ing PID file - fchown()", errno);
            return PID_IO_ERROR;
        }
    }

    m_lockCounter = 1;
    return PID_OK;
}

PidStatus PidFile::unlock()
{
    m_lastErr = "";

    if (not isLocked())
    {
        storeErrStr("Calling unlock without lock");
        return PID_ERROR;
    }

    m_lockCounter = 0;
    if (-1 == close(m_fd))
    {
        Log::fatal("Error closing PID lock file FD!");
    }
    m_fd = -1;

    if (-1 == unlink(m_lockFilePath))
    {
        storeErrStr("Can not remove PID lock file: unlink()", errno);
        Log::fatal("%s", m_lastErr.c_str());
        return PID_IO_ERROR;
    }

    return PID_OK;
}

bool PidFile::isLocked() const
{
    assert((m_lockCounter == 0 and m_fd == -1) or (m_lockCounter != 0 and m_fd != -1));
    return m_lockCounter != 0; // return m_fd != -1;
}

void PidFile::reset()
{
    m_lastErr = "";

    if (m_acquired)
    {
        Log::fatal("Can't reset acquired PID file!");
        return;
    }
    // m_acquired = false;

    m_lockCounter = 0;
    m_pid = 0;
    if (m_fd != -1)
    {
        close(m_fd);
        m_fd = -1;
    }
}

PidStatus PidFile::rlock()
{
    m_lastErr = "";

    if (isLocked())
    {
        ++m_lockCounter;
        return PID_OK;
    }
    return lock();
}

PidStatus PidFile::runlock()
{
    m_lastErr = "";

    if (m_lockCounter > 1)
    {
        --m_lockCounter;
        return PID_OK;
    }
    return unlock();
}

} /* namespace utils */
