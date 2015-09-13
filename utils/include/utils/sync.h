#ifndef UTILS_SYNC_H_
#define UTILS_SYNC_H_

#include "utils/sleep.h"
#include <pthread.h>

namespace utils
{

#define HW_MEMORY_BARRIER __sync_synchronize()
#define SW_MEMORY_BARRIER asm volatile("": : :"memory")
// __sync_synchronize();

class Lock
{
public:
    Lock();

    void lock();
    bool trylock(int64_t timeout);
    void unlock();

    /**
     * This method should be called after fork.
     */
    void reinit();

private:
    pthread_mutex_t m_mutex;
};

class ScopedLocker
{
public:
    ScopedLocker(Lock* lock);
    ~ScopedLocker();

private:
    Lock* m_lock;
};

// Implementations

inline Lock::Lock()
{
    pthread_mutex_init(&m_mutex, NULL);
}

inline void Lock::lock()
{
    pthread_mutex_lock(&m_mutex);
}

inline bool Lock::trylock(int64_t timeout)
{
    int64_t sleepTime = 0;

    while (sleepTime <= timeout)
    {
        if (pthread_mutex_trylock(&m_mutex) == 0)
        {
            return true;
        }

        ns_sleep(SECOND_IN_NS / 100);
        sleepTime += SECOND_IN_NS / 100;
    }

    return false;
}

inline void Lock::unlock()
{
    pthread_mutex_unlock(&m_mutex);
}

inline void Lock::reinit()
{
    pthread_mutex_init(&m_mutex, NULL);
}

inline ScopedLocker::ScopedLocker(Lock* lock)
{
    m_lock = lock;
    m_lock->lock();
}

inline ScopedLocker::~ScopedLocker()
{
    m_lock->unlock();
}

}

#endif
