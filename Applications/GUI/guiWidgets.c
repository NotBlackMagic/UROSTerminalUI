#include "guiWidgets.h"

lv_obj_t* keyboard;

/**
  * @brief  This function initializes this GUI part
  * @param  None
  * @return None
  */
void GUIWidgetKeyboardInit() {
    //Create a keyboard
    keyboard = lv_keyboard_create(lv_scr_act());
    lv_obj_remove_style_all(keyboard);
    lv_obj_add_style(keyboard, &mainStyle, LV_PART_MAIN);
    lv_obj_add_style(keyboard, &mainStyle, LV_PART_ITEMS);
    lv_obj_add_style(keyboard, &mainStylePressed, LV_PART_ITEMS | LV_STATE_PRESSED);
    lv_obj_align(keyboard, LV_ALIGN_BOTTOM_MID, 0, 0);
//    lv_obj_set_style_pad_right(keyboard, 2, LV_PART_MAIN);
//    lv_obj_set_style_pad_bottom(keyboard, 2, LV_PART_MAIN);
    lv_obj_set_size(keyboard, LV_HOR_RES, LV_VER_RES / 2);
    lv_keyboard_set_mode(keyboard, LV_KEYBOARD_MODE_TEXT_LOWER);
    lv_keyboard_set_textarea(keyboard, NULL);
    lv_obj_add_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
}
