#include "guiSubscriberWindow.h"

#define SUBSCRIBER_CONTENT_LINES                11
#define SUBSCRIBER_CONTENT_LINE_CHARS           36
#define SUBSCRIBER_CONTENT_SIZE                 (SUBSCRIBER_CONTENT_LINE_CHARS * SUBSCRIBER_CONTENT_LINES)

//Subscriber Window  GUI objects
lv_obj_t* subscriberWindowArea;
lv_obj_t* subscriberList;
lv_obj_t* subscriberInfoArea;
lv_obj_t* subscriberInfoHeaderLabel;
lv_obj_t* subscriberContentLabel;

lv_obj_t* subscribeTopicPopUpArea;
lv_obj_t* subscribeTopicTypeLabel;
lv_obj_t* subscribeTopicTextArea;
lv_obj_t* subscribeTopicButton;

/**
  * @brief  This function handles text area events of this GUI part
  * @param  lv_event_t: Pointer to the triggered event
  * @return None
  */
static void GUISubscribeTopicTextAreaEvent(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * ta = lv_event_get_target(e);
//    lv_obj_t * keyboard = lv_event_get_user_data(e);
    if(code == LV_EVENT_FOCUSED) {
        lv_obj_align(subscribeTopicPopUpArea, LV_ALIGN_TOP_MID, 0, 3);

        lv_keyboard_set_textarea(keyboard, subscribeTopicTextArea);
        lv_obj_clear_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
        lv_obj_move_foreground(keyboard);
    }

    if(code == LV_EVENT_DEFOCUSED || code == LV_EVENT_READY) {
        lv_obj_align(subscribeTopicPopUpArea, LV_ALIGN_CENTER, 0, 0);

        lv_keyboard_set_textarea(keyboard, NULL);
        lv_obj_add_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
    }
}

/**
  * @brief  This function handles button events of this GUI part
  * @param  lv_event_t: Pointer to the triggered event
  * @return None
  */
static void GUISubscribeTopicPopUpCloseButtonEvent(lv_event_t * e) {
    lv_obj_add_flag(subscribeTopicPopUpArea, LV_OBJ_FLAG_HIDDEN);
}

/**
  * @brief  This function handles button events of this GUI part
  * @param  lv_event_t: Pointer to the triggered event
  * @return None
  */
static void GUISubscribeTopicButtonEvent(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t* btn = lv_event_get_target(e);
    if(code == LV_EVENT_CLICKED) {
        char str[64];
        char* topic = lv_textarea_get_text(subscribeTopicTextArea);
        char* topicType = lv_label_get_text(subscribeTopicTypeLabel);
        sprintf(str, "%s [%s]\n", topic, topicType);
        lv_label_set_text(subscriberInfoHeaderLabel, str);

        //Hide Subscribe to Topic Window
        lv_obj_add_flag(subscribeTopicPopUpArea, LV_OBJ_FLAG_HIDDEN);

        //Convert Subscribe Topic Name to ID number
        int id = UROSThread_Subscriber_None;
        if(strcmp(topicType, "Accel") == 0) {
            id = UROSThread_Subscriber_Accel;
        }
        else if(strcmp(topicType, "Point") == 0) {
            id = UROSThread_Subscriber_Point;
        }
        else if(strcmp(topicType, "Pose") == 0) {
            id = UROSThread_Subscriber_Pose;
        }
        else if(strcmp(topicType, "Quaternion") == 0) {
            id = UROSThread_Subscriber_Quaternion;
        }
        else if(strcmp(topicType, "Transform") == 0) {
            id = UROSThread_Subscriber_Transform;
        }
        else if(strcmp(topicType, "Twist") == 0) {
            id = UROSThread_Subscriber_Twist;

            geometry_msgs__msg__Twist twistMsg;
            twistMsg.linear.x = 2.21f;
            twistMsg.linear.y = 1.68f;
            twistMsg.linear.z = -8.94f;
            twistMsg.angular.x = 1.00f;
            twistMsg.angular.y = -98.1;
            twistMsg.angular.z = 7.11f;
            GUISubscriberContentTwist((void*)&twistMsg);
        }
        else if(strcmp(topicType, "Vector3") == 0) {
            id = UROSThread_Subscriber_Vector3;
        }
        else if(strcmp(topicType, "Odometry") == 0) {
            id = UROSThread_Subscriber_Odometry;

            nav_msgs__msg__Odometry odometryMsg;
            odometryMsg.header.stamp.sec = 10023;
//            odometryMsg.header.frame_id = 0;
//            odometryMsg.child_frame_id = 0;
            odometryMsg.pose.pose.position.x = 1.12;
            odometryMsg.pose.pose.position.y = 2.25;
            odometryMsg.pose.pose.position.z = -56.1;
            odometryMsg.pose.pose.orientation.x = 0.125;
            odometryMsg.pose.pose.orientation.y = -0.98;
            odometryMsg.pose.pose.orientation.z = 0.05;
            odometryMsg.pose.pose.orientation.w = -9.65;
            odometryMsg.twist.twist.linear.x = 5.56;
            odometryMsg.twist.twist.linear.y = -15.26;
            odometryMsg.twist.twist.linear.z = -99.99;
            odometryMsg.twist.twist.angular.x = 1.56;
            odometryMsg.twist.twist.angular.y = 3.14;
            odometryMsg.twist.twist.angular.z = 6.28;
            GUISubscriberContentOdometry((void*)&odometryMsg);
        }
        else {
            return;
        }

        sprintf(str, "[gui] Subscribe to %s [%s]\n", topic, topicType);
        rt_kprintf(str);

        //Send request to subscribe to this topic
        InterThreadMessageStruct uROSMsg = {.id = id, .data = (uint32_t*)topic, .length = strlen(topic) };
        rt_mq_send(&uROSMessageQueue, (void*)&uROSMsg, sizeof(InterThreadMessageStruct));
    }
}

