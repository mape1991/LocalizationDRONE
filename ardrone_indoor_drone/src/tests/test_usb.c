#include "test_usb.h"


void test_usb_main()
{
	char stm[COMM_MESSAGE_SIZE];

	usb_init(USB_PORT_NAME, B9600, 0, 1);	// set speed to 9600 bps, 8n1 (no parity)
	// set blocking (timeout = 0.5sec)

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
		stm[0] = getchar();
		printf("\n");
		usb_write_char(stm[0]);
		// is blocking, react on timeout
	   usb_read(stm, COMM_MESSAGE_SIZE);
	   printf("Stm response: %c", stm[0]);
	}
}
