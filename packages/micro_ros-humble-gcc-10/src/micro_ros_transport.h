#ifndef INCLUDE_MICRO_ROS_MICRO_ROS_TRANSPORT_H_
#define INCLUDE_MICRO_ROS_MICRO_ROS_TRANSPORT_H_

#ifdef __cplusplus
extern "C" {
#endif

//Standard library includes
#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>

//Micro-ROS includes
#include <uxr/client/transport.h>
#include <rmw_microros/rmw_microros.h>

//RTOS (RT-Thread) includes
#include <rtthread.h>

bool serial_transport_open(struct uxrCustomTransport* transport);
bool serial_transport_close(struct uxrCustomTransport* transport);
size_t serial_transport_write(struct uxrCustomTransport* transport, uint8_t* buf, size_t len, uint8_t* err);
size_t serial_transport_read(struct uxrCustomTransport* transport, uint8_t* buf, size_t len, int timeout, uint8_t* err);

static inline void set_microros_transports() {
    rmw_uros_set_custom_transport(
        true,
        NULL,
        serial_transport_open,
        serial_transport_close,
        serial_transport_write,
        serial_transport_read
    );
}

#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_MICRO_ROS_MICRO_ROS_TRANSPORT_H_ */
