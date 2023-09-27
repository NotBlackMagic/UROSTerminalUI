#include "uros.h"

//uROS Status
uint8_t uROSConenctionType = 0x00;      //0: Unconnected; 1: Serial Connection; 2: Ethernet Connection

//uROS Connection variables
rclc_executor_t executor;
rcl_allocator_t allocator;
rclc_support_t support;
rcl_node_t node;

//uROS Subscribers variables
SubscriberTopicTypes subscribedTopicType = 0;
char subscribedTopic[64];
rcl_subscription_t subscriber;

geometry_msgs__msg__Twist twistMsg;

//uROS Publishers variables
rcl_publisher_t publisher;
std_msgs__msg__Int32 int32Msg;

//uROS Topic List variables
uint8_t topicNamesListInitialized = 0x00;
rcl_names_and_types_t topicNamesList;

//Thread related variables
static struct rt_thread uROSThread;
static rt_uint8_t uROSThreadStack[UROS_THREAD_STACK_SIZE];

//Thread input message queue variables
struct rt_messagequeue uROSMessageQueue;
static uint8_t uROSMessagePool[UROS_MESSAGE_POOL_SIZE];

/**
  * @brief  This function handles uROS Subscription callback
  * @param  msgin: Pointer to subscribed message data
  * @return None
  */
void UROSSubscriberCallback(const void *msgin) {
    //Please remember to enable -u_printf_float in compiler settings
//    const geometry_msgs__msg__Twist * twistMsg = (const geometry_msgs__msg__Twist *)msgin;
//    rt_kprintf("linear \n");
//    rt_kprintf("  x: %f \n", twistMsg->linear.x);
//    rt_kprintf("  y: %f \n", twistMsg->linear.y);
//    rt_kprintf("  z: %f \n", twistMsg->linear.z);
//    rt_kprintf("angular \n");
//    rt_kprintf("  x: %f \n", twistMsg->angular.x);
//    rt_kprintf("  y: %f \n", twistMsg->angular.y);
//    rt_kprintf("  z: %f \n", twistMsg->angular.z);

    //Inform GUI of Topics List
    uint32_t id = UROSThread_Subscriber_None;
    switch(subscribedTopicType) {
        case SubscriberTopic_None: {
            return;
        }
        case SubscriberTopic_Twist: {
            id = (0x80 | UROSThread_Subscriber_Twist);
            break;
        }
    }
    InterThreadMessageStruct guiMsg = {.id = id, .data = (uint32_t*)msgin, .length = 0 };
    rt_mq_send(&guiMessageQueue, (void*)&guiMsg, sizeof(InterThreadMessageStruct));
}

/**
  * @brief  This function subscribes to the "topic" of type "type"
  * @param  type: To subscribe message type (ROSIDL_GET_MSG_TYPE_SUPPORT())
  * @param  topic: Name of topic to subscribe
  * @return rmw_ret_t
  */
rmw_ret_t UROSSubscribeToTopic(const rosidl_message_type_support_t* type, const char* topic) {
    //Create new subscriber
    rmw_ret_t error = rclc_subscription_init_default(&subscriber, &node, type, topic);
    if (error != RCL_RET_OK) {
        rt_kprintf("[micro_ros] failed to create subscriber\n");
        return error;
    }

    error = rclc_executor_add_subscription(&executor, &subscriber, &twistMsg, &UROSSubscriberCallback, ON_NEW_DATA);
    if (error != RCL_RET_OK) {
        rt_kprintf("[micro_ros] failed to add subscriber to executor\n");
        return error;
    }

    return error;
}

