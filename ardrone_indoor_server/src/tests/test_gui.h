/*
 * test_gui.h
 *
 *  Created on: Nov 27, 2013
 *      Author: julien
 */

#ifndef TEST_GUI_H_
#define TEST_GUI_H_

#include "../global_config.h"
#include "../../../ardrone_indoor_commons/global_com.h"
#include "../../../ardrone_indoor_commons/com/udp_comm.h"
#include "../../../ardrone_indoor_commons/usb/usb.h"
#include "../gui/gui.h"

#define GUI_MAX_LABEL_SIZE 40

#define GUI_DRONE_HINT_INIT "Drone: initialized"
#define GUI_DRONE_HINT_SYNC "Drone: synchronized"
#define GUI_DRONE_HINT_EXIT "Drone: disconnected"

#define GUI_CONTROLLER_HINT_INIT "Controller: initialized"
#define GUI_CONTROLLER_HINT_SYNC "Controller: synchronized"
#define GUI_CONTROLLER_HINT_EXIT "Controller: disconnected"

void test_gui_thread_udp_read();
void test_gui_thread_send();
void test_gui_thread_usb_read();
void test_gui_main();

// used for UI control when the user wants to send one message to stm32/drone
extern char message_send_enable;
extern char message_send_id;

#endif /* TEST_GUI_H_ */
