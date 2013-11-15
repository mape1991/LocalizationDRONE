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
#include <arpa/inet.h>


int receptionudp(int port, int lg_mesg_emis, char * message, struct sockaddr_in * sender)
{

	struct sockaddr_in adr_local; // adresse du socket local
	int lg_adr_local = sizeof(adr_local);
	int lg_adr_distant;
	int sock; // adresse interne

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
	adr_local.sin_port = port;
	adr_local.sin_addr.s_addr = INADDR_ANY;
	
	// association @socket avec l'adresse interne
	if(bind(sock, (struct sockaddr*) &adr_local, lg_adr_local)==-1)
	{
		printf("Echec du bind\n");
		exit(1);
	}
	
	// réception du message
	recvfrom(sock, message, lg_mesg_emis, 0, (struct sockaddr*) sender, &lg_adr_distant);
	// affichage du message
	printf("PUITS : Réception de %d octets.\n", lg_mesg_emis);

	//fermeture du socket
	close(sock);
	printf ("PUITS : fin réception\n");
	return(0);
}


//envoie un message
void envoiudp(int port, struct sockaddr_in adr_distant, char * message){
	int sock;
	int lg_adr_distant = sizeof(adr_distant);

	// création du socket	
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0))==-1)
	{
		printf("Echec de création du socket\n");
		exit(1);
   }

	//affectation domaine et n° de port
	memset((char*) &adr_distant, 0, sizeof(adr_distant));
	adr_distant.sin_family = AF_INET;
	adr_distant.sin_port = port;

	// construction du message puis envoi
	printf("SOURCE : Envoi de %d octets.\n", sizeof(message));
	sendto(sock, (void *) message, sizeof(message), 0, (struct sockaddr*) &adr_distant, lg_adr_distant);

	// fermeture du socket
	close(sock);
	printf("SOURCE : fin émission\n");
}
