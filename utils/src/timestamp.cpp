#include "utils/log.h"
#include "utils/timestamp.h"

#include <cerrno>
#include <ostream>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

namespace utils
{

std::string Timestamp::getGmTimeStr(unsigned subsecondPrecision) const
{

    time_t timetTime = seconds;
    struct tm tmTime;
    if (NULL == gmtime_r(&timetTime, &tmTime))
    {
        return "[ERROR in gmtime_r]";
    }
    char buf[30];
    char nsbuf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S.", &tmTime);
    sprintf(nsbuf, "%09u", nanoseconds);
    if (subsecondPrecision > 9)
    {
        subsecondPrecision = 9;
    }
    nsbuf[subsecondPrecision] = '\0';
    return std::string(buf) + nsbuf;
}

Timestamp Timestamp::getNow()
{
    timespec tspec;
    if (0 != clock_gettime(CLOCK_REALTIME, &tspec))
    {
        char errbuf[1024];
        Log::fatalExit("clock_gettime(CLOCK_REALTIME, &tspec): %s", strerror_r(errno, errbuf, sizeof(errbuf)));
    }
    return Timestamp(tspec);
}

std::ostream& operator<<(std::ostream& os, const Timestamp& ts)
{
    char nsBuf[10]; // 999,999,999
    os << ts.seconds;
    if (ts.nanoseconds > 999999999)
    {
        snprintf(nsBuf, sizeof(nsBuf), "%09li", 999999999l);
    }
    else
    {
        snprintf(nsBuf, sizeof(nsBuf), "%09li", long(ts.nanoseconds));
    }

    return os << "." << nsBuf;
}

} // namespace utils
