#ifndef APPLICATIONS_INTERTHREADCOMMUNICATION_H_
#define APPLICATIONS_INTERTHREADCOMMUNICATION_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "rtdevice.h"

#define UROS_THREAD_PRIORITY			        10
#define UROS_THREAD_STACK_SIZE		            4096
#define UROS_MESSAGE_POOL_SIZE		            1024

#define GUI_MESSAGE_POOL_SIZE					1024

typedef struct {
    uint32_t id;
    uint32_t *data;
    uint32_t length;
} InterThreadMessageStruct;

extern struct rt_messagequeue guiMessageQueue;
extern struct rt_messagequeue uROSMessageQueue;

typedef enum {
    //General Control
	UROSThread_None = 0x00,
	UROSThread_Connect = 0x01,
	UROSThread_List_Nodes = 0x02,
	UROSThread_List_Topics = 0x03,
	//Subscriber Control
	UROSThread_Subscriber_None = 0x10,
	UROSThread_Subscriber_Twist = 0x11,
	//Publisher Control
	UROSThread_Publisher_None = 0x40
} UROSThreadOpcode;

#ifdef __cplusplus
}
#endif

#endif /* APPLICATIONS_INTERTHREADCOMMUNICATION_H_ */
