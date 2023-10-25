#ifndef INCLUDE_MICRO_ROS_MICRO_ROS_TIME_H_
#define INCLUDE_MICRO_ROS_MICRO_ROS_TIME_H_

#ifdef __cplusplus
extern "C" {
#endif

//Standard library includes
#include <sys/time.h>

//RTOS (RT-Thread) includes
#include <rtthread.h>

int clock_gettime(clockid_t unused, struct timespec *tp);

#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_MICRO_ROS_MICRO_ROS_TIME_H_  */
