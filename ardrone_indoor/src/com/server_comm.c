#include "server_comm.h"

#ifdef SERVER_COMM_ON

#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <sys/time.h>

On_received_callback server_listen_drone_callback = NULL;
int is_server_listening = SERVER_LISTEN_OFF;
int is_server_sending   = SERVER_SEND_OFF;

// Julien:
// the thread structure is taken from the video_stage thread example
// the udp connection code is included with new error signals to handle the diff cases..

int server_listen_drone()
{
   struct sockaddr_in adr_local; // local socket addr
   struct sockaddr_in adr_distant; // remote socket addr
   int lg_adr_local = sizeof(adr_local);
   int lg_adr_distant = sizeof(adr_distant);
   int sock; // internal addr
   int lg_mesg_emis = NUM_BEACONS*sizeof(struct timeval);

   char *message;
   struct timeval * tbalise;
   int i;

   message=malloc(lg_mesg_emis*sizeof(char));

   int error_type = ERROR_TYPE_NONE;
   #ifdef DEBUG_ON
      printf("\n   Server communication thread : receiving : initialization\n\n");
   #endif
   // socket creation	
   if ((sock = socket(AF_INET, SOCK_DGRAM, 0))==-1)
   {
      #ifdef DEBUG_ON 
         printf("\n   Server communication thread : receiving : error during the socket creation\n");
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
           printf("\n    Server communication thread : receiving : failed binding to internal socket\n\n");
      #endif
      error_type = ERROR_TYPE_SOCKET_BINDING;
   }
   /* Processing of the connection */
   if(error_type == ERROR_TYPE_NONE && !ardrone_tool_exit() )
   {
      // exit the listening if the button_listen on the ui is pressed or we exit the application
      while(!ardrone_tool_exit() && is_server_listening)
      {
         // message reception
         recvfrom(sock, message, lg_mesg_emis, 0, (struct sockaddr*) &adr_distant, &lg_adr_distant);
         // if no on message received callback function
         if (server_listen_drone_callback == NULL){
            // simple message display
            #ifdef DEBUG_ON
               printf("\n     Server communication thread : receiving : %d bytes\n\n", lg_mesg_emis);
            #endif
            tbalise = (struct timeval *) message;
            for (i = 0; i < NUM_BEACONS; i++)
                    printf("\n     Server communication thread : receiving : Beacon %d : %d,%d s\n", i, (int)tbalise[i].tv_sec, (int)tbalise[i].tv_usec);
         // otherwise we send the message to the callback function
         }else{
            server_listen_drone_callback(message);
         }
      }

      close(sock);
      #ifdef DEBUG_ON
         printf("\n     Server communication thread : receiving : end of communication\n\n");
      #endif
   }
   #ifdef DEBUG_ON
      printf("\n   Server communication thread : receiving : ended\n\n");
   #endif
   // by default : 0 (taken from the video_stage example)
   return error_type;
}

int server_send_drone(char * dest, char *signal)
{
   int sock;
   //struct hostent *hp;
   struct sockaddr_in adr_distant;
   int lg_adr_distant = sizeof(adr_distant);
   //struct in_addr adresse;
   int error_type = ERROR_TYPE_NONE;

   printf("\n   Server communication thread : sending : initialization\n\n");
   // socket creation		
   if ((sock = socket(AF_INET, SOCK_DGRAM, 0))==-1)
   {
           printf("\n   Drone communication thread : sending : error during the socket creation\n");
           error_type = ERROR_TYPE_SOCKET_CREATION;
   }

   //affectation domaine et n° de port
   memset((char*) &adr_distant, 0, sizeof(adr_distant));
   adr_distant.sin_family = AF_INET;
   adr_distant.sin_port = PORT_WRITE_DRONE;

   //affectation @IP
   inet_aton(dest, &adr_distant.sin_addr);

   // construct msg then send
   printf("\n     Drone communication thread : sending : %d bytes\n\n", sizeof(char));
   sendto(sock, (void *) signal, sizeof(char), 0, (struct sockaddr*) &adr_distant, lg_adr_distant);

   // close the socket
   close(sock);
   printf("\n     Server communication thread : sending : end of communication\n\n");
}

#endif // ifdef SERVER_COMM_ON
/*
 * Aymeric Code :
 * 
int communicate_with_server()
{
	struct sockaddr_in adr_local; // adresse du socket local
	struct sockaddr_in adr_distant; // adresse du socket distant
	int lg_adr_local = sizeof(adr_local);
	int lg_adr_distant = sizeof(adr_distant);
	int sock; // adresse interne
	int lg_mesg_emis = NUM_BEACONS*sizeof(struct timeval);
	
	char *message;
	struct timeval * tbalise;
	int i;
	
	message=malloc(lg_mesg_emis*sizeof(char));

	// création du socket	
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0))==-1)
	{
		printf("Echec de création du socket\n");
		exit(1);
        }

	// construction de l'adresse du socket avec n° de port=8765 et de
	// l'@IP de la machine sur laquelle s'execute le programme
	memset((char*) &adr_local,0,sizeof(adr_local)); // reset
	adr_local.sin_family = AF_INET;
	adr_local.sin_port = PORT_DRONE;
	adr_local.sin_addr.s_addr = INADDR_ANY;
	
	// association @socket avec l'adresse interne
	if(bind(sock, (struct sockaddr*) &adr_local, lg_adr_local)==-1)
	{
		printf("Echec du bind\n");
		exit(1);
	}

	// traitement du message
	while (1) 
	{
		// réception du message
		recvfrom(sock, message, lg_mesg_emis, 0, (struct sockaddr*) &adr_distant, &lg_adr_distant);
		// affichage du message
		printf("PUITS : Réception de %d octets.\n", lg_mesg_emis);
		tbalise = (struct timeval *) message;
		for (i = 0; i < NUM_BEACONS; i++)
			printf("Balise %d : %d,%d s\n", i, tbalise[i].tv_sec, tbalise[i].tv_usec);		
	}

	close(sock);
	printf ("PUITS : fin réception\n");
	return 0;
}*/
