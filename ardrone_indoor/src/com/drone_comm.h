/* 
 * File:   drone_comm.h
 * Author: julien
 *
 * Created on November 12, 2013, 8:31 AM
 */

#ifndef DRONE_COMM_H
#define	DRONE_COMM_H

#include "../global_config.h"
#include "commons_comm.h"

#ifdef DRONE_COMM_ON
/**
 * function retrieving the current time for the beacon beacon_id
 * @param beacon_id the id of the beacon having its time updated
 */
void retrieve_timevals(int beacon_id);

/**
 * sends the message dest to the server with the given port, addr..
 * @param dest the message to be sent to the server
 */
void send_time(char * dest);

/**
 * 
 * @param argc
 * @param argv
 * @return 
 */
int send_to_server(int argc, char ** argv);

struct timeval tbalise[NUM_BEACONS];

#endif

#endif	/* DRONE_COMM_H */

