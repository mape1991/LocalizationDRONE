/* 
 * File:   test_comm.h
 * Author: julien
 *
 * Created on November 20, 2013, 7:33 AM
 */

#ifndef TEST_COMM_H
#define	TEST_COMM_H

#include "../global_config.h"
#include "../../../ardrone_indoor_commons/com/udp_comm.h"
#include "../usb/usb.h"

void test_comm_thread_udp_read();
void test_comm_thread_send();
void test_comm_thread_usb_read();
void test_comm_main();

#endif	/* TEST_COMM_H */

