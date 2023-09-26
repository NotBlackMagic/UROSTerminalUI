#ifndef APPLICATIONS_GUI_GUI_H_
#define APPLICATIONS_GUI_GUI_H_

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
#include "guiBottomBar.h"
#include "guiInfoWindow.h"
#include "guiSubscriberWindow.h"
#include "guiTopBar.h"

void GUIInit();
void GUIUpdate();

#ifdef __cplusplus
}
#endif

#endif /* APPLICATIONS_GUI_GUI_H_ */
