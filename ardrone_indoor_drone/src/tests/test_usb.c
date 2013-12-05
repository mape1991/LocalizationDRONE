#include "test_usb.h"


void test_usb_main()
{
	char stm[COMM_MESSAGE_SIZE];
	// Serial interface init
	int fd = open (PORT_NAME, O_RDWR | O_NOCTTY | O_SYNC);
	if (fd < 0){
		printf("error %d opening %s: %s", errno, PORT_NAME, strerror (errno));
		return;
	}
	set_interface_attribs (fd, B9600, 0);	// set speed to 9600 bps, 8n1 (no parity)
	set_blocking (fd, 1);	// set blocking (timeout = 0.5sec)

	// Initiate communication with stm
	/*stm[0] = COMM_MESSAGE_EXIT_ID;
	write (fd, stm, 1);
	// is blocking, react on timeout
	read (fd, stm, 1);
	if (stm[0] != COMM_MESSAGE_EXIT_ID){
		exit(-1);
	}*/

	while(1){
		printf("Enter char input: ");
		scanf("%c", stm);
		printf("\n");
		write (fd, stm, 1);
		// is blocking, react on timeout
	   read (fd, stm, 1);
	   printf("Stm response: %c", stm[0]);
	}
}
