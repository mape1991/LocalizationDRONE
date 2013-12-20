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
int sock[2]; // socket descriptors
int socket_is_bound = 0;



int udp_open_socket(){
   int error_type = ERROR_TYPE_NONE;
   
   #ifdef DEBUG_ON
      printf("receiving : initialization\n");
   #endif
   // socket creation	
   if (((sock[SOCK_LISTEN] = socket(AF_INET, SOCK_DGRAM, 0))==-1) || ((sock[SOCK_SEND] = socket(AF_INET, SOCK_DGRAM, 0))==-1))
   {
      #ifdef DEBUG_ON 
         printf("receiving : error during the socket creation\n");
      #endif
	  /* program has failed to create both sockets */
      error_type = ERROR_TYPE_SOCKET_CREATION;
   }
   
   return error_type;
}



int udp_listen_once(char *message, int lg_mesg_emis, int port)
{
   int error_type = ERROR_TYPE_NONE;
   
   /* binding step is bypassed if socket is already bound
      (i.e. if the function has already been executed) */
    if(!socket_is_bound){
	
	   // socket address creation with local IP address and port
	   memset((char*) &adr_local,0,sizeof(adr_local)); // reset
	   adr_local.sin_family = AF_INET;
	   adr_local.sin_port = port;
	   adr_local.sin_addr.s_addr = INADDR_ANY;
	   
	   // socket is bound with the internal address
	   if(bind(sock[SOCK_LISTEN], (struct sockaddr*) &adr_local, lg_adr_local)==-1)
	   {
		  #ifdef DEBUG_ON
			   printf("receiving : failed binding to internal socket\n");
		  #endif
		  error_type = ERROR_TYPE_SOCKET_BINDING;
	   }
       // if no error has occurred, socket is marked as bound
	   else socket_is_bound = 1;
	   
	/* if we try to listen on a different port from the one whereto the socket is bound*/
	} else if (adr_local.sin_port != port){
		#ifdef DEBUG_ON
			   printf("socket already bound to another port\n");
		#endif
		error_type = ERROR_TYPE_SOCKET_BINDING;
	}

   /* Processing of the connection */
   if(error_type == ERROR_TYPE_NONE)
   {   
      // message reception
      recvfrom(sock[SOCK_LISTEN], message, lg_mesg_emis, 0, (struct sockaddr*) &adr_distant, &lg_adr_distant);   
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
   struct sockaddr_in adr_dest; // remote socket address
   int lg_adr_dest = sizeof(adr_dest);
	
   printf("sending : initialization\n");

   //domain et port number association
   memset((char*) &adr_dest, 0, sizeof(adr_dest));
   adr_dest.sin_family = AF_INET;
   adr_dest.sin_port = port;
   //IP address association
   inet_aton(dest, &adr_dest.sin_addr);

   // message is sent
   printf("sending : %d bytes : %s\n", size*sizeof(char), message);
   sendto(sock[SOCK_SEND], (void *) message, size*sizeof(char), 0, (struct sockaddr*) &adr_dest, lg_adr_dest);
   
   return 0;
}

int udp_send_char(char * dest, char message,int port)
{
   char message_arr[1];
   message_arr[0] = message;
   return udp_send(dest, message_arr, 1, port);
}

int udp_respond(char* message, int size, int port){
   // destination port number association
   adr_distant.sin_port = port;
   // message is sent
   printf("sending : %d bytes : %s\n", size*sizeof(char), message);
   sendto(sock[SOCK_SEND], (void *) message, size*sizeof(char), 0, (struct sockaddr*) &adr_distant, lg_adr_distant);
   return 0;
}

int udp_respond_char(char message,int port) {
   char message_arr[1];
   message_arr[0] = message;
   return udp_respond(message_arr, 1, port);
}

int udp_close_socket(){
	// close both sockets
    close(sock[SOCK_LISTEN]);
	close(sock[SOCK_SEND]);
	// socket is marked as unbound 
	socket_is_bound = 0;
      #ifdef DEBUG_ON
         printf("receiving : end of communication\n");
      #endif
   return 0;
}
