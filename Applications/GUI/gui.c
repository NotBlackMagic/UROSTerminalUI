#include "gui.h"

struct rt_messagequeue guiMessageQueue;
static uint8_t guiMessagePool[GUI_MESSAGE_POOL_SIZE];

/**
  * @brief	This function handles the initialization of the user GUI interface, called from the GUI Thread Initialization function
  * @param	None
  * @return	None
  */
void lv_user_gui_init(void) {
	StylesInit();
    GUIInit();
}

/**
  * @brief	This function handles updates to the GUI, called from the GUI Thread function before each LVGL update call (lv_task_handler).
  * @param	None
  * @return	None
  */
void lv_user_gui_update(void) {
	GUIUpdate();
}

/**
  * @brief	This function initializes the GUI interface
  * @param	None
  * @return	None
  */
void GUIInit() {
	//Initialize the GUI message queue
    rt_err_t status = rt_mq_init(   &guiMessageQueue,
                                    "GUIMsgQu",
                                    &guiMessagePool[0],
                                    sizeof(InterThreadMessageStruct),
                                    sizeof(guiMessagePool),
                                    RT_IPC_FLAG_FIFO);

	//Set screen background color
	lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex(0x000000), 0);

	//Initialize each part of the GUI interface
	GUIWidgetKeyboardInit();
	GUITopBarInit();
	GUITopicListWindowInit();
	GUISubscriberWindowInit();
	GUIPublisherWindowInit();
	GUIBottomBarInit();
	GUIInfoWindowInit();
}

/**
  * @brief	This function updates the GUI interface
  * @param	None
  * @return	None
  */
void GUIUpdate() {
	//Update Time
	time_t now = time(RT_NULL);
	struct tm tm = *localtime(&now);
	char str[16];
	sprintf(str, "%02d:%02d:%02d", tm.tm_hour, tm.tm_min, tm.tm_sec);
	lv_label_set_text(timeLabel, str);

    //Check if new message in the thread queue, used to transmit necessary updates to the GUI. None blocking call to keep the GUI responsive!
    InterThreadMessageStruct msg;
//  if(rt_mq_recv(&guiMessageQueue, (void*)&msg, sizeof(InterThreadMessageStruct), RT_WAITING_NO) == RT_EOK) {
    while(rt_mq_recv(&guiMessageQueue, (void*)&msg, sizeof(InterThreadMessageStruct), RT_WAITING_NO) == RT_EOK) {
        if(msg.id & 0x80 != 0x80) {
            //Not a return message so ignore
            continue;
        }
        switch((msg.id & 0x7F)) {
            case UROSThread_Connect: {
                //Got connection status
                uint8_t uROSConnectionStatus = (uint8_t)(msg.data);
                if(uROSConnectionStatus == 0x00) {
                    //Unconnected
                    lv_label_set_text(connectionLabel, "UNCONNECTED");
//                    lv_obj_set_style_text_color(connectionLabel, STYLE_COLOR_GREEN, LV_PART_MAIN);
                }
                else if (uROSConnectionStatus == 0x01) {
                    //Serial connection
                    lv_label_set_text(connectionLabel, "COM 115200 8-N-1");
                }
                else if (uROSConnectionStatus == 0x02) {
                    //Ethernet connection
                    lv_label_set_text(connectionLabel, "ETH 192.168.1.1:8800");
                }
                break;
            }
            case UROSThread_List_Nodes: {
                //Got new list of nodes
                break;
            }
            case UROSThread_List_Topics: {
                //Got new list of topics
                rcl_names_and_types_t* topics = (rcl_names_and_types_t*)(msg.data);
                GUISubscriberWindowUpdate(topics);
                break;
            }
            //Subscriber Messages
            //Geometry Messages
            case UROSThread_Subscriber_Accel: {
                //Got new Accel topic message
                GUISubscriberContentAccel(msg.data);
                break;
            }
            case UROSThread_Subscriber_Point: {
                //Got new Point topic message
                GUISubscriberContentPoint(msg.data);
                break;
            }
            case UROSThread_Subscriber_Pose: {
                //Got new Pose topic message
                GUISubscriberContentPose(msg.data);
                break;
            }
            case UROSThread_Subscriber_Quaternion: {
                //Got new Quaternion topic message
                GUISubscriberContentQuaternion(msg.data);
                break;
            }
            case UROSThread_Subscriber_Transform: {
                //Got new Transform topic message
                GUISubscriberContentTransform(msg.data);
                break;
            }
            case UROSThread_Subscriber_Twist: {
                //Got new Twist topic message
                GUISubscriberContentTwist(msg.data);
                break;
            }
            case UROSThread_Subscriber_Vector3: {
                //Got new Vector3 topic message
                GUISubscriberContentVector3(msg.data);
                break;
            }
            //Navigation Message
            case UROSThread_Subscriber_Odometry: {
                //Got new Odometry topic message
                GUISubscriberContentOdometry(msg.data);
                break;
            }
        }
    }
}
