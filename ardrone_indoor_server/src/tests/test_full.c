/*
 * test_full.c
 *
 *  Created on: Dec 11, 2013
 *      Author: julien
 */
#include "test_full.h"
#include "../pos/trilateration.h"
// stdlib for itoa
#include <stdlib.h>
#include <stdio.h>

#ifdef TEST_FULL

/**
 * Converts the message into the four beacon integer distances
 * if message is entered using the terminal, the converted value uses ASCII encoding
 * i.e. MESSAGE S 0000 0000 0000 0000 is converted into 808464432,808464432,808464432,808464432
 */
void test_full_retrieve_message_values(int *values, char *message)
{
	printf("retrieving toa values from the message\n");
	// copy all message values into a integer array of values
	memcpy(values, ((message+1)), NUM_BEACONS*sizeof(int));
}

/** handle the data received by the drone including the timeofarrivals/distances of the beacons (on integers)
 * NOTE: if message is entered using the terminal, the converted value uses ASCII encoding
 * i.e. MESSAGE S 0000 0000 0000 0000 is converted into 808464432,808464432,808464432,808464432
 */
void test_full_thread_udp_read_sync(char *message)
{
	double coordinates[3];
	double dim_room[3]={15,10,5};
	int selected_beacons[NUM_BEACONS]={1,2,3,4};
	double distances[NUM_BEACONS];

	printf("executing test_full_thread_udp_read_sync\n");
	//printf("message %s\n", message);
	printf("message %d\n", message);
	// first byte is the message id
	// we check the rest for retrieving the data
	char text_label[GUI_MAX_LABEL_SIZE];
	int i = 0, values[NUM_BEACONS];
	// retrieve the message values from the message
	test_full_retrieve_message_values(values, message);
	// report the value on the GUI
	printf("displaying toas on the gui\n");
	for (; i < NUM_BEACONS; i++){
		values[i] -= 65*(1+256+256*256+256*256*256);
		printf("beacon%d : %d\n", i, values[i]);
		snprintf(text_label, GUI_MAX_LABEL_SIZE, "%d", values[i]);
		gtk_label_set_text(get_gui()->label_beacon_timevals[i], text_label);
	}

	// TODO: launch calculations from beacons values
	for(i = 0; i < NUM_BEACONS; i++){
		distances[i] = (double)(values[i]*340.0)/(128000.0);
		printf("distances%d : %f\n", i, distances[i]);
	}

	trilateration(distances, coordinates, dim_room, NUM_BEACONS, selected_beacons);
	for (i = 0; i < 3; i++){
		printf("coordinate  %d : %f\n", i, coordinates[i]);
		snprintf(text_label, GUI_MAX_LABEL_SIZE, "%f", coordinates[i]);
		gtk_label_set_text(get_gui()->label_drone_pos_values[i], text_label);
	}
}

void test_full_main()
{
	// the test full has just a different implementation for the udp reading when receiving a measure
	thread_udp_read_sync = &test_full_thread_udp_read_sync;
	// launch the usual test_gui after assigning the correct behaviour for the sync udp messages
	test_gui_main(0, 0);
}
#endif
