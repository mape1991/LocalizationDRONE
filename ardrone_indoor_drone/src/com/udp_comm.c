#include "udp_comm.h"
#ifdef UDP_COMM_ON
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
On_received_callback udp_listen_callback = NULL;
int is_udp_listening = UDP_LISTEN_OFF;
int is_udp_sending   = UDP_SEND_OFF;
// Julien:
// the thread structure is taken from the video_stage thread example
// the udp connection code is included with new error signals to handle the diff cases..
int udp_listen(int lg_mesg_emis)
{
   struct sockaddr_in adr_local; // local socket addr
   struct sockaddr_in adr_distant; // remote socket addr
   int lg_adr_local = sizeof(adr_local);
   int lg_adr_distant = sizeof(adr_distant);
   int sock; // internal addr
   char *message=malloc(lg_mesg_emis*sizeof(char));
   
   int error_type = ERROR_TYPE_NONE;
   #ifdef DEBUG_ON
      printf("receiving : initialization\n");
   #endif
   // socket creation	
   if ((sock = socket(AF_INET, SOCK_DGRAM, 0))==-1)
   {
      #ifdef DEBUG_ON 
         printf("receiving : error during the socket creation\n");
      #endif
      error_type = ERROR_TYPE_SOCKET_CREATION;
   }
   // socket addr creation with the IP of the machine executing the program
   memset((char*) &adr_local,0,sizeof(adr_local)); // reset
   adr_local.sin_family = AF_INET;
   adr_local.sin_port = PORT_READ_DRONE;
   adr_local.sin_addr.s_addr = INADDR_ANY;
   // association @socket with the internal addr
   if(bind(sock, (struct sockaddr*) &adr_local, lg_adr_local)==-1)
   {
      #ifdef DEBUG_ON
           printf("receiving : failed binding to internal socket\n");
      #endif
      error_type = ERROR_TYPE_SOCKET_BINDING;
   }
   /* Processing of the connection */
   if(error_type == ERROR_TYPE_NONE)
   {
      // exit the listening if the button_listen on the ui is pressed or we exit the application
      while(is_udp_listening)
      {
         // message reception
         recvfrom(sock, message, lg_mesg_emis, 0, (struct sockaddr*) &adr_distant, &lg_adr_distant);   
         // simple message display
         #ifdef DEBUG_ON
            printf("receiving : %d bytes : %s\n", lg_mesg_emis, message);
         #endif
         /*tbalise = (struct timeval *) message;
         for (i = 0; i < NUM_BEACONS; i++)
                 printf("\n     receiving : Beacon %d : %d,%d s\n", i, (int)tbalise[i].tv_sec, (int)tbalise[i].tv_usec);*/
         // // if on message received callback function, we send the message to the callback function
         if (udp_listen_callback != NULL){
            udp_listen_callback(message);
         }
      }
      close(sock);
      #ifdef DEBUG_ON
         printf("receiving : end of communication\n");
      #endif
   }
   #ifdef DEBUG_ON
      printf("receiving : ended\n");
   #endif
   // by default : 0 (taken from the video_stage example)
   return error_type;
}
int udp_listen_once(char *message, int lg_mesg_emis)
{
   struct sockaddr_in adr_local; // local socket addr
   struct sockaddr_in adr_distant; // remote socket addr
   int lg_adr_local = sizeof(adr_local);
   int lg_adr_distant = sizeof(adr_distant);
   int sock; // internal addr
   int i;
   int error_type = ERROR_TYPE_NONE;
   #ifdef DEBUG_ON
      printf("receiving : initialization\n");
   #endif
   // socket creation	
   if ((sock = socket(AF_INET, SOCK_DGRAM, 0))==-1)
   {
      #ifdef DEBUG_ON 
         printf("receiving : error during the socket creation\n");
      #endif
      error_type = ERROR_TYPE_SOCKET_CREATION;
   }
   // socket addr creation with the IP of the machine executing the program
   memset((char*) &adr_local,0,sizeof(adr_local)); // reset
   adr_local.sin_family = AF_INET;
   adr_local.sin_port = PORT_READ_DRONE;
   adr_local.sin_addr.s_addr = INADDR_ANY;
   // association @socket with the internal addr
   if(bind(sock, (struct sockaddr*) &adr_local, lg_adr_local)==-1)
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
      recvfrom(sock, message, lg_mesg_emis, 0, (struct sockaddr*) &adr_distant, &lg_adr_distant);   
      // simple message display
      #ifdef DEBUG_ON
         printf("receiving : %d bytes : %s\n", lg_mesg_emis, message);
      #endif

      close(sock);
      #ifdef DEBUG_ON
         printf("receiving : end of communication\n");
      #endif
   }
   #ifdef DEBUG_ON
      printf("receiving : ended\n");
   #endif
   // by default : 0 (taken from the video_stage example)
   return error_type;
}
int udp_send(char * dest, char *message, int size)
{
   int sock;
   //struct hostent *hp;
   struct sockaddr_in adr_distant;
   int lg_adr_distant = sizeof(adr_distant);
   //struct in_addr adresse;
   int error_type = ERROR_TYPE_NONE;
   printf("sending : initialization\n");
   // socket creation		
   if ((sock = socket(AF_INET, SOCK_DGRAM, 0))==-1)
   {
           printf("sending : error during the socket creation\n");
           error_type = ERROR_TYPE_SOCKET_CREATION;
   }
   //affectation domaine et n° de port
   memset((char*) &adr_distant, 0, sizeof(adr_distant));
   adr_distant.sin_family = AF_INET;
   adr_distant.sin_port = PORT_WRITE_DRONE;
   //affectation @IP
   inet_aton(dest, &adr_distant.sin_addr);
   // construct msg then send
   printf("sending : %d bytes : %s\n", size*sizeof(char), message);
   sendto(sock, (void *) message, size*sizeof(char), 0, (struct sockaddr*) &adr_distant, lg_adr_distant);
   // close the socket
   close(sock);
   printf("sending : end of communication\n");
}
int udp_send_char(char * dest, char message)
{
   char message_arr[1];
   message_arr[0] = message;
   return udp_send(dest, message_arr, 1);
}
#endif // ifdef SERVER_COMM_ON