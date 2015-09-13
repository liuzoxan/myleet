#ifndef UTILS_TIMESTAMP_H_
#define UTILS_TIMESTAMP_H_

#include "utils/sleep.h"

#include <cstdint>
#include <string>
#include <time.h>

namespace utils
{

/**
 * Simplified timestamp, good till 2106-02-07 06:28:15 +0000
 *
 * In theory, we could use 2 bits from the nanoseconds to extend
 * the validity to 2514-05-30 01:53:04 +0000, but... :-)
 */
union Timestamp
{
    // method
    Timestamp()
    {
        value = 0;
    }

    Timestamp(Timestamp& rhs)
    {
        value = rhs.value;
    }

    Timestamp& operator=(const Timestamp& rhs)
    {
        value = rhs.value;
        return *this;
    }

    Timestamp(uint32_t secs, uint32_t nsecs)
    {
        seconds = secs;
        nanoseconds = nsecs;
    }

    Timestamp(const Timestamp& other)
    {
        value = other.value;
    }

    Timestamp(const timespec& tspec)
    {
        seconds = uint32_t(tspec.tv_sec);
        nanoseconds = uint32_t(tspec.tv_nsec);
    }

    void toTimespec(timespec& tspec)
    {
        tspec.tv_sec = seconds;
        tspec.tv_nsec = nanoseconds;
    }

    uint64_t asNanoseconds() const
    {
        return seconds * SECOND_IN_NS + nanoseconds;
    }

    std::string getGmTimeStr(unsigned subsecondPrecision = 9) const;

    static Timestamp getNow();

    // Data
    uint64_t value;
    struct
    {
        uint32_t nanoseconds;
        uint32_t seconds;
    }__attribute__((packed));
}__attribute__((packed));

inline bool operator<(const Timestamp& lhs, const Timestamp& rhs)
{
    return lhs.value < rhs.value;
}

inline bool operator<=(const Timestamp& lhs, const Timestamp& rhs)
{
    return lhs.value <= rhs.value;
}

inline bool operator>(const Timestamp& lhs, const Timestamp& rhs)
{
    return lhs.value > rhs.value;
}

inline bool operator>=(const Timestamp& lhs, const Timestamp& rhs)
{
    return lhs.value >= rhs.value;
}

inline bool operator==(const Timestamp& lhs, const Timestamp& rhs)
{
    return lhs.value == rhs.value;
}

inline bool operator!=(const Timestamp& lhs, const Timestamp& rhs)
{
    return lhs.value != rhs.value;
}

inline Timestamp operator+(const Timestamp& tstamp, int64_t deltaNs)
{
    int64_t v = tstamp.nanoseconds + (tstamp.seconds * SECOND_IN_NS) + deltaNs;
    if (v > 0)
    {
        return Timestamp(static_cast<uint32_t>(v / SECOND_IN_NS), static_cast<uint32_t>(v % SECOND_IN_NS));
    }
    return Timestamp(0, 0);
}

inline Timestamp& operator+=(Timestamp& lhs, const int64_t deltaNs)
{
    int64_t v = lhs.nanoseconds + (lhs.seconds * SECOND_IN_NS) + deltaNs;
    if (v > 0)
    {
        lhs.seconds = static_cast<uint32_t>(v / SECOND_IN_NS);
        lhs.nanoseconds = static_cast<uint32_t>(v % SECOND_IN_NS);
    }
    else
    {
        lhs.value = 0;
    }
    return lhs;
}

inline Timestamp operator-(const Timestamp& tstamp, int64_t deltaNs)
{
    int64_t v = tstamp.nanoseconds + (tstamp.seconds * SECOND_IN_NS) - deltaNs;
    if (v > 0)
    {
        return Timestamp(static_cast<uint32_t>(v / SECOND_IN_NS), static_cast<uint32_t>(v % SECOND_IN_NS));
    }
    return Timestamp(0, 0);
}

inline Timestamp& operator-=(Timestamp& lhs, const int64_t deltaNs)
{
    int64_t v = lhs.nanoseconds + (lhs.seconds * SECOND_IN_NS) - deltaNs;
    if (v > 0)
    {
        lhs.seconds = static_cast<uint32_t>(v / SECOND_IN_NS);
        lhs.nanoseconds = static_cast<uint32_t>(v % SECOND_IN_NS);
    }
    else
    {
        lhs.value = 0;
    }
    return lhs;
}

inline int64_t operator-(const Timestamp& lhs, const Timestamp& rhs)
{
    return (lhs.seconds * SECOND_IN_NS + lhs.nanoseconds) - (rhs.seconds * SECOND_IN_NS + rhs.nanoseconds);
}


std::ostream& operator<<(std::ostream& os, const Timestamp& ts);

} // namespace utils

#endif /* UTILS_TIMESTAMP_H_ */
