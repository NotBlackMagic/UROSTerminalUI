#include "guiPublisherWindow.h"

//Publisher Window  GUI objects
lv_obj_t* publisherWindowArea;
lv_obj_t* publisherList;
lv_obj_t* publisherControlArea;
lv_obj_t* publisherControlHeaderLabel;
lv_obj_t* publisherContentLabel;

lv_obj_t* publishTopicPopUpArea;
lv_obj_t* publishTopicTypeLabel;
lv_obj_t* publishTopicNameTextArea;
lv_obj_t* publishTopicPeriodTextArea;
lv_obj_t* publishTopicPeriodDropDown;
lv_obj_t* publishTopicButton;

/**
  * @brief  This function handles text area events of this GUI part
  * @param  lv_event_t: Pointer to the triggered event
  * @return None
  */
static void GUIPublishTopicNameTextAreaEvent(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * ta = lv_event_get_target(e);
//    lv_obj_t * keyboard = lv_event_get_user_data(e);
    if(code == LV_EVENT_FOCUSED) {
        lv_obj_align(publishTopicPopUpArea, LV_ALIGN_TOP_MID, 0, 3);

        lv_keyboard_set_textarea(keyboard, publishTopicNameTextArea);
        lv_obj_clear_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
        lv_obj_move_foreground(keyboard);
    }

    if(code == LV_EVENT_DEFOCUSED || code == LV_EVENT_READY) {
        lv_obj_align(publishTopicPopUpArea, LV_ALIGN_CENTER, 0, 0);

        lv_keyboard_set_textarea(keyboard, NULL);
        lv_obj_add_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
    }
}

/**
  * @brief  This function handles text area events of this GUI part
  * @param  lv_event_t: Pointer to the triggered event
  * @return None
  */
static void GUIPublishTopicPeriodTextAreaEvent(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * ta = lv_event_get_target(e);
//    lv_obj_t * keyboard = lv_event_get_user_data(e);
    if(code == LV_EVENT_FOCUSED) {
        lv_obj_align(publishTopicPopUpArea, LV_ALIGN_TOP_MID, 0, 3);

        lv_keyboard_set_textarea(keyboard, publishTopicPeriodTextArea);
        lv_obj_clear_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
        lv_obj_move_foreground(keyboard);
    }

    if(code == LV_EVENT_DEFOCUSED || code == LV_EVENT_READY) {
        lv_obj_align(publishTopicPopUpArea, LV_ALIGN_CENTER, 0, 0);

        lv_keyboard_set_textarea(keyboard, NULL);
        lv_obj_add_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
    }
}

/**
  * @brief  This function handles button events of this GUI part
  * @param  lv_event_t: Pointer to the triggered event
  * @return None
  */
static void GUIPublishTopicPopUpCloseButtonEvent(lv_event_t * e) {
    lv_obj_add_flag(publishTopicPopUpArea, LV_OBJ_FLAG_HIDDEN);
}

/**
  * @brief  This function handles button events of this GUI part
  * @param  lv_event_t: Pointer to the triggered event
  * @return None
  */
static void GUIPublishTopicButtonEvent(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t* btn = lv_event_get_target(e);
    if(code == LV_EVENT_CLICKED) {
        char str[64];
        char* topic = lv_textarea_get_text(publishTopicNameTextArea);
        char* topicType = lv_label_get_text(publishTopicTypeLabel);
        sprintf(str, "%s [%s]\n", topic, topicType);
        lv_label_set_text(publisherControlHeaderLabel, str);

        //Hide Publish to Topic Window
        lv_obj_add_flag(publishTopicPopUpArea, LV_OBJ_FLAG_HIDDEN);

        uint16_t selected = lv_dropdown_get_selected(publishTopicPeriodDropDown);
        char topicPeriodTxt[16];
        lv_dropdown_get_selected_str(publishTopicPeriodDropDown, topicPeriodTxt, 16);
        int topicPeriod;
        sscanf(topicPeriodTxt, "%d", &topicPeriod);

        //Convert Subscribe Topic Name to ID number
        int id = UROSThread_Publisher_None;
        if(strcmp(topicType, "Twist") == 0) {
            id = UROSThread_Publisher_Twist;
        }
        else {
            return;
        }

        sprintf(str, "[gui] Publish %s [%s] every %d ms\n", topic, topicType, topicPeriod);
        rt_kprintf(str);

//        InterThreadMessageStruct uROSMsg = {.id = id, .data = (uint32_t*)topic, .length = strlen(topic) };
//        rt_mq_send(&uROSMessageQueue, (void*)&uROSMsg, sizeof(InterThreadMessageStruct));
    }
}

