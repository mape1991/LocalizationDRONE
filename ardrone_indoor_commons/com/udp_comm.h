/* 
 * File:   server_thread.h
 * Author: julien
 *
 * Created on November 12, 2013, 2:06 AM
 */

#ifndef UDP_COMM_H
#define	UDP_COMM_H

#include "../global_com.h"

int udp_open_socket();
int udp_listen_once(char *message, int lg_mesg_emis, int port);  /* listen to drone, return a type of errors */
int udp_send(char * dest, char *message, int size, int port); /*  */
int udp_send_char(char * dest, char message, int port);
int udp_respond(char* message, int size, int port);
int udp_respond_char(char message,int port);
int udp_close_socket();

#endif	/* SERVER_THREAD_H */

