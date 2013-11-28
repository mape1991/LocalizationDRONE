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

void test_gui_thread_udp_read();
void test_gui_thread_send();
void test_gui_thread_usb_read();
void test_gui_main(int argc, char** argv);

// used for UI control when the user wants to send one message to stm32/drone
extern char message_send_enable;
extern char message_send_id;

#endif /* TEST_GUI_H_ */
