#include "test_full.h"

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
#include <fcntl.h>
#include <termios.h>
#include <pthread.h>

void *esclave(void * arg) {
	char response[1+NUM_BEACONS*sizeof(int)];
	// Once the thread is started, he signals his readiness to the server
	// > server I
	udp_respond_char(COMM_MESSAGE_INIT_ID, PORT_DRONE_TO_SERVER);
	do{
		read(fd, response, sizeof(response));
		switch (response[0]){
		// stm X > thread exit
		case COMM_MESSAGE_EXIT_ID :
			pthread_exit(NULL);
			break;
		// stm S > server S
		case COMM_MESSAGE_SYNC_ID :
			udp_respond(response, sizeof(response), PORT_DRONE_TO_SERVER);
			break;
		// FIXME stm I > do nothing ?
		default:
			break;
		}
	} while (1);
	return NULL;
}

void test_full_main(){

	pthread_t tid;
	char stm[COMM_MESSAGE_SIZE];
	int server_is_connected = 0;
	char from_server[COMM_MESSAGE_SIZE];
	// Serial interface init
	int fd = open (PORT_NAME, O_RDWR | O_NOCTTY | O_SYNC);
	if (fd < 0){
		printf("error %d opening %s: %s", errno, PORT_NAME, strerror (errno));
		return;
	}	
	set_interface_attribs (fd, B115200, 0);	// set speed to 115,200 bps, 8n1 (no parity)
	set_blocking (fd, 0);	// set no blocking
	
	// Initiate communication with stm
	stm[0] = COMM_MESSAGE_EXIT_ID;
	write (fd, stm, 1);
	read (fd, stm, 1);
	// FIXME: is read blocking until it receives something? any timeout if no value received?
	if (stm[0] != COMM_MESSAGE_EXIT_ID){
		exit(-1);
	}

	// Inititate communication with server
	udp_open_socket();
	
	// MAIN MASTER LOOP :
	// data is read from the server and processed appropriately
	do {
      udp_listen_once(from_server, COMM_MESSAGE_SIZE, PORT_SERVER_TO_DRONE);
      // If we receive an init signal, we launch the other thread
      // server I > launch thread (server I included)
      if(!server_is_connected && from_server[0] == COMM_MESSAGE_INIT_ID){
			server_is_connected = 1;
			pthread_create(&tid, NULL, esclave, NULL);
      } else if (server_is_connected){
      	// Standard treatment
			switch (from_server[0]) {
				// server I > server I
				case COMM_MESSAGE_INIT_ID:
					udp_respond_char(COMM_MESSAGE_INIT_ID, PORT_DRONE_TO_SERVER);
					break;
				// server S > stm S
				case COMM_MESSAGE_SYNC_ID:
					write (fd, from_server, 1);
					break;
				// server X > server X and stop
				case COMM_MESSAGE_EXIT_ID:
					write (fd, from_server, 1);
					server_is_connected = 0;
					pthread_join(tid, NULL);
					break;
				// server others > stop
				default:
					printf("Unexpected message received\n");
					pthread_exit(tid);
						// FIXME : do we really need to exit the process ?
					exit(-1);
					break;
			}
	  }
	} while (1);
}
