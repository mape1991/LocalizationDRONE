/* 
 * File:   server_thread.h
 * Author: julien
 *
 * Created on November 12, 2013, 2:06 AM
 */

#ifndef UDP_COMM_H
#define	UDP_COMM_H

#include "../global_com.h"

#define UDP_LISTEN_ON    1
#define UDP_LISTEN_OFF   0

#define UDP_SEND_ON      1
#define UDP_SEND_OFF     0

#define UDP_SIGNAL_NONE  0 // no signal
#define UDP_SIGNAL_START 1 // start signal to the drone for starting the measurement

typedef void (*On_received_callback)(char *);  

int udp_listen(int lg_mesg_emis,int port); /* listen to drone, return a type of errors */
int udp_listen_once(char *message, int lg_mesg_emis, int port);
int udp_send(char * dest, char *message, int size, int port); /*  */
int udp_send_char(char * dest, char message, int port);

extern On_received_callback udp_listen_callback; // can initialize a function pointer on a callback function, called whenever a message is received
extern int is_udp_listening;
extern int is_udp_sending;

#endif	/* SERVER_THREAD_H */

