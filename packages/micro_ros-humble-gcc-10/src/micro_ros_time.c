#include "micro_ros_time.h"

//int clock_gettime(clockid_t unused, struct timespec *tp);

#define micro_rollover_useconds 4294967295

int clock_gettime(clockid_t unused, struct timespec *tp) {
//    (void)unused;
//
//    rt_tick_t millis = rt_tick_get_millisecond();
//
//    tp->tv_sec = millis / 1000;
//    tp->tv_nsec = (long)(millis % 1000) * 1000;
//
//    return 0;

    (void)unused;
    static uint32_t rollover = 0;
    static uint32_t last_measure = 0;

    uint32_t m = rt_tick_get_millisecond() * 1000;
    rollover += (m < last_measure) ? 1 : 0;

    uint64_t real_us = (uint64_t) (m + rollover * micro_rollover_useconds);
    tp->tv_sec = real_us / 1000000;
    tp->tv_nsec = (real_us % 1000000) * 1000;
    last_measure = m;

    return 0;
}
