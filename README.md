# Micro-ROS (uROS) Terminal UI
This is the firmware for a simple Terminal UI interface for [Micro-ROS](https://micro.ros.org/), allowing to add a simple graphical UI to a [ROS 2](https://www.ros.org/) Robot over a Micro-ROS interface. The board it is running on is the new RT-Thread [HMI-Board](https://blog.lvgl.io/2023-06-14/ra6m3-hmi-board-review) developed by [RT-Thread](www.rt-thread.io) in colaboration with [Renesas](https://www.renesas.com/us/en) and [LVGL](https://lvgl.io/) using the high-performance [RA6M3](https://www.renesas.com/us/en/products/microcontrollers-microprocessors/ra-cortex-m-mcus/ra6m3-32-bit-microcontrollers-120mhz-usb-high-speed-ethernet-and-tft-controller) (Cortex-M4f running at 120 MHz, 2 MB flash and 640 kB RAM) chip from Renesas. This board was kindly provided by RT-Thread, LVGL and Renesas as part of the [Embedded GUI Contest](https://rt-thread.medium.com/2023-embedded-gui-contest-403648de53e4).

The following features are tested and working:
- Test

Features under development but not currently finished:
- Test

Features planned but not yet started:
- Test

## Demos
A short demo of the GUI Interface working and its current features can be found in the video bellow:

[![GUI Interface Demo](https://img.youtube.com/vi/tcLBblnkBhU/0.jpg)](https://youtu.be/tcLBblnkBhU "GUI Interface Demo")

## Hardware

### BOM
- 1x HMI-Board Development Board
- 1x USB-C cable
- 1x USB to UART Bridge (e.g. CP2102 module)

This Micro-ROS Terminal UI interface is being developed with the HMI-Board from RT-Thread. The HMI-Board is based on the high-performance RA6M3 MCU from Renesas with a Cortex-M4f running at 120 MHz and 2 MB of flash and 640 kB of RAM. The board comes with a 4.3" LCD Module (RK043FN66HS-CTG) with a resolution of 480x272 and capacitive touch as well as with a WiFi module and diverse connection options like CAN, Ethernet, USB as well as PMOD and Arduino Headers.

The current version only supports a Micro-ROS connection over serial, specifically over UART4 acceseble on the Arduino Header Jxx Pin 1 (RX) and 2 (TX).

![HMI-Board](./xxx.png)

## Software
The GUI Interface runs on the RT-Thread RTOS and uses the [LVGL](www.lvgl.io) embedded graphics library. The GUI Interface project was developed using the RT-Thread IDE, [RT-Thread Studio](www.rt-thread.io/studio.html), and the Micro-ROS interface is based on a [Micro-ROS RT-Thread package](https://github.com/wuhanstudio/micro_ros) with some modifications. The full project files and dependencies are included in this GitHub repository, including both the LVGL and Micro-ROS package (both with some modifications). To run the project it is therefore only necessary to install RT-Thread Studio, download this repository and import this project in RT-Thread Studio. 

To debug and flash the board from RT-Thread Studio follow the instruction bellow in the "Flashing and Debugging" Section.

### Adding Peripherals
#### Pre Requisites (Installation)
1) Install [RA FSP Smart Configurator](https://www.renesas.com/us/en/software-tool/ra-smart-configurator), available to download [here](https://github.com/renesas/fsp/releases) (use the IAR or Keil MDK release)
2) Click the "RA Smart Configurator" Link in the RT-Thread project
3) Select the install directory of the **RA Smart Configurator** (e.g. "C:\Renesas\RA\sc_v2023-07_fsp_v4.6.0")

#### Post Generation Changes/Configuration
Files that need chang after generation in **RA Smart Configurator** besides enabling newly added peripherals in RT-Thread (to get it to compile):

1) Move generated "memory_regions.ld" from "root" to "/script"
2) Delete the generated "src/hal_entry.c" file
3) Enable/add to build necessary HAL Drivers, both in folders "\rt-thread\components\drivers" and "\libraries\HAL_Drivers"
4) Set required defines in the "rtconfig.h" file
5) It can be necessary to also change the "startup.c" file located at "\ra\fsp\src\bsp\cmsis\Device\RENESAS\Source\startup.c". It should read the following in the Reset Handler function:

