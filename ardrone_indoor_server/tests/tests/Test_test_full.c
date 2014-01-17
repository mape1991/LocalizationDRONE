/*
 * Test_full.c
 *
 *  Created on: Dec 31, 2013
 *      Author: julien
 */
#include "Test_test_full.h"

void Test_full_retrieve_message_values2(CuTest *tc)
{
	int values[NUM_BEACONS];
	// 255 255 255 255
	char message[] = "S00ff00ff00ff00ff";
	test_full_retrieve_message_values(&values, message);

	printf("ok\n");

	CuAssertTrue(tc, values[0] == 255);
	CuAssertTrue(tc, values[1] == 255);
	CuAssertTrue(tc, values[2] == 255);
	CuAssertTrue(tc, values[3] == 255);
}


