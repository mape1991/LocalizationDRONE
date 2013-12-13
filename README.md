# Indoor Localization with Ultrasound technology
====================================

Day and night real-time location.

The repository is split in different parts:

- ardrone_indoor_server : server code including:
	* Graphical User Interface (GUI) using GTK
	* Tests (usb communication, udp communication, communication demo, wifi delay computation, GUI, and a full test coverage)
- ardrone_indoor_drone : drone code including
- ardrone_indoor_commons : the communication libraries for ardrone_indoor_server and ardrone_indoor_drone projects including
	* USB
	* UDP
	* global configuration header file (drone IP, usb port name, message ids)
- ardrone_stm32_server : stm32 emitter code for server-beacon interface
- ardrone_stm32_drone : stm32 receiver code for beacon-drone interface

## Server - ardrone_indoor_server
Requirements: JDK installed, JRE installed, Netbeans C/C++ IDE, ARDrone SDK

0. Download the project code from your workspace with the git commands
1. create new C/C++ project with existing sources
2. Tools > Libraries > Add : put the ARDrone SDK ARDroneLib path (to be tried as I was stuck Friday 7th Nov)

## Drone - ardrone_indoor_drone
Requirements: USB Support modules (pl2303.ko and usbserial.ko)
In case you need to recompile the linux modules for a different kernel version (with respect to the usb cable type), you must follow the procedure on the SEC INSA project googlesite with an additional manipulation during the configuration checking.

## Commons - ardrone_indoor_commons
For maintainability, please keep the usb and udp libs independent and fill in your custom configuration if needed via the global_com.h

## Emitter - ardrone_stm32_server
Requirements: STM KEIL IDE, Open code sourcery compiler (version lite), ARM compiler

0. Download the project code from your workspace with the git commands
1. Open project file in the project folder
2. To flash the code to your stm you need to have the STM32F103RB chip with USART and GPIO ports.
3. The project compiles without any warning or error

## Receiver - ardrone_stm32_drone