```c
void Reset_Handler (void) {
    /* Initialize system using BSP. */
    SystemInit();

    /* Call user application. */
#ifdef __ARMCC_VERSION
    main();
#elif defined(__GNUC__)
    extern int entry(void);
    entry();
#endif

    while (1) {
        /* Infinite Loop. */
    }
}
```
<!--
For RTC (still not working...):

Change "\libraries\HAL_Drivers\drv_common.c" function "void R_BSP_WarmStart (bsp_warm_start_event_t event)":
//https://community.renesas.com/mcu-mpu/ra/f/forum/18972/ra6m4-sub-clock-not-working---bug-fix
-->


### Flashing and Debugging
pyOCD setup:
1) Installed with: py -m pip install -U pyocd
2) Changed pyOCD location in RT-Thread: Debugger settings->Debugger->Executable path = "C:\Python311\Scripts\pyocd.exe"
3) To get path: import a_module
	print(a_module.__file__)
4) Installed support for HMI-Board: pyocd pack install R7FA6M3AH
5) Done!


### Architecture
The GUI Interface firmware is composed of three main modules, each responsible for a specific function and running as their own thread:

- GUI: Responsible to draw the GUI, react to GUI events and update the GUI
- Radio Interface: Responsible for interacting with the VUHFRadio module through a serial interface
- DSP: Responsible for signal processing of the I/Q signals received from the VUHFRadio module through a SAI interface in TDM mode

To manage communication between threads, each thread has its own message queue where other threads, and the ISR in some cases, publish necessary messages to. A standard message type is defined as the **InterThreadMessageStruct**: It holds an ID/OpCode, a pointer to data and a data length field. In cases where the data fits directly into the data pointer, aka data that fits in 4 bytes, it is passed by value and not through a pointer.

The communication from the VUHFRadio module to the GUI Interface are handled as follows: 
1. Bytes are read and processed by the serial ISR and a full packet is assembled
2. The full packet is published by reference from the serial ISR to the Radio Interface message queue
3. The Radio Interface Thread reads the message from the queue and decodes it
4. If changes to the GUI are required, the Radio Interface Thread publishes the required changes to the GUI Interface message queue

The communication in the reverse direction, from GUI Interface to VUHFRadio module, are handled basically in reveres:
1. If the GUI notices a change, an event, it publishes that change to the Radio Interface message queue
2. The Radio Interface thread then reads that message and writes the appropriate command message over the serial interface to the VUHFRadio module

This means that any updates to the GUI are performed ONLY by the GUI Thread, keeping the GUI thread safe as the LVGL library by itself is not. The updates to the GUI are performed before the LVGL task runs, this is done by modifying the **lvgl_thread_entry** function (packages\LVGL-v8.3.0\env_support\rt-thread\lv_rt_thread_port.c) and adding a external gui update handling function in the while loop:

```c
/* handle the tasks of LVGL */
while(1)
{
	lv_user_gui_update();							//User GUI Update Call
	lv_task_handler();								//LVGL GUI Thread Handler
	rt_thread_mdelay(LV_DISP_DEF_REFR_PERIOD);		//Refresh rate of the GUI
}
```

Another way to keep the GUI thread safe would be to wrap the **lv_task_handler** function and any LVGL (**lv_xxx**) function calls with a mutex, as described in the LVGL [documentation](https://docs.lvgl.io/master/porting/os.html).

To manage the GUI better it is split into different areas, with each area being separated into there own source and header files:

![GUI UI](./GUI_UI.png)

The main menu (guiMenu.c) is designed but it is not fully functional yet, it can be navigated and values can be changed but not all are sent to the VUHFRadio Module, for now. The menu style is shown in the figure bellow:

![GUI UI Menu](./GUI_UI_Menu.png)