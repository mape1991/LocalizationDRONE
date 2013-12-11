#include "test_usb.h"

#ifdef TEST_USB

void test_usb_main()
{
	char stm[COMM_MESSAGE_SIZE];
	// Serial interface init
	usb_init(USB_PORT_NAME, B9600, 0, 1);
//	   is_usb_reading = 1;
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
		usb_write_char(stm[0]);
		// is blocking, react on timeout
		stm[0] = 0;
	   usb_read(stm, COMM_MESSAGE_SIZE);
	   printf("Stm response: %c\n", stm[0]);
	   stm[0] = 0;
	}

	usb_close();
}

#endif
