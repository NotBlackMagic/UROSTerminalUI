#include "guiBottomBar.h"

//Bottom bar GUI objects
lv_obj_t* bottomBarArea;
lv_obj_t* menuButton;
lv_obj_t* topicListButton;
lv_obj_t* subscriberButton;
lv_obj_t* publisherButton;
lv_obj_t* infoButton;

/**
  * @brief	This function handles button events of this GUI part
  * @param	lv_event_t: Pointer to the triggered event
  * @return	None
  */
static void GUIBottomBarButtonEvent(lv_event_t * e) {
	lv_event_code_t code = lv_event_get_code(e);
	lv_obj_t * btn = lv_event_get_target(e);
	if(code == LV_EVENT_CLICKED) {
	    if(btn == menuButton) {

	    }
	    else if(btn == topicListButton) {
	        //Close all other windows
//	        lv_obj_add_flag(topicListWindowArea, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(subscriberWindowArea, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(publisherWindowArea, LV_OBJ_FLAG_HIDDEN);

			if(lv_obj_has_flag(topicListWindowArea, LV_OBJ_FLAG_HIDDEN) == true) {
				//Window is hidden (closed) so open it
				lv_obj_clear_flag(topicListWindowArea, LV_OBJ_FLAG_HIDDEN);

				//Send/request new Topic list
                InterThreadMessageStruct uROSMsg = {.id = UROSThread_List_Topics, .data = (uint32_t*)0x00, .length = 0 };
                rt_mq_send(&uROSMessageQueue, (void*)&uROSMsg, sizeof(InterThreadMessageStruct));
			}
			else {
				//Window is not hidden (open) so close it
				lv_obj_add_flag(topicListWindowArea, LV_OBJ_FLAG_HIDDEN);
			}
		}
	    else if(btn == subscriberButton) {
	        //Close all other windows
	        lv_obj_add_flag(topicListWindowArea, LV_OBJ_FLAG_HIDDEN);
//            lv_obj_add_flag(subscriberWindowArea, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(publisherWindowArea, LV_OBJ_FLAG_HIDDEN);

	        if(lv_obj_has_flag(subscriberWindowArea, LV_OBJ_FLAG_HIDDEN) == true) {
                //Window is hidden (closed) so open it
                lv_obj_clear_flag(subscriberWindowArea, LV_OBJ_FLAG_HIDDEN);
            }
            else {
                //Window is not hidden (open) so close it
                lv_obj_add_flag(subscriberWindowArea, LV_OBJ_FLAG_HIDDEN);
            }
	    }
		else if(btn == publisherButton) {
            //Close all other windows
		    lv_obj_add_flag(topicListWindowArea, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(subscriberWindowArea, LV_OBJ_FLAG_HIDDEN);
//            lv_obj_add_flag(publisherWindowArea, LV_OBJ_FLAG_HIDDEN);

          if(lv_obj_has_flag(publisherWindowArea, LV_OBJ_FLAG_HIDDEN) == true) {
              //Window is hidden (closed) so open it
              lv_obj_clear_flag(publisherWindowArea, LV_OBJ_FLAG_HIDDEN);
          }
          else {
              //Window is not hidden (open) so close it
              lv_obj_add_flag(publisherWindowArea, LV_OBJ_FLAG_HIDDEN);
          }
		}
		else if(btn == infoButton) {
			GUIInfoWindowUpdate();

            if(lv_obj_has_flag(infoWindow, LV_OBJ_FLAG_HIDDEN) == true) {
                //Window is hidden (closed) so open it
                lv_obj_clear_flag(infoWindow, LV_OBJ_FLAG_HIDDEN);
            }
            else {
                //Window is not hidden (open) so close it
                lv_obj_add_flag(infoWindow, LV_OBJ_FLAG_HIDDEN);
            }
		}
	}
}

/**
  * @brief	This function initializes this GUI part
  * @param	None
  * @return	None
  */
void GUIBottomBarInit() {
	//Set Bottom Menu Bar
	//Bottom Bar Area
	bottomBarArea = lv_obj_create(lv_scr_act());	//lv_btn_create(lv_scr_act());
	lv_obj_remove_style_all(bottomBarArea);
	lv_obj_add_style(bottomBarArea, &mainStyle, LV_PART_MAIN);
	lv_obj_set_style_border_width(bottomBarArea, 0, LV_PART_MAIN);
	lv_obj_set_style_pad_all(bottomBarArea, 0, LV_PART_MAIN);
	lv_obj_set_size(bottomBarArea, 480, 25);
	lv_obj_align(bottomBarArea, LV_ALIGN_BOTTOM_MID, 0, 0);

//    //Menu Button
//    menuButton = lv_btn_create(bottomBarArea);
//    lv_obj_remove_style_all(menuButton);
//    lv_obj_add_style(menuButton, &mainStyle, LV_PART_MAIN);
//    lv_obj_set_style_bg_color(menuButton, STYLE_COLOR_LIGHT_GREY, LV_PART_MAIN);
//    lv_obj_add_style(menuButton, &mainStylePressed, LV_STATE_PRESSED);
//    lv_obj_set_size(menuButton, 107, 25);
//    lv_obj_align(menuButton, LV_ALIGN_LEFT_MID, 0, 0);
//    //Button Label
//    lv_obj_t* label = lv_label_create(menuButton);
//    lv_label_set_text(label, "MENU");
//    lv_obj_set_style_border_width(label, 0, LV_PART_MAIN);
//    lv_obj_center(label);
//    //Add event
//    lv_obj_add_event_cb(menuButton, GUIBottomBarButtonEvent, LV_EVENT_CLICKED, NULL);

	//Topic List Button
	topicListButton = lv_btn_create(bottomBarArea);
    lv_obj_remove_style_all(topicListButton);
    lv_obj_add_style(topicListButton, &mainStyle, LV_PART_MAIN);
    lv_obj_set_style_bg_color(topicListButton, STYLE_COLOR_LIGHT_GREY, LV_PART_MAIN);
    lv_obj_add_style(topicListButton, &mainStylePressed, LV_STATE_PRESSED);
    lv_obj_set_size(topicListButton, 142, 25);
    lv_obj_align(topicListButton, LV_ALIGN_LEFT_MID, 0, 0);
    //Button Label
    lv_obj_t* label = lv_label_create(topicListButton);
    lv_label_set_text(label, "LIST TOPICS");
    lv_obj_set_style_border_width(label, 0, LV_PART_MAIN);
    lv_obj_center(label);
    //Add event
    lv_obj_add_event_cb(topicListButton, GUIBottomBarButtonEvent, LV_EVENT_CLICKED, NULL);

    //Subscriber Button
    subscriberButton = lv_btn_create(bottomBarArea);
    lv_obj_remove_style_all(subscriberButton);
    lv_obj_add_style(subscriberButton, &mainStyle, LV_PART_MAIN);
    lv_obj_set_style_bg_color(subscriberButton, STYLE_COLOR_LIGHT_GREY, LV_PART_MAIN);
    lv_obj_add_style(subscriberButton, &mainStylePressed, LV_STATE_PRESSED);
    lv_obj_set_size(subscriberButton, 142, 25);
    lv_obj_align(subscriberButton, LV_ALIGN_LEFT_MID, 140, 0);
    //Button Label
    label = lv_label_create(subscriberButton);
    lv_label_set_text(label, "SUBSCRIBER");
    lv_obj_set_style_border_width(label, 0, LV_PART_MAIN);
    lv_obj_center(label);
    //Add event
    lv_obj_add_event_cb(subscriberButton, GUIBottomBarButtonEvent, LV_EVENT_CLICKED, NULL);

	//Publisher Button
    publisherButton = lv_btn_create(bottomBarArea);
    lv_obj_remove_style_all(publisherButton);
    lv_obj_add_style(publisherButton, &mainStyle, LV_PART_MAIN);
    lv_obj_set_style_bg_color(publisherButton, STYLE_COLOR_LIGHT_GREY, LV_PART_MAIN);
    lv_obj_add_style(publisherButton, &mainStylePressed, LV_STATE_PRESSED);
    lv_obj_set_size(publisherButton, 142, 25);
    lv_obj_align(publisherButton, LV_ALIGN_LEFT_MID, 280, 0);
    //Button Label
    label = lv_label_create(publisherButton);
    lv_label_set_text(label, "PUBLISHER");
    lv_obj_set_style_border_width(label, 0, LV_PART_MAIN);
    lv_obj_center(label);
    //Add event
    lv_obj_add_event_cb(publisherButton, GUIBottomBarButtonEvent, LV_EVENT_CLICKED, NULL);

	//Info Button
	infoButton = lv_btn_create(bottomBarArea);
	lv_obj_remove_style_all(infoButton);
	lv_obj_add_style(infoButton, &mainStyle, LV_PART_MAIN);
	lv_obj_set_style_bg_color(infoButton, STYLE_COLOR_LIGHT_GREY, LV_PART_MAIN);
	lv_obj_add_style(infoButton, &mainStylePressed, LV_STATE_PRESSED);
	lv_obj_set_size(infoButton, 60, 25);
	lv_obj_align(infoButton, LV_ALIGN_LEFT_MID, 420, 0);
	//Button Label
	label = lv_label_create(infoButton);
	lv_label_set_text(label, "INFO");
	lv_obj_set_style_border_width(label, 0, LV_PART_MAIN);
	lv_obj_center(label);
	//Add event
	lv_obj_add_event_cb(infoButton, GUIBottomBarButtonEvent, LV_EVENT_CLICKED, NULL);
}
