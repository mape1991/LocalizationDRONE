/* 
 * File:   error_types_comm.h
 * Author: julien
 *
 * Created on November 12, 2013, 8:40 AM
 */

#ifndef GLOBAL_COMM_H
#define	GLOBAL_COMM_H

#define NUM_BEACONS 4

#define PORT_SERVER_TO_DRONE 7000
#define PORT_DRONE_TO_SERVER 7001

#define ERROR_TYPE_NONE 0
#define ERROR_TYPE_SOCKET_CREATION 1
#define ERROR_TYPE_SOCKET_BINDING 2

#define COMM_MESSAGE_SIZE 1
#define UDP_MESSAGE_DRONE_SIZE 5
#define COMM_MESSAGE_INIT_ID 'I'
#define COMM_MESSAGE_SYNC_ID 'S'
#define COMM_MESSAGE_EXIT_ID 'X'
#define COMM_MESSAGE_NONE    ''
#define UDP_MESSAGE_DRONE_INIT_ID "init"
#define UDP_MESSAGE_DRONE_SYNC_ID "sync"
#define UDP_MESSAGE_DRONE_EXIT_ID "exit"
#define UDP_MESSAGE_SYNC_COUNT 3

#endif	/* ERROR_TYPES_COMM_H */

