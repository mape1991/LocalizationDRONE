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

#include"global_config.h"

#ifdef TEST_COMM
   #include "tests/test_comm.h"
#elif defined TEST_WIFI_DELAY
   #include "tests/test_wifi_delay.h"
#elif defined TEST_FULL
	#include "tests/test_full.h"
#elif defined TEST_USB
	#include "tests/test_usb.h"
#elif defined TEST_THREAD
	#include "tests/test_thread.h"
#endif


int main(int argc, char ** argv){
	
   #ifdef TEST_COMM
      test_comm_main();
   #elif defined TEST_WIFI_DELAY
      test_wifi_delay_main();
   #elif defined TEST_FULL
	   test_full_main();
	#elif defined TEST_USB
	   if (argc >= 2)
	   	test_usb_main(argv[1]);
	   else
	   	test_usb_main(NULL);
	#elif defined TEST_THREAD
	   test_thread_main();
   #endif
  
   return 0;
}
