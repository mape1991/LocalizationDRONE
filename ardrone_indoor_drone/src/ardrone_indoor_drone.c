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

#include "com/udp_comm.h"
#include "com/commons_comm.h"

int main(int argc, char ** argv){
	
   #ifdef TEST_DEMO_COMM_LOCAL
      char message[1];

      printf("demo program launched\n\n");

      while (1) {

         udp_listen_once(message, 1);

         if (message[0] == 'I'){
            printf("message init received\n");
            udp_send(DEST_IP, "init", 5);
         }
         udp_listen_once(message, 1);
         if (message[0] == 'S'){
            printf("message sync received\n");
            udp_send(DEST_IP, "sync", 5);
         }
         udp_listen_once(message, 1);
         if (message[0] == 'X'){
            printf("message exit received\n");
            udp_send(DEST_IP, "exit", 5);
            exit(-1);
         }
      }
   #endif
   return 0;
}
