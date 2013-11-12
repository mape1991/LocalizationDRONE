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

int main(int argc, char ** argv)
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
	return(0);
}