/**
  * @brief  This function handles button events of this GUI part
  * @param  lv_event_t: Pointer to the triggered event
  * @return None
  */
static void GUISubscriberListButtonEvent(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t* btn = lv_event_get_target(e);
    if(code == LV_EVENT_CLICKED) {
        lv_obj_t* btnLabel = lv_obj_get_child(btn, NULL);
        char* btnLabelStr = lv_label_get_text(btnLabel);
        lv_label_set_text(subscribeTopicTypeLabel, btnLabelStr);

        lv_obj_clear_flag(subscribeTopicPopUpArea, LV_OBJ_FLAG_HIDDEN);
    }
}

/**
  * @brief  This function handles Accel message information display
  * @param  msgin: Pointer to the twist message
  * @return None
  */
void GUISubscriberContentAccel(const void *msgin) {
    const geometry_msgs__msg__Accel * accelMsg = (const geometry_msgs__msg__Accel *)msgin;
    uint16_t index = 0;
    char str[SUBSCRIBER_CONTENT_SIZE];
    index += sprintf(&str[index], "linear:\n");
    index += sprintf(&str[index], "  x: %.2f\n", accelMsg->linear.x);
    index += sprintf(&str[index], "  y: %.2f\n", accelMsg->linear.y);
    index += sprintf(&str[index], "  z: %.2f\n", accelMsg->linear.z);
    index += sprintf(&str[index], "angular:\n");
    index += sprintf(&str[index], "  x: %.2f\n", accelMsg->angular.x);
    index += sprintf(&str[index], "  y: %.2f\n", accelMsg->angular.y);
    index += sprintf(&str[index], "  z: %.2f\n", accelMsg->angular.z);
    lv_label_set_text(subscriberContentLabel, str);
}

/**
  * @brief  This function handles Point message information display
  * @param  msgin: Pointer to the twist message
  * @return None
  */
void GUISubscriberContentPoint(const void *msgin) {
    const geometry_msgs__msg__Point * pointMsg = (const geometry_msgs__msg__Point *)msgin;
    uint16_t index = 0;
    char str[SUBSCRIBER_CONTENT_SIZE];
    index += sprintf(&str[index], "Point:\n");
    index += sprintf(&str[index], "  x: %.2f\n", pointMsg->x);
    index += sprintf(&str[index], "  y: %.2f\n", pointMsg->y);
    index += sprintf(&str[index], "  z: %.2f\n", pointMsg->z);
    lv_label_set_text(subscriberContentLabel, str);
}

