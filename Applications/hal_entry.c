#include <rtthread.h>
#include "hal_data.h"
#include <rtdevice.h>

#include "Applications/pinMapping.h"

static rt_device_t serial;
struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;

void hal_entry(void) {
    rt_err_t error;

	//Set LED0 pin mode to output
	//rt_pin_mode(GPIO_LED_USER_0, PIN_MODE_OUTPUT);

    rt_kprintf("\nHello NotBlackMagic!\n");

    //Test UART4
//    serial = rt_device_find("uart4");
//    error = rt_device_open(serial, RT_DEVICE_FLAG_INT_RX);
//    char str[] = "UART4 Test!\n";
//    rt_device_write(serial, 0, str, (sizeof(str) - 1));

	while (1) {
//        if(rt_pin_read(GPIO_BUTTON_USER_0) == PIN_LOW) {
//            rt_pin_write(GPIO_LED_USER_0, PIN_HIGH);
//        }
//        if(rt_pin_read(GPIO_BUTTON_USER_1) == PIN_LOW) {
//            rt_pin_write(GPIO_LED_USER_0, PIN_LOW);
//        }
//	    rt_thread_mdelay(1000);

		rt_pin_write(GPIO_LED_USER_0, PIN_HIGH);
		rt_thread_mdelay(1000);
		rt_pin_write(GPIO_LED_USER_0, PIN_LOW);
		rt_thread_mdelay(1000);
	}
}
