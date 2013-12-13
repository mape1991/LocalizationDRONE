/* 
 * File:   error_types_comm.h
 * Author: julien
 *
 * Created on November 12, 2013, 8:40 AM
 */

#ifndef GLOBAL_COMM_H
#define GLOBAL_COMM_H

/** maximum number of beacons handled in the system */
#define NUM_BEACONS 4
/** ports of communication for handling the udp communication */
#define PORT_SERVER_TO_DRONE 7000
#define PORT_DRONE_TO_SERVER 7001
/** errors returned by communication functions when communication fails */
#define ERROR_TYPE_NONE 0
#define ERROR_TYPE_SOCKET_CREATION 1
#define ERROR_TYPE_SOCKET_BINDING 2
/** standard size of id messages */
#define COMM_MESSAGE_SIZE 1
/** message drone-to-server including the drone data (time of arrivals/distance..) */
#define COMM_MESSAGE_DTS_SIZE 1+NUM_BEACONS*sizeof(int)
/** message ids with one simple character */
#define COMM_MESSAGE_INIT_ID 'I'
#define COMM_MESSAGE_SYNC_ID 'S'
#define COMM_MESSAGE_EXIT_ID 'X'
#define COMM_MESSAGE_BUSY_ID 'B'
#define COMM_MESSAGE_NONE 0
/** maximum number of sync when performing a comm_test */
#define COMM_TEST_SYNC_COUNT 3
/** usb constants */
#define USB_PORT_NAME "/dev/ttyUSB0" // common port name for both server and drone
#define USB_BUFFER_MAX_SIZE 100

#endif	/* ERROR_TYPES_COMM_H */

