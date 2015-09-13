/**
 * Copyright (c) 2014,TravelSky.
 * All Rights Reserved.
 * TravelSky CONFIDENTIAL
 *
 * @file       /searchone/utils/src/sleep.cpp
 * @brief      Precise and easy to use sleep implementation
 *
 * @date       04.11.2014
 * @author     dgunchev@gwebu.com
 * @note       Created
 */

#include <time.h>
#include <errno.h>
#include "utils/sleep.h"

namespace utils
{

void ns_sleep_to(int64_t nanosecTimestamp)
{
    struct timespec req;
    req.tv_sec = nanosecTimestamp / SECOND_IN_NS;
    req.tv_nsec = nanosecTimestamp % SECOND_IN_NS;

    // Can not use CLOCK_MONOTONIC_RAW on RHEL 6.x
    while (clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &req, NULL) == -1 && errno == EINTR)
    {
        continue;
    }
}

void ns_sleep(int64_t nanosec)
{
    if (nanosec < 10)
    {
        return;
    }
    struct timespec req;
    // Can not use CLOCK_MONOTONIC_RAW on RHEL 6.x
    clock_gettime(CLOCK_MONOTONIC, &req);

    ns_sleep_to(nanosec + req.tv_sec * SECOND_IN_NS + req.tv_nsec);
}

} // namespace utils
