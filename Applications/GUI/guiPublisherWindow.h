#ifndef APPLICATIONS_GUI_GUIPUBLISHERWINDOW_H_
#define APPLICATIONS_GUI_GUIPUBLISHERWINDOW_H_

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
#include "Applications/config.h"
#include "Applications/uROS/uros.h"

//GUI Parts
#include "Applications/GUI/guiWidgets.h"

//Publisher window GUI objects
extern lv_obj_t* publisherWindowArea;

void GUIPublisherWindowInit();

#ifdef __cplusplus
}
#endif

#endif /* APPLICATIONS_GUI_GUIPUBLISHERWINDOW_H_ */
