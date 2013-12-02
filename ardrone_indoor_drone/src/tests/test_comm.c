#include "test_comm.h"

#ifdef TEST_COMM

void test_comm_main()
{
   char message[COMM_MESSAGE_SIZE];
   char message_sent_id;

   printf("demo program launched\n\n");
   udp_open_socket();
   
   do {
      udp_listen_once(message, COMM_MESSAGE_SIZE, PORT_SERVER_TO_DRONE);
      if (message[0] == COMM_MESSAGE_INIT_ID){
         printf("message init received\n");
         message_sent_id = COMM_MESSAGE_INIT_ID;
         udp_respond_char(message_sent_id, PORT_DRONE_TO_SERVER);
      }
      else if (message[0] == COMM_MESSAGE_SYNC_ID){
         printf("message sync received\n");
         message_sent_id = COMM_MESSAGE_SYNC_ID;
         udp_respond_char(message_sent_id, PORT_DRONE_TO_SERVER);
      }
      else if (message[0] == COMM_MESSAGE_EXIT_ID){
         printf("message quit received\n");
         break;
      }
      message[0] = COMM_MESSAGE_NONE;
   } while (1);
   udp_close_socket();
}

#endif
