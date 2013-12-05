#include "test_comm.h"

#ifdef TEST_COMM

void test_comm_main()
{
   char message[COMM_MESSAGE_SIZE];

   printf("demo program launched\n\n");
   udp_open_socket();
   
   do {
      udp_listen_once(message, COMM_MESSAGE_SIZE, PORT_SERVER_TO_DRONE);
      if (message[0] == COMM_MESSAGE_INIT_ID){
         printf("message init received\n");
         udp_respond_char(COMM_MESSAGE_INIT_ID, PORT_DRONE_TO_SERVER);
      }
      else if (message[0] == COMM_MESSAGE_SYNC_ID){
         printf("message sync received\n");
         udp_respond_char(COMM_MESSAGE_SYNC_ID, PORT_DRONE_TO_SERVER);
      }
      else if (message[0] == COMM_MESSAGE_EXIT_ID){
         printf("message quit received\n");
         udp_respond_char(COMM_MESSAGE_EXIT_ID, PORT_DRONE_TO_SERVER);
         break;
      }
      message[0] = COMM_MESSAGE_NONE;
   } while (1);
   udp_close_socket();
}

#endif
