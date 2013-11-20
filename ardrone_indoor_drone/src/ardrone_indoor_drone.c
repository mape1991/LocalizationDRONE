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

#include "com/commons_comm.h"
      
#ifdef UDP_ON
   #include "com/udp_comm.h"
#endif

#ifdef TEST_COMM
    #include "tests/test_comm.h"
#endif

int main(int argc, char ** argv){
	
   #ifdef TEST_COMM
      test_comm_main();
   #endif
   return 0;
}
