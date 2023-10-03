#ifndef APPLICATIONS_GUI_GUISUBSCRIBERWINDOW_H_
#define APPLICATIONS_GUI_GUISUBSCRIBERWINDOW_H_

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

//Subscriber window GUI objects
extern lv_obj_t* subscriberWindowArea;

//Topic messages to text
//Geometry Messages
void GUISubscriberContentAccel(const void *msgin);
void GUISubscriberContentPoint(const void *msgin);
void GUISubscriberContentPose(const void *msgin);
void GUISubscriberContentQuaternion(const void *msgin);
void GUISubscriberContentTransform(const void *msgin);
void GUISubscriberContentTwist(const void *msgin);
void GUISubscriberContentVector3(const void *msgin);
//Navigation Message
void GUISubscriberContentOdometry(const void *msgin);

void GUISubscriberWindowInit();
void GUISubscriberWindowUpdate(rcl_names_and_types_t* topics);

#ifdef __cplusplus
}
#endif

#endif /* APPLICATIONS_GUI_GUISUBSCRIBERWINDOW_H_ */
