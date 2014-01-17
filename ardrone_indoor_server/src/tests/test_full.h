/*
 * test_full.h
 *
 *  Created on: Dec 8, 2013
 *      Author: julien
 *
 *  This test is meant to cover the overall range of system actions
 *  including the connection/disconnection and synchronization with the drone and Server Emitter (stm32)
 *  EXTENDING THE TEST_GUI WITH MESSAGES EXCEEDING ONE BYTE WHEN SYNCHRONIZING WITH THE DRONE
 */


#ifndef TEST_FULL_H_
#define TEST_FULL_H_

#include "test_gui.h"

/**
 * Populates the values array with the string values contained in the message using a string-to-integer conversion
 */
void test_full_retrieve_message_values(int **values, char *message);

/**
 *
 */
void test_full_main();

#endif /* TEST_FULL_H_ */