/**
  * @brief  This function handles Pose message information display
  * @param  msgin: Pointer to the twist message
  * @return None
  */
void GUISubscriberContentPose(const void *msgin) {
    const geometry_msgs__msg__Pose * poseMsg = (const geometry_msgs__msg__Pose *)msgin;
    uint16_t index = 0;
    char str[SUBSCRIBER_CONTENT_SIZE];
    index += sprintf(&str[index], "Position:\n");
    index += sprintf(&str[index], "  x: %.2f\n", poseMsg->position.x);
    index += sprintf(&str[index], "  y: %.2f\n", poseMsg->position.y);
    index += sprintf(&str[index], "  z: %.2f\n", poseMsg->position.z);
    index += sprintf(&str[index], "Orientation:\n");
    index += sprintf(&str[index], "  x: %.2f\n", poseMsg->orientation.x);
    index += sprintf(&str[index], "  y: %.2f\n", poseMsg->orientation.y);
    index += sprintf(&str[index], "  z: %.2f\n", poseMsg->orientation.z);
    index += sprintf(&str[index], "  w: %.2f\n", poseMsg->orientation.w);
    lv_label_set_text(subscriberContentLabel, str);
}

/**
  * @brief  This function handles Quaternion message information display
  * @param  msgin: Pointer to the twist message
  * @return None
  */
void GUISubscriberContentQuaternion(const void *msgin) {
    const geometry_msgs__msg__Quaternion * quaternionMsg = (const geometry_msgs__msg__Quaternion *)msgin;
    uint16_t index = 0;
    char str[SUBSCRIBER_CONTENT_SIZE];
    index += sprintf(&str[index], "Orientation:\n");
    index += sprintf(&str[index], "  x: %.2f\n", quaternionMsg->x);
    index += sprintf(&str[index], "  y: %.2f\n", quaternionMsg->y);
    index += sprintf(&str[index], "  z: %.2f\n", quaternionMsg->z);
    index += sprintf(&str[index], "  w: %.2f\n", quaternionMsg->w);
    lv_label_set_text(subscriberContentLabel, str);
}

/**
  * @brief  This function handles Transform message information display
  * @param  msgin: Pointer to the twist message
  * @return None
  */
void GUISubscriberContentTransform(const void *msgin) {
    const geometry_msgs__msg__Transform * transformMsg = (const geometry_msgs__msg__Transform *)msgin;
    uint16_t index = 0;
    char str[SUBSCRIBER_CONTENT_SIZE];
    index += sprintf(&str[index], "Translation:\n");
    index += sprintf(&str[index], "  x: %.2f\n", transformMsg->translation.x);
    index += sprintf(&str[index], "  y: %.2f\n", transformMsg->translation.y);
    index += sprintf(&str[index], "  z: %.2f\n", transformMsg->translation.z);
    index += sprintf(&str[index], "Rotation:\n");
    index += sprintf(&str[index], "  x: %.2f\n", transformMsg->rotation.x);
    index += sprintf(&str[index], "  y: %.2f\n", transformMsg->rotation.y);
    index += sprintf(&str[index], "  z: %.2f\n", transformMsg->rotation.z);
    index += sprintf(&str[index], "  w: %.2f\n", transformMsg->rotation.w);
    lv_label_set_text(subscriberContentLabel, str);
}

/**
  * @brief  This function handles Twist message information display
  * @param  msgin: Pointer to the twist message
  * @return None
  */
void GUISubscriberContentTwist(const void *msgin) {
    const geometry_msgs__msg__Twist * twistMsg = (const geometry_msgs__msg__Twist *)msgin;
    uint16_t index = 0;
    char str[SUBSCRIBER_CONTENT_SIZE];
    index += sprintf(&str[index], "linear:\n");
    index += sprintf(&str[index], "  x: %.2f\n", twistMsg->linear.x);
    index += sprintf(&str[index], "  y: %.2f\n", twistMsg->linear.y);
    index += sprintf(&str[index], "  z: %.2f\n", twistMsg->linear.z);
    index += sprintf(&str[index], "angular:\n");
    index += sprintf(&str[index], "  x: %.2f\n", twistMsg->angular.x);
    index += sprintf(&str[index], "  y: %.2f\n", twistMsg->angular.y);
    index += sprintf(&str[index], "  z: %.2f\n", twistMsg->angular.z);
    lv_label_set_text(subscriberContentLabel, str);
}

