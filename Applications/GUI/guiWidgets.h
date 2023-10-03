#ifndef APPLICATIONS_GUI_GUIWIDGETS_H_
#define APPLICATIONS_GUI_GUIWIDGETS_H_

#ifdef __cplusplus
extern "C" {
#endif

//General C libraries includes
#include "string.h"

//LVGL and RT-Thread include
#include "lvgl.h"
#include "rtdevice.h"

//RT-Thread Driver includes
#include "drv_gpio.h"

//Custom application includes
#include "styles.h"

//Helper Objects
extern lv_obj_t* keyboard;

void GUIWidgetKeyboardInit();

#ifdef __cplusplus
}
#endif

#endif /* APPLICATIONS_GUI_GUIWIDGETS_H_ */
