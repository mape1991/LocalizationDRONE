#include "test_usb.h"

/**
 * 2013-12-13 WARNING: required usb modules in linux kernel
 *
 * we pulled through a blocking issue due to a missing module on the drone
 * the module pl2303 must be installed when using the usb port for a stm32 target
 * otherwise the usb library in ardrone_indoor_commons returns a errno 6 (no such device or address found)
 *
 * Julien.
 */


#ifdef TEST_USB

void test_usb_main(char *port)
{
	char stm[COMM_MESSAGE_SIZE];

	if (port == 0){
		port = USB_PORT_NAME;
	}

	usb_init(port, B9600, 0, 1);	// set speed to 9600 bps, 8n1 (no parity)
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
		stm[0] = 0;
		// is blocking, react on timeout
	   usb_read(stm, COMM_MESSAGE_SIZE);
	   printf("Stm response: %c", stm[0]);
	   stm[0] = 0;
	}
}

#endif
