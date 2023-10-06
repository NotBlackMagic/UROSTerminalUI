#include "guiTopBar.h"

//Top bar GUI objects
lv_obj_t* topBarArea;
lv_obj_t* projectNameLabel;

//Time GUI Objects
lv_obj_t* timeButton;
lv_obj_t* timeLabel;
lv_obj_t* timeMenuArea;
lv_obj_t* timeMenuTimeLabel;
lv_obj_t* timeMenu10HUpButton;
lv_obj_t* timeMenu1HUpButton;
lv_obj_t* timeMenu10MUpButton;
lv_obj_t* timeMenu1MUpButton;
lv_obj_t* timeMenu10HDownButton;
lv_obj_t* timeMenu1HDownButton;
lv_obj_t* timeMenu10MDownButton;
lv_obj_t* timeMenu1MDownButton;
//Assistent variables
int currentTimeHour = 0;
int currentTimeMinutes = 0;

//Connection GUI Objects
lv_obj_t* connectionButton;
lv_obj_t* connectionLabel;
lv_obj_t* connectionMenuArea;
lv_obj_t* connectionSerialButton;
lv_obj_t* connectionEthernetButton;
lv_obj_t* connectionDisconnectButton;

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
                lv_obj_move_foreground(connectionMenuArea);
            }
            else {
                //Connection Menu is not hidden (open) so close it
                lv_obj_add_flag(connectionMenuArea, LV_OBJ_FLAG_HIDDEN);
            }
        }
        else if(btn == timeButton) {
        	if(lv_obj_has_flag(timeMenuArea, LV_OBJ_FLAG_HIDDEN) == true) {
				//Time Menu is hidden (closed) so open it
				lv_obj_clear_flag(timeMenuArea, LV_OBJ_FLAG_HIDDEN);
				lv_obj_move_foreground(timeMenuArea);

				//Set menu time label to current time
				time_t now = time(RT_NULL);
				struct tm tm = *localtime(&now);
				currentTimeHour = tm.tm_hour;
				currentTimeMinutes = tm.tm_min;

				char str[16];
				sprintf(str, "%02d:%02d", currentTimeHour, currentTimeMinutes);
				lv_label_set_text(timeMenuTimeLabel, str);
			}
			else {
				//Time Menu is not hidden (open) so close it
				lv_obj_add_flag(timeMenuArea, LV_OBJ_FLAG_HIDDEN);

				//Update time to currently set time
				set_time(currentTimeHour, currentTimeMinutes, 0);
			}
        }
    }
}

/**
  * @brief  This function handles button events of this GUI part
  * @param  lv_event_t: Pointer to the triggered event
  * @return None
  */
