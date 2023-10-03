#include "guiSubscriberWindow.h"

#define TOPIC_CONTENT_LINES                11
#define TOPIC_CONTENT_LINE_CHARS           36
#define TOPIC_CONTENT_SIZE                 (TOPIC_CONTENT_LINE_CHARS * TOPIC_CONTENT_LINES)

//Subscriber Window  GUI objects
lv_obj_t* topicListWindowArea;
lv_obj_t* topicList;
lv_obj_t* topicInfoArea;
lv_obj_t* topicInfoHeaderLabel;
lv_obj_t* topicContentLabel;

/**
  * @brief  This function handles button events of this GUI part
  * @param  lv_event_t: Pointer to the triggered event
  * @return None
  */
static void GUITopicListButtonEvent(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t* btn = lv_event_get_target(e);
    if(code == LV_EVENT_CLICKED) {
        lv_obj_t* btnLabel = lv_obj_get_child(btn, NULL);
        char* btnLabelStr = lv_label_get_text(btnLabel);
        lv_label_set_text(topicInfoHeaderLabel, btnLabelStr);

        //Get topic name and type from pressed button label: "cmd_vel [geometry_msgs/msg/Twist]"
        //First half of string is topic name, e.g. "cmd_vel"
//        char* topic = strtok(btnLabelStr, " ");
        //Second half of string is topic type, e.g. "[geometry_msgs/msg/Twist]"
//        char* type = strtok(NULL, " ");

        char topic[64];
        char type[64];
        sscanf(btnLabelStr, "%s %s", topic, type);
        //Remove brackets around topic type string aka remove first, '[', and last character, ']',
        type[strlen(type) - 1] = '\0';
        char* typePtr = &type[1];

        int topicSubscriberCount = 2;
        int topicPublisherCount = 3;

        char* topicSubscriberNodes[3] = {"VelCtrl", "Local", "None"};
        char* topicPublisherNodes[3] = {"RC", "Manual", "Auto"};

        uint16_t index = 0;
        char str[TOPIC_CONTENT_SIZE];
        index += sprintf(&str[index], "Topic Name: %s\n", topic);
        index += sprintf(&str[index], "Topic Type: %s\n", typePtr);
        index += sprintf(&str[index], "Subscriber Nodes [%d]:\n", topicSubscriberCount);
        index += sprintf(&str[index], "  %s\n", topicSubscriberNodes[0]);
        index += sprintf(&str[index], "  %s\n", topicSubscriberNodes[1]);
        index += sprintf(&str[index], "Publisher Nodes [%d]:\n", topicPublisherCount);
        index += sprintf(&str[index], "  %s\n", topicPublisherNodes[0]);
        index += sprintf(&str[index], "  %s\n", topicPublisherNodes[1]);
        index += sprintf(&str[index], "  %s\n", topicPublisherNodes[2]);

        lv_label_set_text(topicContentLabel, str);
    }
}

/**
  * @brief  This function is used to update more complex GUI elements in this GUI part
  * @param  None
  * @return None
  */
void GUITopicListWindowUpdate(rcl_names_and_types_t* topics) {
    uint8_t topicCnt = topics->names.size;

    char str[200];
    lv_obj_t * listButton;
    uint8_t i;
    for(i = 0; i < topicCnt; i++) {
        sprintf(str, "%s [%s]", topics->names.data[i], topics->types->data[i]);
        listButton = lv_list_add_btn(topicContentLabel, NULL, str);
        lv_obj_set_style_bg_color(listButton, STYLE_COLOR_LIGHT_GREY, LV_PART_MAIN);
        lv_obj_set_style_text_color(listButton, STYLE_COLOR_WHITE, LV_PART_MAIN);
        //Add event
        lv_obj_add_event_cb(listButton, GUITopicListButtonEvent, LV_EVENT_CLICKED, NULL);
    }
}

/**
  * @brief  This function initializes this GUI part
  * @param  None
  * @return None
  */
