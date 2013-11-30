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
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <pthreads.h>

#include "udp.h"
#include "ports.h"
#include "port_serie_config.h"

void * esclave(void * arg) {
	char response[1+NB_BEACONS*sizeof(int)];
	// Once the thread is started, he signals his readiness to the server
	udp_respond_char('I', PORT_DRONE_TO_SERVER);
	do{
		read(fd, response, sizeof(response));
		if (response[0] == 'X'){
			pthread_exit(NULL);
		}
		else {
			udp_respond(response, sizeof(response), PORT_DRONE_TO_SERVER);
		}
	} while (1);
}

void communication_main(){
		
	char * portname = "/dev/ttyO3"
	pthread_t tid;
	char stm[1];
	int server_is_connected = 0;
	char from_server[COMM_MESSAGE_SIZE];
	

	// Serial interface init
	int fd = open (portname, O_RDWR | O_NOCTTY | O_SYNC);
	if (fd < 0){
		error_message ("error %d opening %s: %s", errno, portname, strerror (errno));
		return;
	}	
	set_interface_attribs (fd, B115200, 0);	// set speed to 115,200 bps, 8n1 (no parity)
	set_blocking (fd, 0);	// set no blocking
	
	// Initiate communication with stm
	stm[0] = 'X';
	write (fd, stm, 1);
	read (fd, stm, 1);
	if (stm[0] != 'X'){
		exit(-1);
	}

	// Inititate communication with server
	udp_open_socket();
	
	// MAIN MASTER LOOP :
	// data is read from the server and processed appropriately
	do {
      udp_listen_once(from_server, COMM_MESSAGE_SIZE, PORT_SERVER_TO_DRONE);
	  // If we receive an init signal, we launch the other thread
	  if(!server_is_connected && from_server[0] == 'I'){
		server_is_connected = 1;
		pthread_create(&tid, NULL, esclave, NULL);
	  } else if (server_is_connected){
	    // Standard treatment
		switch from_server[0] {
			case 'I':
				udp_respond_char('I', PORT_DRONE_TO_SERVER);
				break;
			case 'X':
				write (fd, from_server, 1);
				server_is_connected = 0;
				pthread_join(tid);
				break;
			case 'S':
				write (fd, from_server, 1);
				break;
			case default:
				printf("Unexpected message received\n");
				pthread_exit(tid);
				exit(-1);
		}
	  }
	} while (1);
}
