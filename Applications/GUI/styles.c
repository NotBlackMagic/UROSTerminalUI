#include "styles.h"

extern lv_style_t mainStyle;
extern lv_style_t lineStyle;
extern lv_style_t buttonStylePressed;

/**
  * @brief	This function initializes the custom GUI styles variables
  * @param	None
  * @return	None
  */
void StylesInit() {
	//------------------------------//
	//		Init Main Styles		//
	//------------------------------//

	//Main Style for DEFAULT state
	lv_style_init(&mainStyle);
	//Main Area Style
	lv_style_set_radius(&mainStyle, 0);
	lv_style_set_bg_opa(&mainStyle, LV_OPA_100);
	lv_style_set_bg_color(&mainStyle, STYLE_COLOR_BLACK);
	//Border Style
	lv_style_set_border_opa(&mainStyle, LV_OPA_100);
	lv_style_set_border_width(&mainStyle, 2);
	lv_style_set_border_color(&mainStyle, STYLE_COLOR_GREY);
	//Line Style
	lv_style_set_line_width(&mainStyle, 2);
	lv_style_set_line_color(&mainStyle, STYLE_COLOR_WHITE);
	//Text Style
	lv_style_set_text_opa(&mainStyle, LV_OPA_100);
	lv_style_set_text_color(&mainStyle, STYLE_COLOR_WHITE);
	lv_style_set_text_font(&mainStyle, &lv_font_montserrat_14);
	//Set padding
	lv_style_set_pad_top(&mainStyle, 5);
	lv_style_set_pad_right(&mainStyle, 5);
	lv_style_set_pad_bottom(&mainStyle, 5);
	lv_style_set_pad_left(&mainStyle, 5);
	lv_style_set_pad_column(&mainStyle, 3);
	lv_style_set_pad_row(&mainStyle, 3);

	//Style for PRESSED state
	lv_style_init(&mainStylePressed);
	lv_style_set_bg_opa(&mainStylePressed, LV_OPA_100);
	lv_style_set_bg_color(&mainStylePressed, STYLE_COLOR_DARK_GREY);

	//Style for SELECTED state
	lv_style_init(&mainStyleSelected);
	lv_style_set_bg_opa(&mainStyleSelected, LV_OPA_100);
	lv_style_set_bg_color(&mainStyleSelected, STYLE_COLOR_GREY);
	lv_style_set_text_color(&mainStyleSelected, STYLE_COLOR_WHITE);

	//Style for ON/ENABLED state
	lv_style_init(&mainStyleEnabled);
//	lv_style_set_bg_opa(&mainStyleEnabled, LV_OPA_100);
//	lv_style_set_bg_color(&mainStyleEnabled, lv_color_hex(0x003000));
	lv_style_set_border_color(&mainStyleEnabled, STYLE_COLOR_DARK_GREEN);
	lv_style_set_text_color(&mainStyleEnabled, STYLE_COLOR_GREEN);

	//Style for OFF/DISABLED state
	lv_style_init(&mainStyleDisabled);
//	lv_style_set_bg_opa(&mainStyleDisabled, LV_OPA_100);
//	lv_style_set_bg_color(&mainStyleDisabled, lv_color_hex(0x300000));
	lv_style_set_border_color(&mainStyleDisabled, STYLE_COLOR_DARK_RED);
	lv_style_set_text_color(&mainStyleDisabled, STYLE_COLOR_RED);
}
