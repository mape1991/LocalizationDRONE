/**
 * test_gui.h
 *
 *  Created on: Nov 27, 2013
 *      Author: julien
 *
 *  This test is meant to cover the basic actions of the GUI
 *  including the connection/disconnection and synchronization with the drone and Server Emitter (stm32)
 *  NOT INTENDED FOR RECEIVING MESSAGES EXCEEDING ONE BYTE
 */

#ifndef TEST_GUI_H_
#define TEST_GUI_H_

#include "../global_config.h"
#include "../../../ardrone_indoor_commons/global_com.h"
#include "../../../ardrone_indoor_commons/com/udp_comm.h"
#include "../../../ardrone_indoor_commons/usb/usb.h"
#include "../gui/gui.h"
/** include for the semaphores */
#include <semaphore.h>

/** maximum number of characters in a gtk_label */
#define GUI_MAX_LABEL_SIZE 40
/** */
#define GUI_SERVER_HINT_INIT "Server: initialized"
/** */
#define GUI_SERVER_HINT_SYNC "Server: synchronized"
/** */
#define GUI_SERVER_HINT_EXIT "Server: disconnected"
/** */
#define GUI_DRONE_HINT_INIT "Drone: initialized"
/** */
#define GUI_DRONE_HINT_SYNC "Drone: synchronized"
/** */
#define GUI_DRONE_HINT_EXIT "Drone: disconnected"
/** */
#define GUI_CONTROLLER_HINT_INIT "Controller: initialized"
/** */
#define GUI_CONTROLLER_HINT_SYNC "Controller: synchronized"
/** */
#define GUI_CONTROLLER_HINT_EXIT "Controller: disconnected"

/**
 * Impl
 */
void test_gui_thread_udp_read_sync(char *message);
/**
 * Responsible for the behavior of the udp thread listening to the input port
 * - listens to the sender
 * - updates the gui according to the received message
 * - except on a sync message, redirect to the behavior defined by the test itself
 */
void test_gui_thread_udp_read(int message_size);
/**
 *
 */
void test_gui_thread_send();
/**
 *
 */
void test_gui_thread_usb_read();
/**
 * main entry of the test_gui
 */
void test_gui_main(int argc /** number of arguments */, char **argv /** list of arguments */);

/**
 * Semaphore triggered in the ui buttons callbacks (when clicking on the buttons).
 * It locks/unlocks the critical section entry in the communication threads
 * for proper scheduled and non-interfering communication.
 */
extern sem_t message_sema;
/**
 * This variable helps to identify the message to be sent.
 *
 * It is typically defined by a udp listening thread and asks for the sending thread the
 * proper message to be sent.
 */
extern char message_send_id;
/**
 * This function occurs when the udp thread receives an synchronization message.
 * Then an external test using the test_gui can change the behavior of the message reception.
 *
 * i.e. by default it updates the gui state whereas in the test_full it updates the beacons labels with
 * the correct time of arrivals.
 */
extern void (*thread_udp_read_sync)(char *message);

#endif /* TEST_GUI_H_ */
