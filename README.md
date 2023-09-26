# GUI Interface for the VUHFRadio: 2nd Generation
This is the firmware for the 2nd Generation GUI Interface for the [VUHFRadio](www.notblackmagic.com/projects/vuhf-radio/) Module, with a costume M.2 version also developed, using the [MIMXRT1060-EVKB](www.nxp.com/design/development-boards/i-mx-evaluation-and-development-boards/i-mx-rt1060-evaluation-kit:MIMXRT1060-EVKB) development board and touchscreen from NXP. This kit was kindly provided by [NXP](www.nxp.com) and [RT-Thread](www.rt-thread.io) as part of the [Embedded GUI Contest](https://rt-thread.medium.com/embedded-gui-contest-4c22ba04ca37).

The following features are tested and working:
- Loading of configurations and board information on startup from the VUHFRadio Module
- Update and display RSSI and RF Frequency tracking variables
- Working menus for changing main analog domain and digital domain configurations and send the changes to the VUHFRadio module
- Decode and display AX.25 frames received from the VUHFRadio Module over the KISS protocol
- Spectrum of the receiver signal using the digital DSP interface of the AX5043 and the SAI peripheral of the RT1060 MCU
- Costume version of the VUHFRadio module, a M.2 version fully compatible with the M.2 spec and connected to the M.2 connector of the development board

Features under development but not currently finished:
- Advanced settings menu for more analog and digital domain configurations for the VUHFRadio module

Features planned but not yet started:
- A simple message form/window to input AX.25 messages and send them
- A terminal style window for viewing and sending CAT messages from/to the module

## Demos
A short demo of the GUI Interface working and its current features can be found in the video bellow:

[![GUI Interface Demo](https://img.youtube.com/vi/tcLBblnkBhU/0.jpg)](https://youtu.be/tcLBblnkBhU "GUI Interface Demo")

Another short demo, this time for the working baseband signal and spectrum viewer:

[![GUI Spectrum View](https://img.youtube.com/vi/ZY454bezK1s/0.jpg)](https://youtu.be/ZY454bezK1s "GUI Spectrum View")

## Hardware

### BOM
- 1x MIMXRT1060 Development Board
- 1x RK043FN66HS-CTG Touch Screen Module
- 2x USB Micro-B cables
- 4x Dupont F-M jumper cables
- 1x VUHRadio M.2 Module V1

This second generation GUI Interface for the VUHFRadio module is being developed with the MIMXRT1060-EVKB development kit from NXP. The kit comes with the MIMXRT1060 development board and a 4.3" LCD Module (RK043FN66HS-CTG) with a resolution of 480x272 and capacitive touch.

The VUHFRadio Module is a project I developed, it is a dual band (VHF and UHF) dual radio transceiver based around the [AX5043](www.onsemi.com/products/wireless-connectivity/wireless-rf-transceivers/ax5043) Sub-GHz transceiver IC. Both the hardware and firmware for the module are openly available from the [Website](www.notblackmagic.com/projects/vuhf-radio/) and [GitHub](https://github.com/NotBlackMagic/VUHFRadio) respectively. This VUHFRadio module can be connected and powered through the UART header with the MIMXRT1060 development board: The UART pins connected to the Arduino Header J16 Pin 1 (RX) & 2 (TX), and the power pins, 3V3 and GND, are connected to the Arduino Header J32 Pin 4 (3V3) & 6 (GND).

A custom version of the VUHFRadio Module, for the M.2 slot of the MIMXRT1060 board, was developed and is working with the hardware files (Gerber, Schematic and Assembly view) available in the "Hardware" folder. The assembled module is shown in the figure bellow:

![VUHFRadio M.2 module](./VUHFRadio_M2_Module.png)

This M.2 module should conform to the M.2 specs, with the digital interfaces working at 1.8V. But due to the level translator (TXS0108E) for the M.2 module not arriving in time, this had to be bi-passed with bodge wires and to make it work with the development board its M.2 level translators (U9 and U10) had to be removed and bi-passed with bodge wires as well. This is visible in the figure bellow, with the VUHRadio M.2 module mounted in the top left corner and the removed and bi-passed level translator ICs to its right:

![Development board with VUHFRadio M.2 module](./Development_Board_wVUHFRadioM2.png)

## Software
The GUI Interface runs on the RT-Thread RTOS and uses the [LVGL](www.lvgl.io) embedded graphics library. The base project, on which this project is built upon, with RT-Thread and LVGL running on the MIMXRT1060-EVKB board is available [here](www.github.com/NotBlackMagic/MIMXRT1060-EVKB). The GUI Interface project was developed using the RT-Thread IDE, [RT-Thread Studio](www.rt-thread.io/studio.html), and the full project files and dependencies are included in this GitHub repository. To run the project it is therefore only necessary to install RT-Thread Studio, download this repository and import this project in RT-Thread Studio. To debug and flash the board from RT-Thread Studio follow the instruction bellow in the "Flashing and Debugging" Section.

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

### Flashing and Debugging
The project uses the J-Link Debugger settings so it is necessary to flash the MIMXRT1060-EVKB on-board debug probe with the J-Link Firmware. 

The following article provides the  on how to flash the J-Link Firmware onto the on-board debug probe:
https://community.nxp.com/t5/i-MX-RT-Knowledge-Base/Using-J-Link-with-MIMXRT1060-EVKB/ta-p/1452717

### Recover Unresponsive/"Bricked" Board
When flashing an erroneous code, it can happen that the EVK board gets unresponsive, impossible to create a debug link and erase and flash a good code to it. To recover from such events, I found the following steps work:

- Re-flash the on-board debug probe with the CMSIS-DAP firmware
- Change the boot option of the EVK board to the SD card (SW4: 1010), aka to nothing, and power cycle the board
- Use the MCUXpresso "GUI Flash Tool" to fully erase the flash
- Change the boot option back to the QSPI Flash (SW4: 0010) and power cycle the board
- The board should now allow a debug connection again
