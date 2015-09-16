#ifndef PIDFILE_H_
#define PIDFILE_H_

#include <unistd.h> // pid_t
#include <string>

namespace utils
{

enum PidStatus
{
    PID_OK = 0,                // Last operation succeeded

    PID_FREE = 1,              // Was free *AT THE TIME OF THE CHECK*
    PID_ACQUIRED = 2,          // Is acquired by us (this PID)
    PID_OTHER_ACQUIRED = 3,    // The PID is acquired by another process (lock after lock and fork?)
    PID_STALE = 4,             // The PID is stale, was acquired by another process but it is not running any more.

    // For the following codes getLastErrorStr() will give more info
    PID_LOCKED = 1 << 4,       // 16 - The PID file was locked before us
    PID_INVALID = 2 << 4,      // 32 - The PID file contents was invalid
    PID_IO_ERROR = 3 << 4,     // 48 - OS/IO error
    PID_USAGE_ERROR = 4 << 4,  // 64 - Trying to unlock a PID acquired by another process (PID) or similar
    PID_ERROR = 5 << 4,        // 80 - Unexpected/unknown error occurred
};

/**
 * PID file
 */
class PidFile
{
    friend class AutoPidFileLock;
public:
    PidFile(const char* filePath, int mode = 0, int uid = -1, int gid = -1);
    ~PidFile();

    PidFile(const PidFile&) = delete; // Copy constructor
    PidFile(PidFile&& other) = delete; // Move constructor
    PidFile& operator=(const PidFile&) = delete; // Copy assignment operator
    PidFile& operator=(PidFile&&) = delete; // Move assignment operator

    /**
     * Lock the PID file
     */
    PidStatus lock();
    /**
     * Unlock the PID file
     */
    PidStatus unlock();
    /**
     * Returns true if the file is locked
     */
    bool isLocked() const;
    /**
     * Reset the PID file after forking, to leave it for the child to deal with
     */
    void reset();
    /**
     * Acquire the PID, do when fully functional (running).
     *
     * NB: Will unlock if successful!
     */
    PidStatus acquire(bool cleanStale = true) __attribute__((warn_unused_result));
    /**
     * Release the PID, do before stopping the daemon.
     */
    PidStatus release();

    PidStatus check(pid_t* pid = NULL) __attribute__((warn_unused_result));

    PidStatus removeStale() __attribute__((warn_unused_result));

    const std::string& getLastErr() const
    {
        return m_lastErr;
    }

    inline int getGid() const
    {
        return m_gid;
    }

    inline int getMode() const
    {
        return m_mode;
    }

    inline int getUid() const
    {
        return m_uid;
    }

    inline const char* getLockFilePath() const
    {
        return m_lockFilePath;
    }

    inline const char* getPidFilePath() const
    {
        return m_PIDFilePath;
    }

    inline bool isAcquired() const
    {
        return m_acquired;
    }

private:
    PidStatus rlock(); // reentrant
    PidStatus runlock(); // reentrant

    void storeErrStr(const char* msg, int report_errno = -1);

    char* m_PIDFilePath;
    char* m_lockFilePath;
    std::string m_lastErr;
    int m_lockCounter;

    int m_mode;
    int m_uid;
    int m_gid;
    pid_t m_pid;
    int m_fd;
    bool m_acquired;
};

} /* namespace utils */

#endif /* PIDFILE_H_ */
