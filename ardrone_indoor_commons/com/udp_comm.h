/* 
 * File:   server_thread.h
 * Author: julien
 * 
 * Created on November 12, 2013, 2:06 AM
 */

#ifndef UDP_COMM_H
#define	UDP_COMM_H

#include "../global_com.h"


#define SOCK_LISTEN 0
#define SOCK_SEND 1

/** Opens two UDP sockets: one that will be used to receive messages, one that will be used to send messages
  * returns 0 if the sockets have been opened correctly
  */
int udp_open_socket();

/** Waits for a message
  * message: the address whereat the message shall be written, needs to be pre-allocated
  * lg_mesg_emis: the number of char that the function should expect
  * port: the port to which the socket should be bound
  */
int udp_listen_once(char *message, int lg_mesg_emis, int port);  /* listen to drone, return a type of errors */

/** Sends a message to a specified IP address
  * dest: an IP address (format w.x.y.z)
  * message: the message to be sent
  * size: the size of the message to be sent
  * port: the destination port
  */
int udp_send(char * dest, char *message, int size, int port); /*  */

/** sends a single character to a specified IP address
  * dest: an IP address (format w.x.y.z)
  * message: the character to be sent
  * port: the destination port
  */
int udp_send_char(char * dest, char message, int port);

/** Once the udp_listen_once function has been used, one can use this function instead of udp_send() to respond to the sender without specifying its address
  * message: the message to be sent
  * size: the size of the message to be sent
  * port: the destination port
  */
int udp_respond(char* message, int size, int port);

/** Once the udp_listen_once function has been used, one can use this function instead of udp_send() to respond to the sender without specifying its address
  * message: the character to be sent
  * port: the destination port
  */
int udp_respond_char(char message,int port);

/** close both sockets */
int udp_close_socket();

#endif	/* SERVER_THREAD_H */