void GUITopicListWindowInit() {
    //Set Subscriber Window
    //Subscriber Window Area
    topicListWindowArea = lv_obj_create(lv_scr_act());
    lv_obj_remove_style_all(topicListWindowArea);
    lv_obj_add_style(topicListWindowArea, &mainStyle, LV_PART_MAIN);
    lv_obj_set_style_bg_color(topicListWindowArea, STYLE_COLOR_LIGHT_GREY, LV_PART_MAIN);
    lv_obj_set_style_border_width(topicListWindowArea, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(topicListWindowArea, 0, LV_PART_MAIN);
    lv_obj_set_size(topicListWindowArea, 480, 222);        //LV_VER_RES_MAX - 25 - 25
    lv_obj_align(topicListWindowArea, LV_ALIGN_TOP_LEFT, 0, 25);
    lv_obj_add_flag(topicListWindowArea, LV_OBJ_FLAG_HIDDEN);

    //Topic List
    topicContentLabel = lv_list_create(topicListWindowArea);
    lv_obj_remove_style_all(topicContentLabel);
    lv_obj_add_style(topicContentLabel, &mainStyle, LV_PART_MAIN);
    lv_obj_set_style_bg_color(topicContentLabel, STYLE_COLOR_LIGHT_GREY, LV_PART_MAIN);
    lv_obj_set_size(topicContentLabel, 142, 224);
    lv_obj_set_style_pad_row(topicContentLabel, 0, LV_PART_MAIN);
    lv_obj_align(topicContentLabel, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_set_flex_flow(topicContentLabel, LV_FLEX_FLOW_COLUMN);

    //Add Label to list
//    lv_obj_t * label = lv_list_add_text(topicContentLabel, "Topics");
//    lv_obj_remove_style_all(label);
//    lv_obj_add_style(label, &mainStyle, LV_PART_MAIN);
//    lv_obj_set_style_bg_color(label, STYLE_COLOR_GREY, LV_PART_MAIN);
//    lv_obj_set_style_text_color(label, STYLE_COLOR_WHITE, LV_PART_MAIN);
//    lv_obj_set_style_border_width(label, 0, LV_PART_MAIN);

    //Add buttons to list
    lv_obj_t * listButton = lv_list_add_btn(topicContentLabel, NULL, "cmd_vel [geometry_msgs/msg/Twist]");
    lv_obj_set_style_bg_color(listButton, STYLE_COLOR_LIGHT_GREY, LV_PART_MAIN);
    lv_obj_set_style_text_color(listButton, STYLE_COLOR_WHITE, LV_PART_MAIN);
    //Add event
    lv_obj_add_event_cb(listButton, GUITopicListButtonEvent, LV_EVENT_CLICKED, NULL);

    listButton = lv_list_add_btn(topicContentLabel, NULL, "odom [Odom]");
    lv_obj_set_style_bg_color(listButton, STYLE_COLOR_LIGHT_GREY, LV_PART_MAIN);
    lv_obj_set_style_text_color(listButton, STYLE_COLOR_WHITE, LV_PART_MAIN);
    //Add event
    lv_obj_add_event_cb(listButton, GUITopicListButtonEvent, LV_EVENT_CLICKED, NULL);

    //Subscriber Info Area
    topicInfoArea = lv_obj_create(topicListWindowArea);
    lv_obj_remove_style_all(topicInfoArea);
    lv_obj_add_style(topicInfoArea, &mainStyle, LV_PART_MAIN);
    lv_obj_set_style_bg_color(topicInfoArea, STYLE_COLOR_LIGHT_GREY, LV_PART_MAIN);
//    lv_obj_set_style_border_width(topicInfoArea, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(topicInfoArea, 0, LV_PART_MAIN);
    lv_obj_set_size(topicInfoArea, 340, 224);
    lv_obj_align(topicInfoArea, LV_ALIGN_TOP_LEFT, 140, 0);

    topicInfoHeaderLabel = lv_label_create(topicInfoArea);
    lv_obj_remove_style_all(topicInfoHeaderLabel);
    lv_obj_add_style(topicInfoHeaderLabel, &mainStyle, LV_PART_MAIN);
    lv_obj_set_style_bg_color(topicInfoHeaderLabel, STYLE_COLOR_GREY, LV_PART_MAIN);
    lv_obj_set_style_border_width(topicInfoHeaderLabel, 0, LV_PART_MAIN);
//    lv_obj_set_style_border_side(topicInfoHeaderLabel, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN);
    lv_obj_set_size(topicInfoHeaderLabel, 340, 30);
    lv_obj_align(topicInfoHeaderLabel, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_set_style_text_align(topicInfoHeaderLabel, LV_TEXT_ALIGN_CENTER, 0);
    lv_label_set_text(topicInfoHeaderLabel, "None [None]");

    topicContentLabel = lv_label_create(topicInfoArea);
    lv_obj_remove_style_all(topicContentLabel);
    lv_obj_add_style(topicContentLabel, &mainStyle, LV_PART_MAIN);
    lv_obj_set_style_border_width(topicContentLabel, 0, LV_PART_MAIN);
//    lv_obj_set_style_text_font(topicContentLabel, &lv_font_montserrat_12, LV_PART_MAIN);
    lv_obj_set_size(topicContentLabel, 336, 190);                   //W: 340 - 4; H: 224 - 30 - 2 - 2
    lv_obj_align(topicContentLabel, LV_ALIGN_TOP_LEFT, 0, 30);
}
