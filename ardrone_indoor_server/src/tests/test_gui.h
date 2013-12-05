/*
 * test_gui.h
 *
 *  Created on: Nov 27, 2013
 *      Author: julien
 */

#ifndef TEST_GUI_H_
#define TEST_GUI_H_

#include "../global_config.h"
#include "../../../ardrone_indoor_commons/com/udp_comm.h"
#include "../usb/usb.h"
#include "../gui/gui.h"

#define GUI_MAX_LABEL_SIZE 40

#define GUI_SERVER_HINT_INIT "Server: initialized"
#define GUI_SERVER_HINT_SYNC "Server: synchronized"
#define GUI_SERVER_HINT_EXIT "Server: disconnected"

#define GUI_DRONE_HINT_INIT "Drone: initialized"
#define GUI_DRONE_HINT_SYNC "Drone: synchronized"
#define GUI_DRONE_HINT_EXIT "Drone: disconnected"

#define GUI_CONTROLLER_HINT_INIT "Controller: initialized"
#define GUI_CONTROLLER_HINT_SYNC "Controller: synchronized"
#define GUI_CONTROLLER_HINT_EXIT "Controller: disconnected"

void test_gui_thread_udp_read();
void test_gui_thread_send();
void test_gui_thread_usb_read();
void test_gui_main(int argc, char **argv); // arguments for the gui initialization

// used for UI control when the user wants to send one message to stm32/drone
// why is it volatile : http://stackoverflow.com/questions/78172/using-c-pthreads-do-shared-variables-need-to-be-volatile
extern volatile char message_send_enable;
extern volatile char message_send_id;

#endif /* TEST_GUI_H_ */
