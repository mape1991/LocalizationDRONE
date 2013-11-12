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

#define PORT_ENVOI_DRONE 7000
#define NB_BEACON 4

#define NO_ERROR 0
#define ERROR_SOCKET_CREATION 1
#define ERROR_SOCKET_BINDING 2

// Julien:
// the thread structure is taken from the video_stage thread example
// the udp connection code is included with new error signals to handle the diff cases..

DEFINE_THREAD_ROUTINE(server_comm, data)
{
   struct sockaddr_in adr_local; // local socket addr
   struct sockaddr_in adr_distant; // remote socket addr
   int lg_adr_local = sizeof(adr_local);
   int lg_adr_distant = sizeof(adr_distant);
   int sock; // internal addr
   int lg_mesg_emis = NB_BEACON*sizeof(struct timeval);

   char *message;
   struct timeval * tbalise;
   int i;

   message=malloc(lg_mesg_emis*sizeof(char));

   int error = NO_ERROR;
   
   PRINT("\n   Server communication thread : initialisation\n\n");
   // socket creation	
   if ((sock = socket(AF_INET, SOCK_DGRAM, 0))==-1)
   {
        PRINT("\n   Server communication thread : Error during the socket creation\n");
        error = ERROR_SOCKET_CREATION;
   }

   // socket addr creation with the IP of the machine executing the program
   memset((char*) &adr_local,0,sizeof(adr_local)); // reset
   adr_local.sin_family = AF_INET;
   adr_local.sin_port = PORT_ENVOI_DRONE;
   adr_local.sin_addr.s_addr = INADDR_ANY;

   // association @socket with the internal addr
   if(bind(sock, (struct sockaddr*) &adr_local, lg_adr_local)==-1)
   {
           PRINT("\n    Server communication thread : failed binding to internal socket\n\n");
           error = ERROR_SOCKET_BINDING;
   }
   /* Processing of the connection */
   if(error == NO_ERROR && !ardrone_tool_exit() )
   {
         // TODO input signal from GUI for exiting the transmission 
      while(!ardrone_tool_exit())
      {
         // message reception
         recvfrom(sock, message, lg_mesg_emis, 0, (struct sockaddr*) &adr_distant, &lg_adr_distant);
         // message display
         PRINT("\n     Server communication thread : Received %d bytes\n\n", lg_mesg_emis);
         tbalise = (struct timeval *) message;
         for (i = 0; i < NB_BEACON; i++)
                 PRINT("\n     Server communication thread : Beacon %d : %d,%d s\n", i, (int)tbalise[i].tv_sec, (int)tbalise[i].tv_usec);
      }

      close(sock);
      PRINT("\n     Server communication thread : end of communication\n\n");
  }

  PRINT("\n   Server communication thread ended\n\n");

  // by default : 0 (taken from the video_stage example)
  return error;
}
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
	int lg_mesg_emis = NB_BEACON*sizeof(struct timeval);
	
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
	adr_local.sin_port = PORT_ENVOI_DRONE;
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
		for (i = 0; i < NB_BEACON; i++)
			printf("Balise %d : %d,%d s\n", i, tbalise[i].tv_sec, tbalise[i].tv_usec);		
	}

	close(sock);
	printf ("PUITS : fin réception\n");
	return 0;
}*/
