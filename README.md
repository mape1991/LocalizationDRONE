# Indoor Localization with Ultrasound technology
====================================

Day and night real-time location.

The repository is split in different parts:

- ardrone_indoor : server code with UI
- ardrone_stm32_server : stm32 code for server-beacon interface


## Server Code ardrone_indoor
Requirements: JDK installed, JRE installed, Netbeans C/C++ IDE, ARDrone SDK

0. Download the project code from your workspace with the git commands
1. create new C/C++ project with existing sources
2. Tools > Libraries > Add : put the ARDrone SDK ARDroneLib path (to be tried as I was stuck Friday 7th Nov)

## Server-Beacons STM32 
Requirements: STM KEIL IDE, Open code sourcery compiler (version lite), ARM compiler

0. Download the project code from your workspace with the git commands
1. Open project file in the project folder
2. To flash the code to your stm you need to have the STM32F103RB chip with USART and GPIO ports.
3. The project compiles without any warning or error