/**
  * @brief  This function handles button events of this GUI part
  * @param  lv_event_t: Pointer to the triggered event
  * @return None
  */
static void GUIPublisherListButtonEvent(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t* btn = lv_event_get_target(e);
    if(code == LV_EVENT_CLICKED) {
        lv_obj_t* btnLabel = lv_obj_get_child(btn, NULL);
        char* btnLabelStr = lv_label_get_text(btnLabel);
        lv_label_set_text(publishTopicTypeLabel, btnLabelStr);

        lv_obj_clear_flag(publishTopicPopUpArea, LV_OBJ_FLAG_HIDDEN);
    }
}

/**
  * @brief  This function initializes this GUI part
  * @param  None
  * @return None
  */
void GUIPublisherWindowInit() {
    //Set Publisher Window
    //Publisher Window Area
    publisherWindowArea = lv_obj_create(lv_scr_act());
    lv_obj_remove_style_all(publisherWindowArea);
    lv_obj_add_style(publisherWindowArea, &mainStyle, LV_PART_MAIN);
    lv_obj_set_style_bg_color(publisherWindowArea, STYLE_COLOR_LIGHT_GREY, LV_PART_MAIN);
    lv_obj_set_style_border_width(publisherWindowArea, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(publisherWindowArea, 0, LV_PART_MAIN);
    lv_obj_set_size(publisherWindowArea, 480, 222);        //LV_VER_RES_MAX - 25 - 25
    lv_obj_align(publisherWindowArea, LV_ALIGN_TOP_LEFT, 0, 25);
    lv_obj_add_flag(publisherWindowArea, LV_OBJ_FLAG_HIDDEN);

    //--------------------------------------------------------------------------//
    //                       Publishable Topic List                             //
    //--------------------------------------------------------------------------//
    //Topic List
    publisherList = lv_list_create(publisherWindowArea);
    lv_obj_remove_style_all(publisherList);
    lv_obj_add_style(publisherList, &mainStyle, LV_PART_MAIN);
    lv_obj_set_style_bg_color(publisherList, STYLE_COLOR_LIGHT_GREY, LV_PART_MAIN);
//    lv_obj_set_style_border_color(publisherList, STYLE_COLOR_BLACK, LV_PART_MAIN);
    lv_obj_set_size(publisherList, 142, 224);
    lv_obj_set_style_pad_all(publisherList, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_gap(publisherList, 0, LV_PART_MAIN);
    lv_obj_align(publisherList, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_set_flex_flow(publisherList, LV_FLEX_FLOW_COLUMN);
//    lv_obj_set_scrollbar_mode(publisherList, LV_SCROLLBAR_MODE_ON);

    //Geometry Messages
    //Add Label to list
    lv_obj_t* listLabel = lv_list_add_text(publisherList, "Geometry");
    lv_obj_set_style_bg_color(listLabel, STYLE_COLOR_DARK_GREY, LV_PART_MAIN);
    lv_obj_set_style_text_color(listLabel, STYLE_COLOR_GREEN, LV_PART_MAIN);
    lv_obj_set_style_border_width(listLabel, 0, LV_PART_MAIN);
    lv_obj_set_height(listLabel, 22);
    lv_obj_set_style_pad_all(listLabel, 2, LV_PART_MAIN);

    //Add buttons to list
    lv_obj_t* listButton = lv_list_add_btn(publisherList, NULL, "Twist");
    lv_obj_set_style_bg_color(listButton, STYLE_COLOR_LIGHT_GREY, LV_PART_MAIN);
    lv_obj_set_style_text_color(listButton, STYLE_COLOR_WHITE, LV_PART_MAIN);
    lv_obj_set_height(listButton, 25);
    //Add event
    lv_obj_add_event_cb(listButton, GUIPublisherListButtonEvent, LV_EVENT_CLICKED, NULL);

    //--------------------------------------------------------------------------//
    //                      Published Topic Control Area                        //
    //--------------------------------------------------------------------------//
    //Subscriber Info Area
    publisherControlArea = lv_obj_create(publisherWindowArea);
    lv_obj_remove_style_all(publisherControlArea);
    lv_obj_add_style(publisherControlArea, &mainStyle, LV_PART_MAIN);
    lv_obj_set_style_bg_color(publisherControlArea, STYLE_COLOR_LIGHT_GREY, LV_PART_MAIN);
//    lv_obj_set_style_border_color(publisherControlArea, STYLE_COLOR_DARKER_GREY, LV_PART_MAIN);
    lv_obj_set_style_pad_all(publisherControlArea, 0, LV_PART_MAIN);
    lv_obj_set_size(publisherControlArea, 340, 224);
    lv_obj_align(publisherControlArea, LV_ALIGN_TOP_LEFT, 140, 0);

    publisherControlHeaderLabel = lv_label_create(publisherControlArea);
    lv_obj_remove_style_all(publisherControlHeaderLabel);
    lv_obj_add_style(publisherControlHeaderLabel, &mainStyle, LV_PART_MAIN);
    lv_obj_set_style_bg_color(publisherControlHeaderLabel, STYLE_COLOR_GREY, LV_PART_MAIN);
    lv_obj_set_style_border_width(publisherControlHeaderLabel, 0, LV_PART_MAIN);
//    lv_obj_set_style_border_side(publisherControlHeaderLabel, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN);
    lv_obj_set_size(publisherControlHeaderLabel, 340, 30);
    lv_obj_align(publisherControlHeaderLabel, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_set_style_text_align(publisherControlHeaderLabel, LV_TEXT_ALIGN_CENTER, 0);
    lv_label_set_text(publisherControlHeaderLabel, "None [None]");

//    subscriberContentLabel = lv_label_create(subscriberInfoArea);
//    lv_obj_remove_style_all(subscriberContentLabel);
//    lv_obj_add_style(subscriberContentLabel, &mainStyle, LV_PART_MAIN);
//    lv_obj_set_style_border_width(subscriberContentLabel, 0, LV_PART_MAIN);
////    lv_obj_set_style_text_font(subscriberContentLabel, &lv_font_montserrat_12, LV_PART_MAIN);
//    lv_obj_set_size(subscriberContentLabel, 336, 190);                   //W: 340 - 4; H: 224 - 30 - 2 - 2
//    lv_obj_align(subscriberContentLabel, LV_ALIGN_TOP_LEFT, 0, 30);

    //--------------------------------------------------------------------------//
    //                      Publish to Topic Pop Up Window                      //
    //--------------------------------------------------------------------------//
    publishTopicPopUpArea = lv_win_create(lv_scr_act(), 30);
    lv_win_add_title(publishTopicPopUpArea, "Publish a Topic");
    lv_obj_set_style_bg_color(publishTopicPopUpArea, STYLE_COLOR_BLACK, LV_PART_MAIN);
    lv_obj_set_style_pad_all(publishTopicPopUpArea, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(publishTopicPopUpArea, 2, LV_PART_MAIN);
    lv_obj_set_style_border_color(publishTopicPopUpArea, STYLE_COLOR_GREY, LV_PART_MAIN);
    lv_obj_set_style_outline_width(publishTopicPopUpArea, 3, LV_PART_MAIN);
    lv_obj_set_style_outline_color(publishTopicPopUpArea, STYLE_COLOR_DARK_GREEN, LV_PART_MAIN);
    lv_obj_set_size(publishTopicPopUpArea, 250, 165);
    lv_obj_align(publishTopicPopUpArea, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_flag(publishTopicPopUpArea, LV_OBJ_FLAG_HIDDEN);

    lv_obj_t* btn = lv_win_add_btn(publishTopicPopUpArea, LV_SYMBOL_CLOSE, 60);
    lv_obj_remove_style_all(btn);
    lv_obj_add_style(btn, &mainStyle, 0);
    lv_obj_add_style(btn, &mainStylePressed, LV_STATE_PRESSED);
    lv_obj_set_width(btn, 25);
    lv_obj_add_event_cb(btn, GUIPublishTopicPopUpCloseButtonEvent, LV_EVENT_CLICKED, NULL);

    lv_obj_t* head = lv_win_get_header(publishTopicPopUpArea);
    lv_obj_set_style_bg_color(head, STYLE_COLOR_GREY, LV_PART_MAIN);
    lv_obj_set_style_text_color(head, STYLE_COLOR_WHITE, LV_PART_MAIN);

    lv_obj_t* cont = lv_win_get_content(publishTopicPopUpArea);
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

    publishTopicTypeLabel  = lv_label_create(cont);
    lv_obj_remove_style_all(publishTopicTypeLabel);
    lv_obj_add_style(publishTopicTypeLabel, &mainStyle, LV_PART_MAIN);
    lv_obj_set_style_bg_color(publishTopicTypeLabel, STYLE_COLOR_DARK_GREY, LV_PART_MAIN);
    lv_obj_set_width(publishTopicTypeLabel, 120);
    lv_obj_align(publishTopicTypeLabel, LV_ALIGN_TOP_RIGHT, 0, 0);
    lv_label_set_text(publishTopicTypeLabel, "Topic Type");

    //Create Topic Name Text Area
    label = lv_label_create(cont);
    lv_obj_remove_style_all(label);
    lv_obj_add_style(label, &mainStyle, LV_PART_MAIN);
    lv_obj_set_style_border_width(label, 0, LV_PART_MAIN);
    lv_obj_set_width(label, 120);
    lv_obj_align(label, LV_ALIGN_TOP_LEFT, 0, 33);
    lv_label_set_text(label, "Topic Name:");

    publishTopicNameTextArea = lv_textarea_create(cont);
    lv_obj_remove_style_all(publishTopicNameTextArea);
    lv_obj_add_style(publishTopicNameTextArea, &mainStyle, LV_PART_MAIN);
    lv_textarea_set_one_line(publishTopicNameTextArea, true);
    lv_textarea_set_max_length(publishTopicNameTextArea, 9);
    lv_obj_set_style_border_width(publishTopicNameTextArea, 2, LV_PART_CURSOR | LV_STATE_FOCUSED);
    lv_obj_set_style_border_side(publishTopicNameTextArea, LV_BORDER_SIDE_LEFT, LV_PART_CURSOR);
    lv_obj_set_style_border_color(publishTopicNameTextArea, STYLE_COLOR_GREY, LV_PART_CURSOR);
    lv_obj_set_width(publishTopicNameTextArea, 120);
    lv_obj_align(publishTopicNameTextArea, LV_ALIGN_TOP_RIGHT, 0, 33);
    lv_textarea_set_text(publishTopicNameTextArea, "Topic Name");
    lv_obj_add_event_cb(publishTopicNameTextArea, GUIPublishTopicNameTextAreaEvent, LV_EVENT_FOCUSED, NULL);
    lv_obj_add_event_cb(publishTopicNameTextArea, GUIPublishTopicNameTextAreaEvent, LV_EVENT_DEFOCUSED, NULL);
    lv_obj_add_event_cb(publishTopicNameTextArea, GUIPublishTopicNameTextAreaEvent, LV_EVENT_READY, NULL);

    //Create Topic Period Text Area
    label = lv_label_create(cont);
    lv_obj_remove_style_all(label);
    lv_obj_add_style(label, &mainStyle, LV_PART_MAIN);
    lv_obj_set_style_border_width(label, 0, LV_PART_MAIN);
    lv_obj_set_width(label, 120);
    lv_obj_align(label, LV_ALIGN_TOP_LEFT, 0, 66);
    lv_label_set_text(label, "Topic Period:");

//    publishTopicPeriodTextArea = lv_textarea_create(cont);
//    lv_obj_remove_style_all(publishTopicPeriodTextArea);
//    lv_obj_add_style(publishTopicPeriodTextArea, &mainStyle, LV_PART_MAIN);
//    lv_textarea_set_one_line(publishTopicPeriodTextArea, true);
//    lv_textarea_set_max_length(publishTopicPeriodTextArea, 9);
//    lv_obj_set_style_border_width(publishTopicPeriodTextArea, 2, LV_PART_CURSOR | LV_STATE_FOCUSED);
//    lv_obj_set_style_border_side(publishTopicPeriodTextArea, LV_BORDER_SIDE_LEFT, LV_PART_CURSOR);
//    lv_obj_set_style_border_color(publishTopicPeriodTextArea, STYLE_COLOR_GREY, LV_PART_CURSOR);
//    lv_obj_set_width(publishTopicPeriodTextArea, 120);
//    lv_obj_align(publishTopicPeriodTextArea, LV_ALIGN_TOP_RIGHT, 0, 66);
//    lv_textarea_set_text(publishTopicPeriodTextArea, "1000");
//    lv_obj_add_event_cb(publishTopicPeriodTextArea, GUIPublishTopicPeriodTextAreaEvent, LV_EVENT_FOCUSED, NULL);
//    lv_obj_add_event_cb(publishTopicPeriodTextArea, GUIPublishTopicPeriodTextAreaEvent, LV_EVENT_DEFOCUSED, NULL);
//    lv_obj_add_event_cb(publishTopicPeriodTextArea, GUIPublishTopicPeriodTextAreaEvent, LV_EVENT_READY, NULL);

    publishTopicPeriodDropDown = lv_dropdown_create(cont);
    lv_dropdown_set_options(publishTopicPeriodDropDown, "TRIGGER\n"
                                                        "50\n"
                                                        "100\n"
                                                        "200\n"
                                                        "500\n"
                                                        "1000");
    lv_obj_remove_style_all(publishTopicPeriodDropDown);
    lv_obj_add_style(publishTopicPeriodDropDown, &mainStyle, 0);
    lv_dropdown_set_symbol(publishTopicPeriodDropDown, NULL);
    lv_obj_set_width(publishTopicPeriodDropDown, 120);
    lv_obj_align(publishTopicPeriodDropDown, LV_ALIGN_TOP_RIGHT, 0, 66);
    //Style Drop Down list items
    lv_obj_t* list = lv_dropdown_get_list(publishTopicPeriodDropDown);
    lv_obj_remove_style_all(list);
    lv_obj_add_style(list, &mainStyle, LV_PART_MAIN);
    lv_obj_add_style(list, &mainStyleSelected, LV_PART_SELECTED);
    lv_obj_set_style_text_line_space(list, 6, LV_PART_MAIN);
    //Add event
//    lv_obj_add_event_cb(publishTopicPeriodDropDown, GUIBasicRadioConfigDropDownEvent, LV_EVENT_VALUE_CHANGED, NULL);

    //Create Topic Publish Button
    publishTopicButton = lv_btn_create(publishTopicPopUpArea);
    lv_obj_remove_style_all(publishTopicButton);
    lv_obj_add_style(publishTopicButton, &mainStyle, LV_PART_MAIN);
    lv_obj_set_style_bg_color(publishTopicButton, STYLE_COLOR_LIGHT_GREY, LV_PART_MAIN);
    lv_obj_add_style(publishTopicButton, &mainStylePressed, LV_STATE_PRESSED);
    lv_obj_set_size(publishTopicButton, 252, 25);
    lv_obj_align(publishTopicButton, LV_ALIGN_BOTTOM_LEFT, -2, -2);
    //Button Label
    label = lv_label_create(publishTopicButton);
    lv_label_set_text(label, "PUBLISH");
    lv_obj_set_style_border_width(label, 0, LV_PART_MAIN);
    lv_obj_center(label);
    //Add event
    lv_obj_add_event_cb(publishTopicButton, GUIPublishTopicButtonEvent, LV_EVENT_CLICKED, NULL);
}
