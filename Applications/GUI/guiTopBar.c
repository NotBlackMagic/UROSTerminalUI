#include "guiTopBar.h"

//Top bar GUI objects
lv_obj_t* topBarArea;
lv_obj_t* projectNameLabel;
lv_obj_t* connectionLabel;
lv_obj_t* timeLabel;

lv_obj_t* connectionButton;
lv_obj_t* connectionMenuArea;
lv_obj_t* connectionSerialButton;
lv_obj_t* connectionEthernetButton;

/**
  * @brief  This function handles button events of this GUI part
  * @param  lv_event_t: Pointer to the triggered event
  * @return None
  */
static void GUITopBarButtonEvent(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * btn = lv_event_get_target(e);
    if(code == LV_EVENT_CLICKED) {
        if(btn == connectionButton) {
            if(lv_obj_has_flag(connectionMenuArea, LV_OBJ_FLAG_HIDDEN) == true) {
                //Connection Menu is hidden (closed) so open it
                lv_obj_clear_flag(connectionMenuArea, LV_OBJ_FLAG_HIDDEN);
            }
            else {
                //Connection Menu is not hidden (open) so close it
                lv_obj_add_flag(connectionMenuArea, LV_OBJ_FLAG_HIDDEN);
            }
        }
        else if(btn == connectionSerialButton) {
            //Ask for a Serial uROS connection
            uint8_t connectionType = 0x01;
            InterThreadMessageStruct uROSMsg = {.id = UROSThread_Connect, .data = (uint32_t*)connectionType, .length = 0 };
            rt_mq_send(&uROSMessageQueue, (void*)&uROSMsg, sizeof(InterThreadMessageStruct));

            lv_obj_add_flag(connectionMenuArea, LV_OBJ_FLAG_HIDDEN);
        }
        else if(btn == connectionEthernetButton) {
            //Ask for a Ethernet uROS connection
            uint8_t connectionType = 0x02;
            InterThreadMessageStruct uROSMsg = {.id = UROSThread_Connect, .data = (uint32_t*)connectionType, .length = 0 };
//            rt_mq_send(&uROSMessageQueue, (void*)&uROSMsg, sizeof(InterThreadMessageStruct));

            lv_obj_add_flag(connectionMenuArea, LV_OBJ_FLAG_HIDDEN);
        }
    }
}

/**
  * @brief	This function initializes this GUI part
  * @param	None
  * @return	None
  */
