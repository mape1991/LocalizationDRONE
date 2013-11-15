#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "com/udp.h"
#include "../../ardrone_indoor_server/src/com/commons_comm.h"

int main(int argc, char ** argv){
		
	struct sockaddr_in serveur;
	char * message = NULL;
	char buf [100];

	
	while (1) {
		receptionudp(PORT_WRITE_DRONE, 1, message, &serveur);
		if (message[0] == 'I')
			envoiudp(PORT_READ_DRONE, serveur, "init");
		else if (message[0] == 'S')
			envoiudp(PORT_READ_DRONE, serveur, "sync");
		else if (message[0] == 'X'){
			envoiudp(PORT_READ_DRONE, serveur, "stop");
			exit(-1);
		}
	}
}
