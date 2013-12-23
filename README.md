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

### Source Documentation
The Drone,Server and Commons projects are documented using doxygen.
A .doxygen file is to be found at each root project folder containing parameters to parse the source comments along with the code files.
For adding your comments to the documentation, please respect the syntax specified in doxygen reference manual.
See http://www.stack.nl/~dimitri/doxygen/manual/docblocks.html

You can either generate the sources using doxygen from the command line or adding a plugin to your IDE supporting the build utility for .doxygen files (i.e. Eclipse uses Eclox).

## Server - ardrone_indoor_server
Requirements: JDK installed, JRE installed, Eclipse CDT, ARDrone SDK, Gtk (>= 2.0), GtkExtra (>= 3.0)

0. Download the project code from your workspace with the git commands, download the gtkextra lib 3.0
1. Create new C/C++ project with existing sources
2. Project > Properties > C/C++ General > Paths and Symbols > Includes section > GNU C : put the ARDrone SDK ARDroneLib path, put the gtkextra3.X/gtkextra directory path (if you want to get rid of any unresolved includes, you need to specify any dependent subpaths)
3. Specify all mandatory paths of the user_template.properties in a user.properties file with the same properties names.

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
