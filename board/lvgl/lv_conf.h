#ifndef LV_CONF_H
#define LV_CONF_H

#include <rtconfig.h>

/* Enable additional color format support */
#define DLG_LVGL_CF                 0

#define LV_USE_GPU_RA6M3_G2D        1
#define LV_GPU_RA6M3_G2D_INCLUDE    "hal_data.h"

#define LV_USE_PERF_MONITOR         1
#define LV_COLOR_DEPTH              16

#ifdef PKG_USING_ILI9341
    #define LV_HOR_RES_MAX          240
    #define LV_VER_RES_MAX          320
    #define LV_COLOR_16_SWAP        1
    #define LV_DPI_DEF              99
#else
    #define LV_HOR_RES_MAX          480
    #define LV_VER_RES_MAX          272
    #define LV_DPI_DEF              89
#endif

#define LV_ATTRIBUTE_MEM_ALIGN_SIZE 64

#define LV_FONT_UNSCII_8            1
#define LV_FONT_UNSCII_16           1
#define LV_FONT_MONTSERRAT_12       1
#define LV_FONT_MONTSERRAT_16       1
#define LV_FONT_MONTSERRAT_40       1
#define LV_COLOR_SCREEN_TRANSP      1

#endif
