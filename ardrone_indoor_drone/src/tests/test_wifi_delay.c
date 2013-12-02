#include "test_wifi_delay.h"
#include <stdio.h>
#include <string.h>

#ifdef TEST_WIFI_DELAY

void test_wifi_delay_main()
{
   char message[COMM_MESSAGE_SIZE];
   char message_sent_id;
   printf("demo program launched\n\n");
   int i =1;
   udp_open_socket();
   //while(i){
       udp_listen_once(message, COMM_MESSAGE_SIZE,PORT_SERVER_TO_DRONE);
       printf("haha %c\n",message[0]);
          if (message[0] == COMM_MESSAGE_INIT_ID){
            
             printf("message init received\n");
             message_sent_id = COMM_MESSAGE_INIT_ID;
             udp_respond_char(message_sent_id, PORT_DRONE_TO_SERVER);
             printf("message sent\n");
             i=0;
          }

   udp_close_socket();
}

#endif

