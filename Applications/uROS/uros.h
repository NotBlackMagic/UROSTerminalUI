#ifndef APPLICATIONS_UROS_UROS_H_
#define APPLICATIONS_UROS_UROS_H_

#ifdef __cplusplus
extern "C" {
#endif

//General C libraries includes
#include <stdio.h>

//uROS includes
#include <micro_ros_rtt.h>

//uROS RCL includes
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

//ROS Messages
#include <geometry_msgs/msg/twist.h>
#include <std_msgs/msg/int32.h>

//RT-Thread include
#include "rtdevice.h"

//Custom application includes
#include "Applications/config.h"
#include "Applications/pinMapping.h"
#include "Applications/interThreadCommunication.h"

//Public uROS objects
extern rcl_names_and_types_t topic_names_and_types;

#ifdef __cplusplus
}
#endif

#endif /* APPLICATIONS_UROS_UROS_H_ */