void GUITopBarInit() {
	//Set Top Info Bar
	//Top Bar Area
	topBarArea = lv_obj_create(lv_scr_act());
	lv_obj_remove_style_all(topBarArea);
	lv_obj_add_style(topBarArea, &mainStyle, LV_PART_MAIN);
	lv_obj_set_style_bg_color(topBarArea, STYLE_COLOR_DARK_GREY, LV_PART_MAIN);
	lv_obj_set_style_border_width(topBarArea, 0, LV_PART_MAIN);
	lv_obj_set_style_pad_all(topBarArea, 0, LV_PART_MAIN);
	lv_obj_set_size(topBarArea, 480, 25);
	lv_obj_align(topBarArea, LV_ALIGN_TOP_MID, 0, 0);

	//Project Name Sign Label
	projectNameLabel = lv_label_create(topBarArea);
	lv_obj_remove_style_all(projectNameLabel);
	lv_obj_add_style(projectNameLabel, &mainStyle, LV_PART_MAIN);
	lv_obj_set_style_bg_color(projectNameLabel, STYLE_COLOR_DARK_GREY, LV_PART_MAIN);
	lv_obj_set_style_border_width(projectNameLabel, 0, LV_PART_MAIN);
//	lv_obj_set_style_bg_color(projectNameLabel, STYLE_COLOR_GREY, LV_PART_MAIN);
	lv_obj_set_size(projectNameLabel, 120, 25);
//	lv_obj_set_pos(projectNameLabel, 0, 0);
	lv_obj_align(projectNameLabel, LV_ALIGN_LEFT_MID, 0, 0);
	lv_obj_set_style_text_align(projectNameLabel, LV_TEXT_ALIGN_CENTER, 0);
	lv_obj_set_style_text_color(projectNameLabel, STYLE_COLOR_GREEN, LV_PART_MAIN);
	lv_label_set_text(projectNameLabel, "uROS Terminal");

//	//Connection Indicator
//	connectionLabel = lv_label_create(topBarArea);
//	lv_obj_remove_style_all(connectionLabel);
//	lv_obj_add_style(connectionLabel, &mainStyle, LV_PART_MAIN);
//	lv_obj_set_style_bg_color(connectionLabel, STYLE_COLOR_DARK_GREY, LV_PART_MAIN);
//	lv_obj_set_style_border_width(connectionLabel, 0, LV_PART_MAIN);
//	lv_obj_set_size(connectionLabel, 240, 25);
//	lv_obj_align(connectionLabel, LV_ALIGN_CENTER, 0, 0);
//	lv_obj_set_style_text_align(connectionLabel, LV_TEXT_ALIGN_CENTER, 0);
//	lv_label_set_text(connectionLabel, "UNCONNECTED");
////	lv_label_set_text(connectionLabel, "COM 115200 8-N-1");
////	lv_label_set_text(connectionLabel, "ETH 192.168.1.1:8800");

    //Connection Button
    connectionButton = lv_btn_create(topBarArea);
    lv_obj_remove_style_all(connectionButton);
    lv_obj_add_style(connectionButton, &mainStyle, LV_PART_MAIN);
    lv_obj_set_style_bg_color(connectionButton, STYLE_COLOR_DARKER_GREY, LV_PART_MAIN);
//    lv_obj_set_style_border_color(connectionButton, STYLE_COLOR_GREY, LV_PART_MAIN);
    lv_obj_set_style_border_width(connectionButton, 0, LV_PART_MAIN);
    lv_obj_add_style(connectionButton, &mainStylePressed, LV_STATE_PRESSED);
    lv_obj_set_size(connectionButton, 240, 25);
    lv_obj_align(connectionButton, LV_ALIGN_CENTER, 0, 0);
    //Button Label
    connectionLabel = lv_label_create(connectionButton);
    lv_label_set_text(connectionLabel, "UNCONNECTED");
//    lv_obj_set_style_border_width(connectionLabel, 0, LV_PART_MAIN);
    lv_obj_center(connectionLabel);
    //Add event
    lv_obj_add_event_cb(connectionButton, GUITopBarButtonEvent, LV_EVENT_CLICKED, NULL);

    //Connection Menu Area
    connectionMenuArea = lv_obj_create(lv_scr_act());
    lv_obj_remove_style_all(connectionMenuArea);
    lv_obj_add_style(connectionMenuArea, &mainStyle, LV_PART_MAIN);
    lv_obj_set_style_bg_color(connectionMenuArea, lv_color_hex(0x101010), LV_PART_MAIN);
    lv_obj_set_style_border_width(connectionMenuArea, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(connectionMenuArea, 0, LV_PART_MAIN);
    lv_obj_set_size(connectionMenuArea, 240, 48);
    lv_obj_align(connectionMenuArea, LV_ALIGN_TOP_MID, 0, 25);
    lv_obj_add_flag(connectionMenuArea, LV_OBJ_FLAG_HIDDEN);

    //Connection Menu Serial Button
    connectionSerialButton = lv_btn_create(connectionMenuArea);
    lv_obj_remove_style_all(connectionSerialButton);
    lv_obj_add_style(connectionSerialButton, &mainStyle, LV_PART_MAIN);
    lv_obj_set_style_bg_color(connectionSerialButton, STYLE_COLOR_DARK_GREY, LV_PART_MAIN);
    lv_obj_add_style(connectionSerialButton, &mainStylePressed, LV_STATE_PRESSED);
    lv_obj_set_size(connectionSerialButton, 240, 25);
    lv_obj_align(connectionSerialButton, LV_ALIGN_TOP_MID, 0, 0);
    //Button Label
    lv_obj_t* label = lv_label_create(connectionSerialButton);
    lv_label_set_text(label, "SERIAL (UART4)");
    lv_obj_align(label, LV_ALIGN_LEFT_MID, 0, 0);
    //Add event
    lv_obj_add_event_cb(connectionSerialButton, GUITopBarButtonEvent, LV_EVENT_CLICKED, NULL);

    //Connection Menu Ethernet Button
    connectionEthernetButton = lv_btn_create(connectionMenuArea);
    lv_obj_remove_style_all(connectionEthernetButton);
    lv_obj_add_style(connectionEthernetButton, &mainStyle, LV_PART_MAIN);
    lv_obj_set_style_bg_color(connectionEthernetButton, STYLE_COLOR_DARK_GREY, LV_PART_MAIN);
    lv_obj_add_style(connectionEthernetButton, &mainStylePressed, LV_STATE_PRESSED);
    lv_obj_set_size(connectionEthernetButton, 240, 25);
    lv_obj_align(connectionEthernetButton, LV_ALIGN_TOP_MID, 0, 23);
    //Button Label
    label = lv_label_create(connectionEthernetButton);
    lv_label_set_text(label, "ETHERNET (ETH)");
    lv_obj_align(label, LV_ALIGN_LEFT_MID, 0, 0);
    //Add event
//    lv_obj_add_event_cb(connectionEthernetButton, GUITopBarButtonEvent, LV_EVENT_CLICKED, NULL);

	//Time
    timeLabel = lv_label_create(topBarArea);
    lv_obj_remove_style_all(timeLabel);
    lv_obj_add_style(timeLabel, &mainStyle, LV_PART_MAIN);
    lv_obj_set_style_bg_color(timeLabel, STYLE_COLOR_DARK_GREY, LV_PART_MAIN);
    lv_obj_set_style_border_width(timeLabel, 0, LV_PART_MAIN);
    lv_obj_set_size(timeLabel, 75, 25);
//  lv_obj_set_pos(timeLabel, 0, 0);
    lv_obj_align(timeLabel, LV_ALIGN_RIGHT_MID, 0, 0);
    lv_obj_set_style_text_align(timeLabel, LV_TEXT_ALIGN_CENTER, 0);
    lv_label_set_text(timeLabel, "12:00:00");
}
