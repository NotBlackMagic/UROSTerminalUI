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
        lv_label_set_text(subscriberInfoHeaderLabel, lv_label_get_text(btnLabel));

        //Send request to subscribe to this topic
        char* topic = "cmd_vel";
        InterThreadMessageStruct uROSMsg = {.id = UROSThread_Subscriber_Twist, .data = (uint32_t*)topic, .length = strlen(topic) };
        rt_mq_send(&uROSMessageQueue, (void*)&uROSMsg, sizeof(InterThreadMessageStruct));

        geometry_msgs__msg__Twist twistMsg;
        twistMsg.linear.x = 2.21f;
        twistMsg.linear.y = 1.68f;
        twistMsg.linear.z = -8.94f;
        twistMsg.angular.x = 1.00f;
        twistMsg.angular.y = -98.1;
        twistMsg.angular.z = 7.11f;
        GUISubscriberContentTwist((void*)&twistMsg);
    }
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

    //Topic List
    subscriberList = lv_list_create(subscriberWindowArea);
    lv_obj_remove_style_all(subscriberList);
    lv_obj_add_style(subscriberList, &mainStyle, LV_PART_MAIN);
    lv_obj_set_style_bg_color(subscriberList, STYLE_COLOR_LIGHT_GREY, LV_PART_MAIN);
    lv_obj_set_size(subscriberList, 140, 222);
    lv_obj_set_style_pad_row(subscriberList, 0, LV_PART_MAIN);
    lv_obj_align(subscriberList, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_set_flex_flow(subscriberList, LV_FLEX_FLOW_COLUMN);

    //Add Label to list
//    lv_obj_t * label = lv_list_add_text(subscriberList, "Topics");
//    lv_obj_remove_style_all(label);
//    lv_obj_add_style(label, &mainStyle, LV_PART_MAIN);
//    lv_obj_set_style_bg_color(label, STYLE_COLOR_GREY, LV_PART_MAIN);
//    lv_obj_set_style_text_color(label, STYLE_COLOR_WHITE, LV_PART_MAIN);
//    lv_obj_set_style_border_width(label, 0, LV_PART_MAIN);

    //Add buttons to list
    lv_obj_t * listButton = lv_list_add_btn(subscriberList, NULL, "Twist [Twist]");
    lv_obj_set_style_bg_color(listButton, STYLE_COLOR_LIGHT_GREY, LV_PART_MAIN);
    lv_obj_set_style_text_color(listButton, STYLE_COLOR_WHITE, LV_PART_MAIN);
    //Add event
    lv_obj_add_event_cb(listButton, GUISubscriberListButtonEvent, LV_EVENT_CLICKED, NULL);

    listButton = lv_list_add_btn(subscriberList, NULL, "Odom [Odom]");
    lv_obj_set_style_bg_color(listButton, STYLE_COLOR_LIGHT_GREY, LV_PART_MAIN);
    lv_obj_set_style_text_color(listButton, STYLE_COLOR_WHITE, LV_PART_MAIN);
    //Add event
    lv_obj_add_event_cb(listButton, GUISubscriberListButtonEvent, LV_EVENT_CLICKED, NULL);

    //Subscriber Info Area
    subscriberInfoArea = lv_obj_create(subscriberWindowArea);
    lv_obj_remove_style_all(subscriberInfoArea);
    lv_obj_add_style(subscriberInfoArea, &mainStyle, LV_PART_MAIN);
    lv_obj_set_style_bg_color(subscriberInfoArea, STYLE_COLOR_LIGHT_GREY, LV_PART_MAIN);
//    lv_obj_set_style_border_width(subscriberInfoArea, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(subscriberInfoArea, 0, LV_PART_MAIN);
    lv_obj_set_size(subscriberInfoArea, 340, 222);
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
    lv_obj_set_size(subscriberContentLabel, 336, 188);                   //W: 340 - 4; H: 222 - 30 - 2 - 2
    lv_obj_align(subscriberContentLabel, LV_ALIGN_TOP_LEFT, 0, 30);
}