/**
  * @brief  This function handles Vector3 message information display
  * @param  msgin: Pointer to the twist message
  * @return None
  */
void GUISubscriberContentVector3(const void *msgin) {
    const geometry_msgs__msg__Vector3 * vector3Msg = (const geometry_msgs__msg__Vector3 *)msgin;
    uint16_t index = 0;
    char str[SUBSCRIBER_CONTENT_SIZE];
    index += sprintf(&str[index], "Vector 3D:\n");
    index += sprintf(&str[index], "  x: %.2f\n", vector3Msg->x);
    index += sprintf(&str[index], "  y: %.2f\n", vector3Msg->y);
    index += sprintf(&str[index], "  z: %.2f\n", vector3Msg->z);
    lv_label_set_text(subscriberContentLabel, str);
}

/**
  * @brief  This function handles Odometry message information display
  * @param  msgin: Pointer to the twist message
  * @return None
  */
void GUISubscriberContentOdometry(const void *msgin) {
    const nav_msgs__msg__Odometry * odometryMsg = (const nav_msgs__msg__Odometry *)msgin;
    uint16_t index = 0;
    char str[SUBSCRIBER_CONTENT_SIZE];
    index += sprintf(&str[index], "Header:\n");
    index += sprintf(&str[index], "  Stamp: %d; Frame ID: \n", odometryMsg->header.stamp.sec); //odometryMsg->header.frame_id
    index += sprintf(&str[index], "  Child Frame ID: \n");  //odometryMsg->child_frame_id
    index += sprintf(&str[index], "Pose:\n");
    index += sprintf(&str[index], "  Position: x: %.2f; y: %.2f; z: %.2f\n", odometryMsg->pose.pose.position.x, odometryMsg->pose.pose.position.y, odometryMsg->pose.pose.position.z);
    index += sprintf(&str[index], "  Orientation: x: %.2f; y: %.2f; z: %.2f; w: %.2f\n", odometryMsg->pose.pose.orientation.x, odometryMsg->pose.pose.orientation.y, odometryMsg->pose.pose.orientation.z, odometryMsg->pose.pose.orientation.w);
    index += sprintf(&str[index], "Twist:\n");
    index += sprintf(&str[index], "  Linear: x: %.2f; y: %.2f; z: %.2f\n", odometryMsg->twist.twist.linear.x, odometryMsg->twist.twist.linear.y, odometryMsg->twist.twist.linear.z);
    index += sprintf(&str[index], "  Angular: x: %.2f; y: %.2f; z: %.2f\n", odometryMsg->twist.twist.angular.x, odometryMsg->twist.twist.angular.y, odometryMsg->twist.twist.angular.z);
    lv_label_set_text(subscriberContentLabel, str);
}

/**
  * @brief  This function is used to update more complex GUI elements in this GUI part
  * @param  None
  * @return None
  */
void GUISubscriberWindowUpdate(rcl_names_and_types_t* topics) {
    uint8_t topicCnt = topics->names.size;

    char str[200];
    lv_obj_t * listButton;
    uint8_t i;
    for(i = 0; i < topicCnt; i++) {
        sprintf(str, "%s [%s]", topics->names.data[i], topics->types->data[i]);
        listButton = lv_list_add_btn(subscriberList, NULL, str);
        lv_obj_set_style_bg_color(listButton, STYLE_COLOR_LIGHT_GREY, LV_PART_MAIN);
        lv_obj_set_style_text_color(listButton, STYLE_COLOR_WHITE, LV_PART_MAIN);
        //Add event
        lv_obj_add_event_cb(listButton, GUISubscriberListButtonEvent, LV_EVENT_CLICKED, NULL);
    }
}

/**
  * @brief  This function initializes this GUI part
  * @param  None
  * @return None
  */