void UROSThread() {
    rt_err_t status;
    InterThreadMessageStruct msg;

    rmw_ret_t error;
    uROSConenctionType = 0x00;
    rt_pin_write(GPIO_LED_USER_1, PIN_HIGH);

    //**********************************************************************************//
    //Create uROS Connection Code
    /*
#if defined MICRO_ROS_USE_SERIAL
    //Serial setup
    set_microros_transports();
#endif

#if defined MICRO_ROS_USE_UDP
    // UDP setup
     if(argc==2) {
         set_microros_udp_transports(argv[1], 9999);
     }
     else {
         set_microros_udp_transports("192.168.1.100", 9999);
     }
#endif

    allocator = rcl_get_default_allocator();

    //Create init_options
    error = rclc_support_init(&support, 0, NULL, &allocator);
    if (error != RCL_RET_OK) {
        rt_kprintf("[micro_ros] failed to initialize\n");
        uROSConnected = 0x00;
        return;
    }
    uROSConnected = 0x01;

    //Create node
    error = rclc_node_init_default(&node, "uROS_Terminal", "", &support);
    if (error != RCL_RET_OK) {
        rt_kprintf("[micro_ros] failed to create node\n");
        return;
    }
    */
    //**********************************************************************************//

    //**********************************************************************************//
    //Subscriber Code
    /*
    //Create subscriber
    error = rclc_subscription_init_default( &subscriber,
                                            &node,
                                            ROSIDL_GET_MSG_TYPE_SUPPORT(geometry_msgs, msg, Twist),
                                            "cmd_vel");
    if (error != RCL_RET_OK) {
        rt_kprintf("[micro_ros] failed to create subscriber\n");
        return;
    }

    //Create executor
    error = rclc_executor_init(&executor, &support.context, 1, &allocator);
    if (error != RCL_RET_OK) {
        rt_kprintf("[micro_ros] failed to initialize executor\n");
        return;
    }

    error = rclc_executor_add_subscription(&executor, &subscriber, &twistMsg, &TwistSubscriberCallback, ON_NEW_DATA);
    if (error != RCL_RET_OK) {
        rt_kprintf("[micro_ros] failed to add subscriber to executor\n");
        return;
    }
     */
    //**********************************************************************************//

    //**********************************************************************************//
    //Publisher Code
    /*
    //Create publisher
    error = rclc_publisher_init_default(    &publisher,
                                            &node,
                                            ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
                                            "test_topic");
    if (error != RCL_RET_OK) {
        rt_kprintf("[micro_ros] failed to create publisher\n");
        return;
    }
    */
    //**********************************************************************************//

    uint8_t btnPressed = 0x00;
    while(1) {
        if(uROSConenctionType != 0x00) {
            error = rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100));
            if (error != RCL_RET_OK) {
                rt_kprintf("[micro_ros] failed to spin executor\n");
            }
        }

        //Wait for new message in the thread message queue, or timeout to handle serial message ACK timeouts
        status = rt_mq_recv(&uROSMessageQueue, (void*)&msg, sizeof(InterThreadMessageStruct), 1000);
        if(status != -RT_ETIMEOUT) {
            switch(msg.id) {
                //General Control
                case UROSThread_Connect: {
                    //Connect to ROS server (if not already connected)
                    if(uROSConenctionType == 0x00) {
                        //Not Connected, create a connection

                        //Get arguments
                        uint8_t interface = (uint8_t)(msg.data);
                        if(interface == 0x00) {
                            //Disconnect
                            uROSConenctionType = 0x00;
                            rt_pin_write(GPIO_LED_USER_1, PIN_HIGH);
                        }
                        else if(interface == 0x01) {
                            //Connect via Serial interface
                            set_microros_transports();
                            uROSConenctionType = 0x01;
                            rt_pin_write(GPIO_LED_USER_1, PIN_LOW);
                        }
                        else if(interface == 0x02) {
                            //Connect via Ethernet interface
//                            set_microros_udp_transports("192.168.1.100", 9999);
                            uROSConenctionType = 0x02;
                            rt_pin_write(GPIO_LED_USER_1, PIN_LOW);
                        }

                        //Get allocator
                        allocator = rcl_get_default_allocator();

                        //Create init_options
                        error = rclc_support_init(&support, 0, NULL, &allocator);
                        if (error != RCL_RET_OK) {
                            rt_kprintf("[micro_ros] failed to initialize\n");
                            uROSConenctionType = 0x00;
                            rt_pin_write(GPIO_LED_USER_1, PIN_HIGH);
                        }

                        //Create node
                        error = rclc_node_init_default(&node, "uROS_Terminal", "", &support);
                        if (error != RCL_RET_OK) {
                            rt_kprintf("[micro_ros] failed to create node\n");
                            uROSConenctionType = 0x00;
                            rt_pin_write(GPIO_LED_USER_1, PIN_HIGH);
                        }

                        //Create executor
                        error = rclc_executor_init(&executor, &support.context, 1, &allocator);
                        if (error != RCL_RET_OK) {
                            rt_kprintf("[micro_ros] failed to initialize executor\n");
                            return;
                        }
                    }

                    //Inform GUI of uROS Connection Status
                    InterThreadMessageStruct guiMsg = {.id = (0x80 | UROSThread_Connect), .data = (uint32_t*)uROSConenctionType, .length = 0 };
                    rt_mq_send(&guiMessageQueue, (void*)&guiMsg, sizeof(InterThreadMessageStruct));
                    break;
                }
                case UROSThread_List_Nodes: {
                    //Get new node list
//                    rcutils_string_array_t node_names = rcutils_get_zero_initialized_string_array();
//                    rcutils_string_array_t node_namespaces = rcutils_get_zero_initialized_string_array();
//                    rcl_get_node_names(&node, &allocator, &node_names, &node_namespaces);
//                    rcutils_string_array_fini(&node_names);           //Destroy/clear node_names
//                    rcutils_string_array_fini(&node_namespaces);      //Destroy/clear node_namespaces

                    //Inform GUI of Nodes List
//                    InterThreadMessageStruct guiMsg = {.id = (0x80 | UROSThread_List_Nodes), .data = (uint32_t*)&node_names, .length = 0 };
//                    rt_mq_send(&guiMessageQueue, (void*)&guiMsg, sizeof(InterThreadMessageStruct));
                    break;
                }
                case UROSThread_List_Topics: {
                    //Get new node list
                    if(topicNamesListInitialized == 0x01) {
                        rcl_names_and_types_fini(&topicNamesList);   //Destroy/clear topic_names_and_types
                        topicNamesListInitialized = 0x00;
                    }
                    topicNamesList = rcl_get_zero_initialized_names_and_types();
                    error = rcl_get_topic_names_and_types(&node, &allocator, true, &topicNamesList);
                    if (error != RCL_RET_OK) {
                        char str[64];
                        sprintf(str, "[micro_ros] failed to get topic list (Error: %d)\n", error);
                        //RCL_RET_NODE_INVALID              200
                        //RCL_RET_INVALID_ARGUMENT          11
                        //RCL_RET_NODE_INVALID_NAME         201
                        //RCL_RET_NODE_INVALID_NAMESPACE    202
                        //RCL_RET_ERROR                     1
                        //RMW_RET_TIMEOUT                   2
                        //RMW_RET_UNSUPPORTED               3
//                        rt_kprintf("[micro_ros] failed to get topic list\n");
                        rt_kprintf(str);
                        break;
                    }
                    topicNamesListInitialized = 0x01;

                    rt_kprintf("[micro_ros] Topics:\n");
                    uint8_t i;
                    for(i = 0; i < topicNamesList.names.size; i++) {
                        rt_kprintf(topicNamesList.names.data[i]);
                        rt_kprintf(topicNamesList.types->data[i]);
                        rt_kprintf("\n");
                    }

                    //Count publishers and subscriber to topic
//                    rcl_count_publishers();
//                    rcl_count_subscribers();

                    //Inform GUI of Topics List
                    InterThreadMessageStruct guiMsg = {.id = (0x80 | UROSThread_List_Topics), .data = (uint32_t*)&topicNamesList, .length = 0 };
                    rt_mq_send(&guiMessageQueue, (void*)&guiMsg, sizeof(InterThreadMessageStruct));
                    break;
                }
                //Subscriber Control
                case UROSThread_Subscriber_Twist: {
                    //Twist Subscriber control
                    //Check if already existing subscriber, if yes unsubscribe and delete/clear subscriber
                    if(subscribedTopicType != SubscriberTopic_None) {
                        //Already subscribed to a topic, unsubscribe and clear subscriber
                        error = rclc_executor_remove_subscription(&executor, &subscriber);    //Unsubscribe
                        if (error != RCL_RET_OK) {
                            rt_kprintf("[micro_ros] failed to unsubscribe from topic\n");
                            break;
                        }
                        rcl_subscription_fini(&subscriber, &node);                            //Destroy/clear subscriber
                    }

                    //Subscribe to new topic
                    error = UROSSubscribeToTopic(ROSIDL_GET_MSG_TYPE_SUPPORT(geometry_msgs, msg, Twist), (char*)msg.data);
                    if (error != RCL_RET_OK) {
                        break;
                    }

                    //Safe subscribed topic information
                    subscribedTopicType = SubscriberTopic_Twist;
                    strcpy(subscribedTopic, (char*)msg.data);
                    break;
                }
                //Publisher Control
                case UROSThread_Publisher_None: {
                    //Publisher control
//                    //Create publisher
//                    error = rclc_publisher_init_default(    &publisher,
//                                                            &node,
//                                                            ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
//                                                            "test_topic");
//                    if (error != RCL_RET_OK) {
//                        rt_kprintf("[micro_ros] failed to create publisher\n");
//                        return;
//                    }
//
//                    int32Msg.data = 0;
//                    error = rcl_publish(&publisher, &int32Msg, NULL);
//                    if (error != RCL_RET_OK) {
//                        rt_kprintf("[micro_ros] failed to publish topic\n");
//                    }
                    break;
                }
            }
        }

        if(btnPressed == 0x00 && rt_pin_read(GPIO_BUTTON_USER_2) == PIN_LOW) {
            uint8_t connectionType = 0x01;
            InterThreadMessageStruct uROSMsg = {.id = UROSThread_Connect, .data = (uint32_t*)connectionType, .length = 0 };
            rt_mq_send(&uROSMessageQueue, (void*)&uROSMsg, sizeof(InterThreadMessageStruct));
            btnPressed = 0x01;
        }
        else {
            btnPressed = 0x00;
        }

        rt_thread_mdelay(100);
    }

    //Clean up
    rcl_names_and_types_fini(&topicNamesList);   //Destroy/clear topic_names_and_types
    rcl_subscription_fini(&subscriber, &node);          //Destroy/clear subscriber
    rcl_publisher_fini(&publisher, &node);              //Destroy/clear publisher
}

void UROSThreadInit() {
    rt_err_t status;

    //Initialize the uROS message queue
    status = rt_mq_init(&uROSMessageQueue,
                        "uROSMsgQu",
                        &uROSMessagePool[0],
                        sizeof(InterThreadMessageStruct),
                        sizeof(uROSMessagePool),
                        RT_IPC_FLAG_FIFO);

    //Initialize the actual uROS thread
    status = rt_thread_init(&uROSThread,
                            "uROSTh",
                            UROSThread,
                            RT_NULL,
                            &uROSThreadStack[0],
                            sizeof(uROSThreadStack),
                            UROS_THREAD_PRIORITY,
                            10);

    //Startup the uROS thread
    status = rt_thread_startup(&uROSThread);

    return 0;
}
INIT_ENV_EXPORT(UROSThreadInit);
