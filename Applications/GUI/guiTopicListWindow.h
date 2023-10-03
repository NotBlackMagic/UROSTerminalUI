#ifndef APPLICATIONS_GUI_GUITOPICLISTWINDOW_H_
#define APPLICATIONS_GUI_GUITOPICLISTWINDOW_H_

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

//Topic list window GUI objects
extern lv_obj_t* topicListWindowArea;

void GUITopicListWindowInit();
void GUITopicListWindowUpdate(rcl_names_and_types_t* topics);

#ifdef __cplusplus
}
#endif

#endif /* APPLICATIONS_GUI_GUITOPICLISTWINDOW_H_ */
