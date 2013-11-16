/* 
 * File:   error_types_comm.h
 * Author: julien
 *
 * Created on November 12, 2013, 8:40 AM
 */

#ifndef COMMONS_COMM_H
#define	COMMONS_COMM_H

#include "../global_config.h"

#define PORT_WRITE_DRONE 7000
#define PORT_READ_DRONE 7001
#define NUM_BEACONS 4

#define ERROR_TYPE_NONE 0
#define ERROR_TYPE_SOCKET_CREATION 1
#define ERROR_TYPE_SOCKET_BINDING 2

#ifdef TEST_GUI_STANDALONE
        #define DEST_IP "192.168.87.130"
#else
        #define DEST_IP "192.168.1.1"
#endif

#endif	/* ERROR_TYPES_COMM_H */

