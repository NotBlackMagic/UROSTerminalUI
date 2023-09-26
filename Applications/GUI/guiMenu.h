#ifndef APPLICATIONS_GUI_GUIMENU_H_
#define APPLICATIONS_GUI_GUIMENU_H_

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
#include "Applications/config.h"
#include "Applications/interThreadCommunication.h"
#include "styles.h"

//GUI Parts

//Menu GUI objects
extern lv_obj_t* menu;

void GUIMenuInit();

#ifdef __cplusplus
}
#endif

#endif /* APPLICATIONS_GUI_GUIMENU_H_ */
