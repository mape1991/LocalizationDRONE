#include "udp_comm.h"

#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <arpa/inet.h>

//variables globales
struct sockaddr_in adr_local; // local socket addr
struct sockaddr_in adr_distant; // remote socket addr
int lg_adr_local = sizeof(adr_local);
int lg_adr_distant = sizeof(adr_distant);
int sock[2]; // internal addr


// Julien:
// the thread structure is taken from the video_stage thread example
// the udp connection code is included with new error signals to handle the diff cases..

int udp_open_socket(){
   int error_type = ERROR_TYPE_NONE;
   
   #ifdef DEBUG_ON
      printf("receiving : initialization\n");
   #endif
   // socket creation	
   if (((sock[0] = socket(AF_INET, SOCK_DGRAM, 0))==-1) || ((sock[1] = socket(AF_INET, SOCK_DGRAM, 0))==-1))
   {
      #ifdef DEBUG_ON 
         printf("receiving : error during the socket creation\n");
      #endif
      error_type = ERROR_TYPE_SOCKET_CREATION;
   }
   
   return error_type;
}



int udp_listen_once(char *message, int lg_mesg_emis, int port)
{

   int error_type;
   // socket addr creation with the IP of the machine executing the program
   memset((char*) &adr_local,0,sizeof(adr_local)); // reset
   adr_local.sin_family = AF_INET;
   adr_local.sin_port = port;
   adr_local.sin_addr.s_addr = INADDR_ANY;
   
      // association @socket with the internal addr
   if(bind(sock[0], (struct sockaddr*) &adr_local, lg_adr_local)==-1)
   {
      #ifdef DEBUG_ON
           printf("receiving : failed binding to internal socket\n");
      #endif
      error_type = ERROR_TYPE_SOCKET_BINDING;
   }

   /* Processing of the connection */
   if(error_type == ERROR_TYPE_NONE)
   {   
      // message reception
      recvfrom(sock[0], message, lg_mesg_emis, 0, (struct sockaddr*) &adr_distant, &lg_adr_distant);   
      // simple message display
      #ifdef DEBUG_ON
         printf("receiving : %d bytes : %s\n", lg_mesg_emis, message);
      #endif

	}
   #ifdef DEBUG_ON
      printf("receiving : ended\n");
   #endif
	return error_type;
}

int udp_send(char * dest, char *message, int size, int port)
{
	struct sockaddr_in adr_dest; // remote socket addr
	int lg_adr_dest = sizeof(adr_dest);
	
   printf("sending : initialization\n");

   //affectation domaine et n° de port
   memset((char*) &adr_dest, 0, sizeof(adr_dest));
   adr_dest.sin_family = AF_INET;
   adr_dest.sin_port = port;

   //affectation @IP
   inet_aton(dest, &adr_dest.sin_addr);

   // construct msg then send
   printf("sending : %d bytes : %s\n", size*sizeof(char), message);
   sendto(sock[1], (void *) message, size*sizeof(char), 0, (struct sockaddr*) &adr_dest, lg_adr_dest);
   
   return 0;
}

int udp_send_char(char * dest, char message,int port)
{
   char message_arr[1];
   message_arr[0] = message;
   return udp_send(dest, message_arr, 1, port);
}

int udp_respond(char* message, int size, int port){
	// construct msg then send
   printf("sending : %d bytes : %s\n", size*sizeof(char), message);
   sendto(sock[1], (void *) message, size*sizeof(char), 0, (struct sockaddr*) &adr_distant, lg_adr_distant);
   return 0;
}

int udp_respond_char(char message,int port) {
   char message_arr[1];
   message_arr[0] = message;
   return udp_respond(message_arr, 1, port);
}

int udp_close_socket(){
	// close the socket
    close(sock[0]);
	close(sock[1]);
      #ifdef DEBUG_ON
         printf("receiving : end of communication\n");
      #endif
   return 0;
}
