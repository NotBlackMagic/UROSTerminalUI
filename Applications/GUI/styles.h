#ifndef APPLICATIONS_GUI_STYLES_H_
#define APPLICATIONS_GUI_STYLES_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

#define STYLE_COLOR_WHITE				lv_color_hex(0xFFFFFF)
#define STYLE_COLOR_BLACK				lv_color_hex(0x000000)
#define STYLE_COLOR_DARKER_GREY         lv_color_hex(0x101010)
#define STYLE_COLOR_DARK_GREY			lv_color_hex(0x202020)
#define STYLE_COLOR_GREY				lv_color_hex(0x404040)
#define STYLE_COLOR_LIGHT_GREY			lv_color_hex(0x606060)
#define STYLE_COLOR_LIGHTER_GREY        lv_color_hex(0x808080)
#define STYLE_COLOR_RED					lv_color_hex(0xFF0000)
#define STYLE_COLOR_DARK_RED			lv_color_hex(0x600000)
#define STYLE_COLOR_GREEN				lv_color_hex(0x00FF00)
#define STYLE_COLOR_DARK_GREEN			lv_color_hex(0x006000)
#define STYLE_COLOR_BLUE				lv_color_hex(0x0000FF)
#define STYLE_COLOR_DARK_BLUE			lv_color_hex(0x000060)

lv_style_t mainStyle;
lv_style_t mainStylePressed;
lv_style_t mainStyleSelected;
lv_style_t mainStyleEnabled;
lv_style_t mainStyleDisabled;

void StylesInit();

#ifdef __cplusplus
}
#endif

#endif /* APPLICATIONS_GUI_STYLES_H_ */
