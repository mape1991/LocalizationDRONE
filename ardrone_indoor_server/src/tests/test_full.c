/*
 * test_full.c
 *
 *  Created on: Dec 11, 2013
 *      Author: julien
 */
#include "test_full.h"

// stdlib for itoa
#include <stdlib.h>
#include <stdio.h>

#ifdef TEST_FULL

void test_full_retrieve_message_values(int **values, char *message)
{
		// copy all message values into a integer array of values
		memcpy(*values, ((int *) (message+1), NUM_BEACONS*sizeof(int));
}

// handle the data received by the drone including the timeofarrivals/distances of the beacons (on integers)
void test_full_thread_udp_read_sync(char *message)
{
	// first byte is the message id
	// we check the rest for retrieving the data
	char text_label[GUI_MAX_LABEL_SIZE];
	int i = 0, values[NUM_BEACONS];
	// retrieve the message values from the message
	test_full_retrieve_message_values(&values, message);
	// report the value on the GUI
	for (; i < NUM_BEACONS; i++){
		itoa(values[i], text_label, 10);
		gtk_label_set_text(get_gui()->label_beacon_timevals[i], text_label);
	}

	// TODO: launch calculations from beacons values
}

void test_full_main()
{
	// the test full has just a different implementation for the udp reading when receiving a measure
	thread_udp_read_sync = &test_full_thread_udp_read_sync;
	// launch the usual test_gui after assigning the correct behaviour for the sync udp messages
	test_gui_main(0, 0);
}
#endif
