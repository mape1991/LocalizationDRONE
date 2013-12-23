/* 
 * File:   test_comm.h
 * Author: julien
 *
 * Created on November 20, 2013, 7:33 AM
 */

#ifndef TEST_COMM_H
#define TEST_COMM_H

#include "../global_config.h"
#include "../../../ardrone_indoor_commons/global_com.h"
#include "../../../ardrone_indoor_commons/com/udp_comm.h"
#include "../../../ardrone_indoor_commons/usb/usb.h"

/**
 * Responsible for the behavior of the udp thread listening to the input port.
 * - listens continuously to any received message
 * - outputs it to the user using printf function
 */
void test_comm_thread_udp_read();

/**
 * Responsible for the behavior of the udp thread sending to the output port.
 * The test includes a sequence of actions as follows :
 * - sends an initialization message
 * - sends a sequence of synchronization messages
 * - sends an exit message
 * - if usb is on, sends an initialization message to the emitter
 *  	- if usb is on, sends a synchronization message to the emitter.
 *
 * The test does not wait for receiver's acknowledgements.
 */
void test_comm_thread_send();
void test_comm_thread_usb_read();
void test_comm_main();

#endif	/* TEST_COMM_H */

