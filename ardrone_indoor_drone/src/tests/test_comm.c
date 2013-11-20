#include "test_comm.h"

void test_comm_main()
{
   char message[UDP_MESSAGE_SERVER_SIZE];
   char message_sent_id;

   printf("demo program launched\n\n");
   udp_listen_once(message, UDP_MESSAGE_SERVER_SIZE);
   while (1){
      if (message[0] == 'I'){
         printf("message init received\n");
         strcpy(message_sent_id, UDP_MESSAGE_DRONE_INIT_ID);
         udp_send(DEST_IP, message_sent_id, UDP_MESSAGE_DRONE_SIZE);
         udp_listen_once(message, UDP_MESSAGE_SERVER_SIZE);
      }
      else if (message[0] == 'S'){
         printf("message sync received\n");
         strcpy(message_sent_id, UDP_MESSAGE_DRONE_SYNC_ID);
         udp_send(DEST_IP, message_sent_id, UDP_MESSAGE_DRONE_SIZE);
         udp_listen_once(message, UDP_MESSAGE_SERVER_SIZE);

      }
      else if (message[0] == 'X'){
               printf("message quit received\n");
               break;
      }
   }
}
