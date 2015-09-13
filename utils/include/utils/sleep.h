#ifndef UTILS_SLEEP_H_
#define UTILS_SLEEP_H_

#include <stdint.h>

namespace utils
{

static const long SECOND_IN_NS = 1000000000l; // 1 second in nanoseconds

void ns_sleep_to(int64_t nanosecTimestamp);
void ns_sleep(int64_t nanosec);

#ifndef likely
#define likely(x)       __builtin_expect((x),1)
#endif // likely

#ifndef unlikely
#define unlikely(x)     __builtin_expect((x),0)
#endif // unlikely

} // namespace utils

#endif /* UTILS_SLEEP_H_ */