static void GUIConnectionMenuButtonEvent(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * btn = lv_event_get_target(e);
    if(code == LV_EVENT_CLICKED) {
        if(btn == connectionSerialButton) {
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
        else if(btn == connectionDisconnectButton) {
            //Ask to disconnect
            uint8_t connectionType = 0x00;
            InterThreadMessageStruct uROSMsg = {.id = UROSThread_Connect, .data = (uint32_t*)connectionType, .length = 0 };
//            rt_mq_send(&uROSMessageQueue, (void*)&uROSMsg, sizeof(InterThreadMessageStruct));

            lv_obj_add_flag(connectionMenuArea, LV_OBJ_FLAG_HIDDEN);
        }
    }
}

/**
  * @brief  This function handles button events of this GUI part
  * @param  lv_event_t: Pointer to the triggered event
  * @return None
  */
static void GUITimeMenuCloseButtonEvent(lv_event_t * e) {
    lv_obj_add_flag(timeMenuArea, LV_OBJ_FLAG_HIDDEN);

    //Update time to currently set time
	set_time(currentTimeHour, currentTimeMinutes, 0);
}

/**
  * @brief  This function handles button events of this GUI part
  * @param  lv_event_t: Pointer to the triggered event
  * @return None
  */
static void GUITimeMenuButtonEvent(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * btn = lv_event_get_target(e);
    if(code == LV_EVENT_CLICKED) {
    	if(btn == timeMenu10HUpButton) {
    		currentTimeHour += 10;
    		if(currentTimeHour >= 30) {
                currentTimeHour = currentTimeHour - 30;
            }
            else if(currentTimeHour >= 24) {
                currentTimeHour = currentTimeHour - 20;
            }
    	}
    	else if(btn == timeMenu1HUpButton) {
    		currentTimeHour += 1;
    		if(currentTimeHour >= 24) {
    			currentTimeHour = 0;
    		}
    	}
    	else if(btn == timeMenu10MUpButton) {
			currentTimeMinutes += 10;
			if(currentTimeMinutes >= 60) {
				currentTimeMinutes = currentTimeMinutes - 60;
			}
		}
    	else if(btn == timeMenu1MUpButton) {
    		currentTimeMinutes += 1;
			if(currentTimeMinutes >= 60) {
				currentTimeMinutes = 0;
			}
		}
    	else if(btn == timeMenu10HDownButton) {
    		currentTimeHour -= 10;
    		if(currentTimeHour <= -7) {
    			currentTimeHour = currentTimeHour + 30;
    		}
    		else if(currentTimeHour < 0) {
    		    currentTimeHour = currentTimeHour + 20;
    		}
    	}
    	else if(btn == timeMenu1HDownButton) {
    		currentTimeHour -= 1;
    		if(currentTimeHour < 0) {
    			currentTimeHour = 23;
    		}
    	}
    	else if(btn == timeMenu10MDownButton) {
    		currentTimeMinutes -= 10;
    		if(currentTimeMinutes < 0) {
    			currentTimeMinutes = currentTimeMinutes + 60;
    		}
    	}
    	else if(btn == timeMenu1MDownButton) {
    		currentTimeMinutes -= 1;
    		if(currentTimeMinutes < 0) {
    			currentTimeMinutes = 59;
    		}
    	}
    	//Update Time Menu Time Label
    	char str[16];
		sprintf(str, "%02d:%02d", currentTimeHour, currentTimeMinutes);
		lv_label_set_text(timeMenuTimeLabel, str);
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

	//--------------------------------------------------------------------------//
	//                       	Connection GUI									//
	//--------------------------------------------------------------------------//
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
    lv_obj_set_style_bg_color(connectionButton, STYLE_COLOR_DARK_GREY, LV_PART_MAIN);
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
    lv_obj_set_style_bg_color(connectionMenuArea, STYLE_COLOR_DARKER_GREY, LV_PART_MAIN);
    lv_obj_set_style_border_width(connectionMenuArea, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(connectionMenuArea, 0, LV_PART_MAIN);
    lv_obj_set_size(connectionMenuArea, 240, 71);
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
    lv_obj_add_event_cb(connectionSerialButton, GUIConnectionMenuButtonEvent, LV_EVENT_CLICKED, NULL);

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
    lv_obj_add_event_cb(connectionEthernetButton, GUIConnectionMenuButtonEvent, LV_EVENT_CLICKED, NULL);

    connectionDisconnectButton = lv_btn_create(connectionMenuArea);
    lv_obj_remove_style_all(connectionDisconnectButton);
    lv_obj_add_style(connectionDisconnectButton, &mainStyle, LV_PART_MAIN);
    lv_obj_set_style_bg_color(connectionDisconnectButton, STYLE_COLOR_DARK_GREY, LV_PART_MAIN);
    lv_obj_add_style(connectionDisconnectButton, &mainStylePressed, LV_STATE_PRESSED);
    lv_obj_set_size(connectionDisconnectButton, 240, 25);
    lv_obj_align(connectionDisconnectButton, LV_ALIGN_TOP_MID, 0, 46);
    //Button Label
    label = lv_label_create(connectionDisconnectButton);
    lv_label_set_text(label, "DISCONNECT");
    lv_obj_align(label, LV_ALIGN_LEFT_MID, 0, 0);
    //Add event
    lv_obj_add_event_cb(connectionDisconnectButton, GUIConnectionMenuButtonEvent, LV_EVENT_CLICKED, NULL);

    //--------------------------------------------------------------------------//
	//                       		Time GUI									//
	//--------------------------------------------------------------------------//
//	//Time Indicator
//    timeLabel = lv_label_create(topBarArea);
//    lv_obj_remove_style_all(timeLabel);
//    lv_obj_add_style(timeLabel, &mainStyle, LV_PART_MAIN);
//    lv_obj_set_style_bg_color(timeLabel, STYLE_COLOR_DARK_GREY, LV_PART_MAIN);
//    lv_obj_set_style_border_width(timeLabel, 0, LV_PART_MAIN);
//    lv_obj_set_size(timeLabel, 75, 25);
////  lv_obj_set_pos(timeLabel, 0, 0);
//    lv_obj_align(timeLabel, LV_ALIGN_RIGHT_MID, 0, 0);
//    lv_obj_set_style_text_align(timeLabel, LV_TEXT_ALIGN_CENTER, 0);
//    lv_label_set_text(timeLabel, "12:00:00");

    //Time Button
    timeButton = lv_btn_create(topBarArea);
    lv_obj_remove_style_all(timeButton);
    lv_obj_add_style(timeButton, &mainStyle, LV_PART_MAIN);
    lv_obj_set_style_bg_color(timeButton, STYLE_COLOR_DARK_GREY, LV_PART_MAIN);
//    lv_obj_set_style_border_color(timeButton, STYLE_COLOR_GREY, LV_PART_MAIN);
    lv_obj_set_style_border_width(timeButton, 0, LV_PART_MAIN);
    lv_obj_add_style(timeButton, &mainStylePressed, LV_STATE_PRESSED);
    lv_obj_set_size(timeButton, 75, 25);
    lv_obj_align(timeButton, LV_ALIGN_TOP_RIGHT, 0, 0);
    //Button Label
    timeLabel = lv_label_create(timeButton);
    lv_label_set_text(timeLabel, "12:00:00");
//    lv_obj_set_style_border_width(timeLabel, 0, LV_PART_MAIN);
    lv_obj_center(timeLabel);
    //Add event
    lv_obj_add_event_cb(timeButton, GUITopBarButtonEvent, LV_EVENT_CLICKED, NULL);

    //Connection Menu Area
    timeMenuArea = lv_win_create(lv_scr_act(), 30);
    lv_win_add_title(timeMenuArea, "Set Time");
	lv_obj_set_style_bg_color(timeMenuArea, STYLE_COLOR_BLACK, LV_PART_MAIN);
	lv_obj_set_style_pad_all(timeMenuArea, 0, LV_PART_MAIN);
	lv_obj_set_style_border_width(timeMenuArea, 2, LV_PART_MAIN);
	lv_obj_set_style_border_color(timeMenuArea, STYLE_COLOR_GREY, LV_PART_MAIN);
	lv_obj_set_style_outline_width(timeMenuArea, 3, LV_PART_MAIN);
	lv_obj_set_style_outline_color(timeMenuArea, STYLE_COLOR_DARK_GREEN, LV_PART_MAIN);
	lv_obj_set_size(timeMenuArea, 152, 130);
	lv_obj_align(timeMenuArea, LV_ALIGN_CENTER, 0, 0);
	lv_obj_add_flag(timeMenuArea, LV_OBJ_FLAG_HIDDEN);

	lv_obj_t* btn = lv_win_add_btn(timeMenuArea, LV_SYMBOL_CLOSE, 60);
	lv_obj_remove_style_all(btn);
	lv_obj_add_style(btn, &mainStyle, 0);
	lv_obj_add_style(btn, &mainStylePressed, LV_STATE_PRESSED);
	lv_obj_set_width(btn, 25);
	lv_obj_add_event_cb(btn, GUITimeMenuCloseButtonEvent, LV_EVENT_CLICKED, NULL);

	lv_obj_t * head = lv_win_get_header(timeMenuArea);
	lv_obj_set_style_bg_color(head, STYLE_COLOR_GREY, LV_PART_MAIN);
	lv_obj_set_style_text_color(head, STYLE_COLOR_WHITE, LV_PART_MAIN);

	lv_obj_t * cont = lv_win_get_content(timeMenuArea);
	lv_obj_set_style_bg_color(cont, STYLE_COLOR_BLACK, LV_PART_MAIN);
	lv_obj_set_style_pad_all(cont, 4, LV_PART_MAIN);

	//Time Menu Time label (shows current set time)
	timeMenuTimeLabel = lv_label_create(cont);
    lv_obj_remove_style_all(timeMenuTimeLabel);
    lv_obj_add_style(timeMenuTimeLabel, &mainStyle, LV_PART_MAIN);
    lv_obj_set_style_bg_color(timeMenuTimeLabel, STYLE_COLOR_BLACK, LV_PART_MAIN);
    lv_obj_set_style_border_width(timeMenuTimeLabel, 0, LV_PART_MAIN);
    lv_obj_set_width(timeMenuTimeLabel, 130);
    lv_obj_align(timeMenuTimeLabel, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_text_align(timeMenuTimeLabel, LV_TEXT_ALIGN_CENTER, 0);
    lv_label_set_text(timeMenuTimeLabel, "12:00");
    lv_obj_set_style_text_font(timeMenuTimeLabel, &lv_font_montserrat_40, LV_PART_MAIN);

    //10's Hour UP Button
    timeMenu10HUpButton = lv_btn_create(cont);
    lv_obj_remove_style_all(timeMenu10HUpButton);
    lv_obj_add_style(timeMenu10HUpButton, &mainStyle, LV_PART_MAIN);
    lv_obj_set_style_bg_color(timeMenu10HUpButton, STYLE_COLOR_DARK_GREY, LV_PART_MAIN);
    lv_obj_add_style(timeMenu10HUpButton, &mainStylePressed, LV_STATE_PRESSED);
    lv_obj_set_size(timeMenu10HUpButton, 30, 25);
    lv_obj_align(timeMenu10HUpButton, LV_ALIGN_TOP_LEFT, 0, 0);
    //Button Label
    label = lv_label_create(timeMenu10HUpButton);
//    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
	lv_label_set_text(label, LV_SYMBOL_UP);
    //Add event
    lv_obj_add_event_cb(timeMenu10HUpButton, GUITimeMenuButtonEvent, LV_EVENT_CLICKED, NULL);

    //1's Hour UP Button
    timeMenu1HUpButton = lv_btn_create(cont);
	lv_obj_remove_style_all(timeMenu1HUpButton);
	lv_obj_add_style(timeMenu1HUpButton, &mainStyle, LV_PART_MAIN);
	lv_obj_set_style_bg_color(timeMenu1HUpButton, STYLE_COLOR_DARK_GREY, LV_PART_MAIN);
	lv_obj_add_style(timeMenu1HUpButton, &mainStylePressed, LV_STATE_PRESSED);
	lv_obj_set_size(timeMenu1HUpButton, 30, 25);
	lv_obj_align(timeMenu1HUpButton, LV_ALIGN_TOP_LEFT, 35, 0);
	//Button Label
	label = lv_label_create(timeMenu1HUpButton);
	lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
	lv_label_set_text(label, LV_SYMBOL_UP);
	//Add event
	lv_obj_add_event_cb(timeMenu1HUpButton, GUITimeMenuButtonEvent, LV_EVENT_CLICKED, NULL);

	//10's Minute UP Button
	timeMenu10MUpButton = lv_btn_create(cont);
	lv_obj_remove_style_all(timeMenu10MUpButton);
	lv_obj_add_style(timeMenu10MUpButton, &mainStyle, LV_PART_MAIN);
	lv_obj_set_style_bg_color(timeMenu10MUpButton, STYLE_COLOR_DARK_GREY, LV_PART_MAIN);
	lv_obj_add_style(timeMenu10MUpButton, &mainStylePressed, LV_STATE_PRESSED);
	lv_obj_set_size(timeMenu10MUpButton, 30, 25);
	lv_obj_align(timeMenu10MUpButton, LV_ALIGN_TOP_LEFT, 75, 0);
	//Button Label
	label = lv_label_create(timeMenu10MUpButton);
	lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
	lv_label_set_text(label, LV_SYMBOL_UP);
	//Add event
	lv_obj_add_event_cb(timeMenu10MUpButton, GUITimeMenuButtonEvent, LV_EVENT_CLICKED, NULL);

	//1's Minute UP Button
	timeMenu1MUpButton = lv_btn_create(cont);
	lv_obj_remove_style_all(timeMenu1MUpButton);
	lv_obj_add_style(timeMenu1MUpButton, &mainStyle, LV_PART_MAIN);
	lv_obj_set_style_bg_color(timeMenu1MUpButton, STYLE_COLOR_DARK_GREY, LV_PART_MAIN);
	lv_obj_add_style(timeMenu1MUpButton, &mainStylePressed, LV_STATE_PRESSED);
	lv_obj_set_size(timeMenu1MUpButton, 30, 25);
	lv_obj_align(timeMenu1MUpButton, LV_ALIGN_TOP_LEFT, 110, 0);
	//Button Label
	label = lv_label_create(timeMenu1MUpButton);
	lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
	lv_label_set_text(label, LV_SYMBOL_UP);
	//Add event
	lv_obj_add_event_cb(timeMenu1MUpButton, GUITimeMenuButtonEvent, LV_EVENT_CLICKED, NULL);

	//10's Hour DOWN Button
	timeMenu10HDownButton = lv_btn_create(cont);
	lv_obj_remove_style_all(timeMenu10HDownButton);
	lv_obj_add_style(timeMenu10HDownButton, &mainStyle, LV_PART_MAIN);
	lv_obj_set_style_bg_color(timeMenu10HDownButton, STYLE_COLOR_DARK_GREY, LV_PART_MAIN);
	lv_obj_add_style(timeMenu10HDownButton, &mainStylePressed, LV_STATE_PRESSED);
	lv_obj_set_size(timeMenu10HDownButton, 30, 25);
	lv_obj_align(timeMenu10HDownButton, LV_ALIGN_BOTTOM_LEFT, 0, 0);
	//Button Label
	label = lv_label_create(timeMenu10HDownButton);
	lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
	lv_label_set_text(label, LV_SYMBOL_DOWN);
	//Add event
	lv_obj_add_event_cb(timeMenu10HDownButton, GUITimeMenuButtonEvent, LV_EVENT_CLICKED, NULL);

	//1's Hour DOWN Button
	timeMenu1HDownButton = lv_btn_create(cont);
	lv_obj_remove_style_all(timeMenu1HDownButton);
	lv_obj_add_style(timeMenu1HDownButton, &mainStyle, LV_PART_MAIN);
	lv_obj_set_style_bg_color(timeMenu1HDownButton, STYLE_COLOR_DARK_GREY, LV_PART_MAIN);
	lv_obj_add_style(timeMenu1HDownButton, &mainStylePressed, LV_STATE_PRESSED);
	lv_obj_set_size(timeMenu1HDownButton, 30, 25);
	lv_obj_align(timeMenu1HDownButton, LV_ALIGN_BOTTOM_LEFT, 35, 0);
	//Button Label
	label = lv_label_create(timeMenu1HDownButton);
	lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
	lv_label_set_text(label, LV_SYMBOL_DOWN);
	//Add event
	lv_obj_add_event_cb(timeMenu1HDownButton, GUITimeMenuButtonEvent, LV_EVENT_CLICKED, NULL);

	//10's Minute DOWN Button
	timeMenu10MDownButton = lv_btn_create(cont);
	lv_obj_remove_style_all(timeMenu10MDownButton);
	lv_obj_add_style(timeMenu10MDownButton, &mainStyle, LV_PART_MAIN);
	lv_obj_set_style_bg_color(timeMenu10MDownButton, STYLE_COLOR_DARK_GREY, LV_PART_MAIN);
	lv_obj_add_style(timeMenu10MDownButton, &mainStylePressed, LV_STATE_PRESSED);
	lv_obj_set_size(timeMenu10MDownButton, 30, 25);
	lv_obj_align(timeMenu10MDownButton, LV_ALIGN_BOTTOM_LEFT, 75, 0);
	//Button Label
	label = lv_label_create(timeMenu10MDownButton);
	lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
	lv_label_set_text(label, LV_SYMBOL_DOWN);
	//Add event
	lv_obj_add_event_cb(timeMenu10MDownButton, GUITimeMenuButtonEvent, LV_EVENT_CLICKED, NULL);

	//1's Minute DOWN Button
	timeMenu1MDownButton = lv_btn_create(cont);
	lv_obj_remove_style_all(timeMenu1MDownButton);
	lv_obj_add_style(timeMenu1MDownButton, &mainStyle, LV_PART_MAIN);
	lv_obj_set_style_bg_color(timeMenu1MDownButton, STYLE_COLOR_DARK_GREY, LV_PART_MAIN);
	lv_obj_add_style(timeMenu1MDownButton, &mainStylePressed, LV_STATE_PRESSED);
	lv_obj_set_size(timeMenu1MDownButton, 30, 25);
	lv_obj_align(timeMenu1MDownButton, LV_ALIGN_BOTTOM_LEFT, 110, 0);
	//Button Label
	label = lv_label_create(timeMenu1MDownButton);
	lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
	lv_label_set_text(label, LV_SYMBOL_DOWN);
	//Add event
	lv_obj_add_event_cb(timeMenu1MDownButton, GUITimeMenuButtonEvent, LV_EVENT_CLICKED, NULL);
}
