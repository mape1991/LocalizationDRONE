#include "test_thread.h"

#ifdef TEST_THREAD

#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <termios.h>
#include <pthread.h>

void *esclave_test(void * arg) {
	char response[1+NUM_BEACONS*sizeof(int)];
	// Once the thread is started, he signals his readiness to the server
	// > server I
        printf("ESCLAVE -> thread démarré.\n");
	udp_respond_char(COMM_MESSAGE_INIT_ID, PORT_DRONE_TO_SERVER);
	do{
		//read(fd, response, sizeof(response));
		scanf("%s", response);
                printf("ESCLAVE -> message reçu : %c.\n", response[0]);
		switch (response[0]){
			// stm X > thread exit
			case COMM_MESSAGE_EXIT_ID :
					printf("ESCLAVE -> commande arrêt reçue, arrêt du thread.\n");
					udp_respond_char(COMM_MESSAGE_EXIT_ID, PORT_DRONE_TO_SERVER);
					pthread_exit(NULL);
				break;
			// stm S > server S or stm B > server B
			case COMM_MESSAGE_SYNC_ID :
			case COMM_MESSAGE_BUSY_ID :
					printf("ESCLAVE -> envoi de la commande au serveur.\n");
					udp_respond(response, sizeof(response), PORT_DRONE_TO_SERVER);
				break;
			// FIXME stm I > do nothing ?
			default:
				break;
		}
		// empty the response string
		strcpy (response, "");
		getchar();
	} while (1);
	return NULL;
}

void test_thread_main(){

	pthread_t tid;
	char stm[COMM_MESSAGE_SIZE];
	int server_is_connected = 0;
	char from_server[COMM_MESSAGE_SIZE];
	
	/*
	// Serial interface init
	int fd = open (PORT_NAME, O_RDWR | O_NOCTTY | O_SYNC);
	if (fd < 0){
		printf("error %d opening %s: %s", errno, PORT_NAME, strerror (errno));
		return;
	}	
	set_interface_attribs (fd, B115200, 0);	// set speed to 115,200 bps, 8n1 (no parity)
	set_blocking (fd, 0);	// set no blocking
	*/
	// Serial interface init
	usb_init(USB_PORT_NAME, B9600, 0, 0);

   printf("Thread principal démarré.\n");
	// Initiate communication with stm
	stm[0] = COMM_MESSAGE_EXIT_ID;
	//write (fd, stm, 1);
	//read (fd, stm, 1);
   printf("Initialisation de la communication avec le STM.\n");
	printf("%c", stm[0]);
	stm[0] = getchar();
   getchar();
	// FIXME: is read blocking until it receives something? any timeout if no value received?
	if (stm[0] != COMM_MESSAGE_EXIT_ID){
      printf("Mauvaise réponse de STM, arrêt du programme.\n");
		exit(-1);
	}
   printf("Le STM a répondu.\n");

	// Inititate communication with server
	udp_open_socket();
	
	// MAIN MASTER LOOP :
	// data is read from the server and processed appropriately
   do {
     printf("J'attends un message du serveur.\n");
	  udp_listen_once(from_server, COMM_MESSAGE_SIZE, PORT_SERVER_TO_DRONE);
	  // If we receive an init signal, we launch the other thread
	  // server I > launch thread (server I included)
	  if (server_is_connected){
	      // Standard treatment
         switch (from_server[0]) {
					// server I > server I
					case COMM_MESSAGE_INIT_ID:
                  printf("MASTER -> message init reçu, serveur déjà connecté\n");
						udp_respond_char(COMM_MESSAGE_INIT_ID, PORT_DRONE_TO_SERVER);
						break;
					// server S > stm S
					case COMM_MESSAGE_SYNC_ID:
                  printf("MASTER -> message sync reçu, relai au STM32\n");
						//write (fd, from_server, 1);
						printf("%s", from_server);
						break;
					// server X > server X and stop
					case COMM_MESSAGE_EXIT_ID:
                  printf("MASTER -> message exit reçu, relai au STM32\n");
						//write (fd, from_server, 1);
						printf("%s", from_server);
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
     } else if(from_server[0] == COMM_MESSAGE_INIT_ID){
	    printf("MASTER -> message init reçu : serveur connecté, démarrage du thread esclave.\n");
		 server_is_connected = 1;
		 pthread_create(&tid, NULL, esclave_test, NULL);
     }
	} while (1);
}

#endif

