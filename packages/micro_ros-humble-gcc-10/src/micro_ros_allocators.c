#include "micro_ros_allocators.h"

//Source: https://github.com/micro-ROS/micro_ros_rtthread_component/blob/main/include/microros_allocators.h

//void * microros_allocate(size_t size, void * state);
//void microros_deallocate(void * pointer, void * state);
//void * microros_reallocate(void * pointer, size_t size, void * state);
//void * microros_zero_allocate(size_t number_of_elements, size_t size_of_element, void * state);

void * microros_allocate(size_t size, void * state) {
    return rt_malloc(size);
};

void microros_deallocate(void * pointer, void * state) {
    return rt_free(pointer);
};

void * microros_reallocate(void * pointer, size_t size, void * state) {
    return rt_realloc(pointer, size);
};

void * microros_zero_allocate(size_t number_of_elements, size_t size_of_element, void * state) {
    return rt_calloc(number_of_elements, size_of_element);
};
