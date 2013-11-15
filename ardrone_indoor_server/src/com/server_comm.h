/* 
 * File:   server_thread.h
 * Author: julien
 *
 * Created on November 12, 2013, 2:06 AM
 */

#ifndef SERVER_COMM_H
#define	SERVER_COMM_H

#include "../global_config.h"

#ifdef SERVER_COMM_ON

#include "commons_comm.h"
#include <config.h>

#define SERVER_LISTEN_ON    1
#define SERVER_LISTEN_OFF   0

#define SERVER_SEND_ON      1
#define SERVER_SEND_OFF     0

#define SERVER_SIGNAL_NONE  0 // no signal
#define SERVER_SIGNAL_START 1 // start signal to the drone for starting the measurement

typedef void (*On_received_callback)(char *);  

int server_listen_drone(); /* listen to drone, return a type of errors */
extern On_received_callback server_listen_drone_callback; // can initialize a function pointer on a callback function, called whenever a message is received
extern int is_server_listening;
extern int is_server_sending;

#endif

#endif	/* SERVER_THREAD_H */

