#ifndef APPLICATIONS_GUI_GUIBOTTOMBAR_H_
#define APPLICATIONS_GUI_GUIBOTTOMBAR_H_

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

//GUI Parts
#include "guiInfoWindow.h"
#include "guiPublisherWindow.h"
#include "guiSubscriberWindow.h"
#include "guiTopicListWindow.h"

//Bottom bar GUI objects
extern lv_obj_t* bottomBarArea;
extern lv_obj_t* menuButton;
extern lv_obj_t* topicListButton;
extern lv_obj_t* publisherButton;
extern lv_obj_t* infoButton;

void GUIBottomBarInit();

#ifdef __cplusplus
}
#endif

#endif /* APPLICATIONS_GUI_GUIBOTTOMBAR_H_ */