void GUISubscriberWindowInit() {
    //Set Subscriber Window
    //Subscriber Window Area
    subscriberWindowArea = lv_obj_create(lv_scr_act());
    lv_obj_remove_style_all(subscriberWindowArea);
    lv_obj_add_style(subscriberWindowArea, &mainStyle, LV_PART_MAIN);
    lv_obj_set_style_bg_color(subscriberWindowArea, STYLE_COLOR_LIGHT_GREY, LV_PART_MAIN);
    lv_obj_set_style_border_width(subscriberWindowArea, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(subscriberWindowArea, 0, LV_PART_MAIN);
    lv_obj_set_size(subscriberWindowArea, 480, 222);        //LV_VER_RES_MAX - 25 - 25
    lv_obj_align(subscriberWindowArea, LV_ALIGN_TOP_LEFT, 0, 25);
    lv_obj_add_flag(subscriberWindowArea, LV_OBJ_FLAG_HIDDEN);

    //-------------------------------------------------------------------------//
    //                       Subscribable Topic List                           //
    //-------------------------------------------------------------------------//
    //Topic List
    subscriberList = lv_list_create(subscriberWindowArea);
    lv_obj_remove_style_all(subscriberList);
    lv_obj_add_style(subscriberList, &mainStyle, LV_PART_MAIN);
    lv_obj_set_style_bg_color(subscriberList, STYLE_COLOR_LIGHT_GREY, LV_PART_MAIN);
//    lv_obj_set_style_border_color(subscriberList, STYLE_COLOR_BLACK, LV_PART_MAIN);
    lv_obj_set_size(subscriberList, 142, 224);
    lv_obj_set_style_pad_all(subscriberList, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_gap(subscriberList, 0, LV_PART_MAIN);
    lv_obj_align(subscriberList, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_set_flex_flow(subscriberList, LV_FLEX_FLOW_COLUMN);
//    lv_obj_set_scrollbar_mode(subscriberList, LV_SCROLLBAR_MODE_ON);

    //Geometry Messages
    //Add Label to list
    lv_obj_t* listLabel = lv_list_add_text(subscriberList, "Geometry");
    lv_obj_set_style_bg_color(listLabel, STYLE_COLOR_DARK_GREY, LV_PART_MAIN);
    lv_obj_set_style_text_color(listLabel, STYLE_COLOR_GREEN, LV_PART_MAIN);
    lv_obj_set_style_border_width(listLabel, 0, LV_PART_MAIN);
    lv_obj_set_height(listLabel, 22);
    lv_obj_set_style_pad_all(listLabel, 2, LV_PART_MAIN);

    //Add buttons to list
    lv_obj_t* listButton = lv_list_add_btn(subscriberList, NULL, "Accel");
    lv_obj_set_style_bg_color(listButton, STYLE_COLOR_LIGHT_GREY, LV_PART_MAIN);
    lv_obj_set_style_text_color(listButton, STYLE_COLOR_WHITE, LV_PART_MAIN);
    lv_obj_set_height(listButton, 25);
    //Add event
    lv_obj_add_event_cb(listButton, GUISubscriberListButtonEvent, LV_EVENT_CLICKED, NULL);

    //Add buttons to list
    listButton = lv_list_add_btn(subscriberList, NULL, "Point");
    lv_obj_set_style_bg_color(listButton, STYLE_COLOR_LIGHT_GREY, LV_PART_MAIN);
    lv_obj_set_style_text_color(listButton, STYLE_COLOR_WHITE, LV_PART_MAIN);
    lv_obj_set_height(listButton, 25);
    //Add event
    lv_obj_add_event_cb(listButton, GUISubscriberListButtonEvent, LV_EVENT_CLICKED, NULL);

    //Add buttons to list
    listButton = lv_list_add_btn(subscriberList, NULL, "Pose");
    lv_obj_set_style_bg_color(listButton, STYLE_COLOR_LIGHT_GREY, LV_PART_MAIN);
    lv_obj_set_style_text_color(listButton, STYLE_COLOR_WHITE, LV_PART_MAIN);
    lv_obj_set_height(listButton, 25);
    //Add event
    lv_obj_add_event_cb(listButton, GUISubscriberListButtonEvent, LV_EVENT_CLICKED, NULL);

    //Add buttons to list
    listButton = lv_list_add_btn(subscriberList, NULL, "Quaternion");
    lv_obj_set_style_bg_color(listButton, STYLE_COLOR_LIGHT_GREY, LV_PART_MAIN);
    lv_obj_set_style_text_color(listButton, STYLE_COLOR_WHITE, LV_PART_MAIN);
    lv_obj_set_height(listButton, 25);
    //Add event
    lv_obj_add_event_cb(listButton, GUISubscriberListButtonEvent, LV_EVENT_CLICKED, NULL);

    //Add buttons to list
    listButton = lv_list_add_btn(subscriberList, NULL, "Transform");
    lv_obj_set_style_bg_color(listButton, STYLE_COLOR_LIGHT_GREY, LV_PART_MAIN);
    lv_obj_set_style_text_color(listButton, STYLE_COLOR_WHITE, LV_PART_MAIN);
    lv_obj_set_height(listButton, 25);
    //Add event
    lv_obj_add_event_cb(listButton, GUISubscriberListButtonEvent, LV_EVENT_CLICKED, NULL);

    //Add buttons to list
    listButton = lv_list_add_btn(subscriberList, NULL, "Twist");
    lv_obj_set_style_bg_color(listButton, STYLE_COLOR_LIGHT_GREY, LV_PART_MAIN);
    lv_obj_set_style_text_color(listButton, STYLE_COLOR_WHITE, LV_PART_MAIN);
    lv_obj_set_height(listButton, 25);
    //Add event
    lv_obj_add_event_cb(listButton, GUISubscriberListButtonEvent, LV_EVENT_CLICKED, NULL);

    //Add buttons to list
    listButton = lv_list_add_btn(subscriberList, NULL, "Vector3");
    lv_obj_set_style_bg_color(listButton, STYLE_COLOR_LIGHT_GREY, LV_PART_MAIN);
    lv_obj_set_style_text_color(listButton, STYLE_COLOR_WHITE, LV_PART_MAIN);
    lv_obj_set_height(listButton, 25);
    //Add event
    lv_obj_add_event_cb(listButton, GUISubscriberListButtonEvent, LV_EVENT_CLICKED, NULL);

    //Navigation Messages
    //Add Label to list
    listLabel = lv_list_add_text(subscriberList, "Navigation");
    lv_obj_set_style_bg_color(listLabel, STYLE_COLOR_DARK_GREY, LV_PART_MAIN);
    lv_obj_set_style_text_color(listLabel, STYLE_COLOR_GREEN, LV_PART_MAIN);
    lv_obj_set_style_border_width(listLabel, 0, LV_PART_MAIN);
    lv_obj_set_height(listLabel, 22);
    lv_obj_set_style_pad_all(listLabel, 2, LV_PART_MAIN);

    listButton = lv_list_add_btn(subscriberList, NULL, "Odometry");
    lv_obj_set_style_bg_color(listButton, STYLE_COLOR_LIGHT_GREY, LV_PART_MAIN);
    lv_obj_set_style_text_color(listButton, STYLE_COLOR_WHITE, LV_PART_MAIN);
    lv_obj_set_height(listButton, 25);
    //Add event
    lv_obj_add_event_cb(listButton, GUISubscriberListButtonEvent, LV_EVENT_CLICKED, NULL);

    //--------------------------------------------------------------------------//
    //                      Subscribed Topic Info Area                          //
    //--------------------------------------------------------------------------//
    //Subscriber Info Area
    subscriberInfoArea = lv_obj_create(subscriberWindowArea);
    lv_obj_remove_style_all(subscriberInfoArea);
    lv_obj_add_style(subscriberInfoArea, &mainStyle, LV_PART_MAIN);
    lv_obj_set_style_bg_color(subscriberInfoArea, STYLE_COLOR_LIGHT_GREY, LV_PART_MAIN);
//    lv_obj_set_style_border_color(subscriberInfoArea, STYLE_COLOR_DARKER_GREY, LV_PART_MAIN);
    lv_obj_set_style_pad_all(subscriberInfoArea, 0, LV_PART_MAIN);
    lv_obj_set_size(subscriberInfoArea, 340, 224);
    lv_obj_align(subscriberInfoArea, LV_ALIGN_TOP_LEFT, 140, 0);

    subscriberInfoHeaderLabel = lv_label_create(subscriberInfoArea);
    lv_obj_remove_style_all(subscriberInfoHeaderLabel);
    lv_obj_add_style(subscriberInfoHeaderLabel, &mainStyle, LV_PART_MAIN);
    lv_obj_set_style_bg_color(subscriberInfoHeaderLabel, STYLE_COLOR_GREY, LV_PART_MAIN);
    lv_obj_set_style_border_width(subscriberInfoHeaderLabel, 0, LV_PART_MAIN);
//    lv_obj_set_style_border_side(subscriberInfoHeaderLabel, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN);
    lv_obj_set_size(subscriberInfoHeaderLabel, 340, 30);
    lv_obj_align(subscriberInfoHeaderLabel, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_set_style_text_align(subscriberInfoHeaderLabel, LV_TEXT_ALIGN_CENTER, 0);
    lv_label_set_text(subscriberInfoHeaderLabel, "None [None]");

    subscriberContentLabel = lv_label_create(subscriberInfoArea);
    lv_obj_remove_style_all(subscriberContentLabel);
    lv_obj_add_style(subscriberContentLabel, &mainStyle, LV_PART_MAIN);
    lv_obj_set_style_border_width(subscriberContentLabel, 0, LV_PART_MAIN);
//    lv_obj_set_style_text_font(subscriberContentLabel, &lv_font_montserrat_12, LV_PART_MAIN);
    lv_obj_set_size(subscriberContentLabel, 336, 190);                   //W: 340 - 4; H: 224 - 30 - 2 - 2
    lv_obj_align(subscriberContentLabel, LV_ALIGN_TOP_LEFT, 0, 30);

    //--------------------------------------------------------------------------//
    //                  Subscribe to Topic Pop Up Window                        //
    //--------------------------------------------------------------------------//
    subscribeTopicPopUpArea = lv_win_create(lv_scr_act(), 30);
    lv_win_add_title(subscribeTopicPopUpArea, "Subscribe to Topic");
    lv_obj_set_style_bg_color(subscribeTopicPopUpArea, STYLE_COLOR_BLACK, LV_PART_MAIN);
    lv_obj_set_style_pad_all(subscribeTopicPopUpArea, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(subscribeTopicPopUpArea, 2, LV_PART_MAIN);
    lv_obj_set_style_border_color(subscribeTopicPopUpArea, STYLE_COLOR_GREY, LV_PART_MAIN);
    lv_obj_set_style_outline_width(subscribeTopicPopUpArea, 3, LV_PART_MAIN);
    lv_obj_set_style_outline_color(subscribeTopicPopUpArea, STYLE_COLOR_DARK_GREEN, LV_PART_MAIN);
    lv_obj_set_size(subscribeTopicPopUpArea, 250, 130);
    lv_obj_align(subscribeTopicPopUpArea, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_flag(subscribeTopicPopUpArea, LV_OBJ_FLAG_HIDDEN);

    lv_obj_t* btn = lv_win_add_btn(subscribeTopicPopUpArea, LV_SYMBOL_CLOSE, 60);
    lv_obj_remove_style_all(btn);
    lv_obj_add_style(btn, &mainStyle, 0);
    lv_obj_add_style(btn, &mainStylePressed, LV_STATE_PRESSED);
    lv_obj_set_width(btn, 25);
    lv_obj_add_event_cb(btn, GUISubscribeTopicPopUpCloseButtonEvent, LV_EVENT_CLICKED, NULL);

    lv_obj_t * head = lv_win_get_header(subscribeTopicPopUpArea);
    lv_obj_set_style_bg_color(head, STYLE_COLOR_GREY, LV_PART_MAIN);
    lv_obj_set_style_text_color(head, STYLE_COLOR_WHITE, LV_PART_MAIN);

    lv_obj_t * cont = lv_win_get_content(subscribeTopicPopUpArea);
    lv_obj_set_style_bg_color(cont, STYLE_COLOR_BLACK, LV_PART_MAIN);
    lv_obj_set_style_pad_all(cont, 4, LV_PART_MAIN);

    //Create Topic Type Label
    lv_obj_t* label = lv_label_create(cont);
    lv_obj_remove_style_all(label);
    lv_obj_add_style(label, &mainStyle, LV_PART_MAIN);
    lv_obj_set_style_border_width(label, 0, LV_PART_MAIN);
    lv_obj_set_width(label, 120);
    lv_obj_align(label, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_label_set_text(label, "Topic Type:");

    subscribeTopicTypeLabel  = lv_label_create(cont);
    lv_obj_remove_style_all(subscribeTopicTypeLabel);
    lv_obj_add_style(subscribeTopicTypeLabel, &mainStyle, LV_PART_MAIN);
    lv_obj_set_style_bg_color(subscribeTopicTypeLabel, STYLE_COLOR_DARK_GREY, LV_PART_MAIN);
    lv_obj_set_width(subscribeTopicTypeLabel, 120);
    lv_obj_align(subscribeTopicTypeLabel, LV_ALIGN_TOP_RIGHT, 0, 0);
    lv_label_set_text(subscribeTopicTypeLabel, "Topic Type");

    //Create Topic Name Text Area
    label = lv_label_create(cont);
    lv_obj_remove_style_all(label);
    lv_obj_add_style(label, &mainStyle, LV_PART_MAIN);
    lv_obj_set_style_border_width(label, 0, LV_PART_MAIN);
    lv_obj_set_width(label, 120);
    lv_obj_align(label, LV_ALIGN_TOP_LEFT, 0, 33);
    lv_label_set_text(label, "Topic Name:");

    subscribeTopicTextArea = lv_textarea_create(cont);
    lv_obj_remove_style_all(subscribeTopicTextArea);
    lv_obj_add_style(subscribeTopicTextArea, &mainStyle, LV_PART_MAIN);
    lv_textarea_set_one_line(subscribeTopicTextArea, true);
    lv_textarea_set_max_length(subscribeTopicTextArea, 9);
    lv_obj_set_style_border_width(subscribeTopicTextArea, 2, LV_PART_CURSOR | LV_STATE_FOCUSED);
    lv_obj_set_style_border_side(subscribeTopicTextArea, LV_BORDER_SIDE_LEFT, LV_PART_CURSOR);
    lv_obj_set_style_border_color(subscribeTopicTextArea, STYLE_COLOR_GREY, LV_PART_CURSOR);
    lv_obj_set_width(subscribeTopicTextArea, 120);
    lv_obj_align(subscribeTopicTextArea, LV_ALIGN_TOP_RIGHT, 0, 33);
    lv_textarea_set_text(subscribeTopicTextArea, "Topic Name");
    lv_obj_add_event_cb(subscribeTopicTextArea, GUISubscribeTopicTextAreaEvent, LV_EVENT_FOCUSED, NULL);
    lv_obj_add_event_cb(subscribeTopicTextArea, GUISubscribeTopicTextAreaEvent, LV_EVENT_DEFOCUSED, NULL);
    lv_obj_add_event_cb(subscribeTopicTextArea, GUISubscribeTopicTextAreaEvent, LV_EVENT_READY, NULL);

    //Create Topic Subscribe Button
    subscribeTopicButton = lv_btn_create(subscribeTopicPopUpArea);
    lv_obj_remove_style_all(subscribeTopicButton);
    lv_obj_add_style(subscribeTopicButton, &mainStyle, LV_PART_MAIN);
    lv_obj_set_style_bg_color(subscribeTopicButton, STYLE_COLOR_LIGHT_GREY, LV_PART_MAIN);
    lv_obj_add_style(subscribeTopicButton, &mainStylePressed, LV_STATE_PRESSED);
    lv_obj_set_size(subscribeTopicButton, 252, 25);
    lv_obj_align(subscribeTopicButton, LV_ALIGN_BOTTOM_LEFT, -2, -2);
    //Button Label
    label = lv_label_create(subscribeTopicButton);
    lv_label_set_text(label, "SUBSCRIBE");
    lv_obj_set_style_border_width(label, 0, LV_PART_MAIN);
    lv_obj_center(label);
    //Add event
    lv_obj_add_event_cb(subscribeTopicButton, GUISubscribeTopicButtonEvent, LV_EVENT_CLICKED, NULL);
}
