#include "test_full.h"

#if (defined(TEST_FULL) || defined(TEST_THREAD))

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


/** This function is executed by the slave thread
  * it reads from the USB port and froward the response to the server
  * arg: unused argument
*/
void *esclave(void * arg) {

	char response[1+NUM_BEACONS*sizeof(int)];

	// Once the thread is started, he signals his readiness to the server
	// > server I
   printf("ESCLAVE -> thread démarré.\n");
	udp_respond_char(COMM_MESSAGE_INIT_ID, PORT_DRONE_TO_SERVER);
	// main loop
	do{
		#ifdef TEST_FULL
			usb_read(response, sizeof(response));
		#elif TEST_THREAD
			// note: space before %s for ignoring the whitespaces/newlines
			// see http://stackoverflow.com/questions/8300963/while-loop-ignores-scanf-the-second-time
			scanf(" %s", response);
		#endif
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
	} while (1);
	return NULL;
}

void test_full_main(){

	pthread_t tid;
	char stm[COMM_MESSAGE_SIZE];
	int server_is_connected = 0;
	char from_server[COMM_MESSAGE_SIZE];

	// Serial interface init
	usb_init(USB_PORT_NAME, B9600, 0, 1);

   printf("Thread principal démarré.\n");
	// Initiate communication with stm
	stm[0] = COMM_MESSAGE_INIT_ID;
   printf("Initialisation de la communication avec le STM.\n");
   // send the init message and wait for acknowledgement
	#ifdef TEST_FULL
		usb_write_char(stm[0]);
		stm[0] = 0;
		usb_read(stm, 1);
	// if testing thread, wait for user input
	#elif TEST_THREAD
		stm[0] = getchar();
	   getchar();
	#endif
	// FIXME: is read blocking until it receives something? any timeout if no value received?
	if (stm[0] != COMM_MESSAGE_INIT_ID){
      printf("Mauvaise réponse de STM, arrêt du programme.\n");
		exit(-1);
	}
   printf("Le STM a répondu.\n");

	// Inititate communication with server
	udp_open_socket();
	
	// MAIN MASTER LOOP :
	// data is read from the server and processed appropriately
   do {
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
						#ifdef TEST_FULL
                  	usb_write_char(from_server[0]);
						#endif
                  printf("%s", from_server);
						break;
					// server X > server X and stop
					case COMM_MESSAGE_EXIT_ID:
                  printf("MASTER -> message exit reçu, relai au STM32\n");
						#ifdef TEST_FULL
                  	usb_write_char(from_server[0]);
						#endif
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
		 pthread_create(&tid, NULL, esclave, NULL);
     }
	  // reinitialize the message
	  from_server[0] = 0;
	} while (1);
}

#endif


