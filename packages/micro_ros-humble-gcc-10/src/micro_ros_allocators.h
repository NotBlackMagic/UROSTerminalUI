#ifndef INCLUDE_MICRO_ROS_MICRO_ROS_ALLOCATORS_H_
#define INCLUDE_MICRO_ROS_MICRO_ROS_ALLOCATORS_H_

#ifdef __cplusplus
extern "C" {
#endif

//Standard library includes
#include <stdlib.h>
#include <stdint.h>

//Micro-ROS includes
#include <rcl/rcl.h>

//RTOS (RT-Thread) includes
#include <rtthread.h>

void * microros_allocate(size_t size, void * state);
void microros_deallocate(void * pointer, void * state);
void * microros_reallocate(void * pointer, size_t size, void * state);
void * microros_zero_allocate(size_t number_of_elements, size_t size_of_element, void * state);

static inline void set_microros_allocators() {
    rcl_allocator_t allocator = rcutils_get_zero_initialized_allocator();
    allocator.allocate = microros_allocate;
    allocator.deallocate = microros_deallocate;
    allocator.reallocate = microros_reallocate;
    allocator.zero_allocate =  microros_zero_allocate;
    (void)!rcutils_set_default_allocator(&allocator);
}

#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_MICRO_ROS_MICRO_ROS_ALLOCATORS_H_  */
