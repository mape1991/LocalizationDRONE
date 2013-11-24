#include "test_wifi_delay.h"

#ifdef TEST_WIFI_DELAY

#define DEST_IP "192.168.1.3"

void test_wifi_delay_main()
{
   char message[COMM_MESSAGE_SIZE];
   char message_sent_id[UDP_MESSAGE_DRONE_SIZE];
   printf("demo program launched\n\n");
   int i =1;
   //while(i){
       udp_listen_once(message, COMM_MESSAGE_SIZE,PORT_SERVER_TO_DRONE);
       printf("haha %c\n",message[0]);
          if (message[0] == COMM_MESSAGE_INIT_ID){
            
             printf("message init received\n");
             strcpy(message_sent_id, UDP_MESSAGE_DRONE_INIT_ID);
             udp_send(DEST_IP, message_sent_id, UDP_MESSAGE_DRONE_SIZE,PORT_DRONE_TO_SERVER);
             printf("finish of sending");
             i=0;
             //udp_listen_once(message, COMM_MESSAGE_SIZE);
          }
   /*
      else if (message[0] == 'S'){
         printf("message sync received\n");
         strcpy(message_sent_id, UDP_MESSAGE_DRONE_SYNC_ID);
         udp_send(DEST_IP, message_sent_id, UDP_MESSAGE_DRONE_SIZE);
         udp_listen_once(message, COMM_MESSAGE_SIZE);

      }
      else if (message[0] == 'X'){
               printf("message quit received\n");
               break;
      }*/
   //}
}

#endif

