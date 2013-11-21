#include "test_wifi_delay.h"

#ifdef TEST_WIFI_DELAY

#define DEST_IP "192.168.1.1"

char message[UDP_MESSAGE_DRONE_SIZE];
char message_send_enable = 0;
char message_sent_id = COMM_MESSAGE_INIT_ID;
char message_sync_count = 0;
double time1 = 0;
double time2 = 0;

void test_wifi_delay_udp_read()
{
      struct timeval tim;
      gettimeofday(&tim,NULL);
      double t1=tim.tv_sec+(tim.tv_usec/1000000.0);
      udp_listen_once(message, UDP_MESSAGE_DRONE_SIZE);
      if (strcmp(message, UDP_MESSAGE_DRONE_INIT_ID) == 0)
      {
         printf("init received\n");
         strcpy(message, "");
         time1 = t1;
         printf("attentionnnnnnnnnnnnn  %.6lf seconds elapsed\n", time1-time2);
         return;
      }
      /*else if (strcmp(message, UDP_MESSAGE_DRONE_SYNC_ID) == 0)
      {
         strcpy(message, "");
         // initial demo
         message_sync_count++;
         printf("sync %d received\n", message_sync_count);
         if (message_sync_count <= UDP_MESSAGE_SYNC_COUNT){
            message_sent_id = UDP_MESSAGE_SERVER_SYNC_ID;
            // we exit the demo by an exit message after a certain number of sync messages
         }else{
            message_sent_id = UDP_MESSAGE_SERVER_EXIT_ID;
         }
         message_send_enable = UDP_SEND_ON;
      }*/
      
      
   
}


void test_wifi_delay_udp_send()
{
   sleep(1);
   message_sent_id = COMM_MESSAGE_INIT_ID;
   struct timeval tim;
   gettimeofday(&tim,NULL);
   double t2=tim.tv_sec+(tim.tv_usec/1000000.0);
   udp_send_char(DEST_IP, message_sent_id);

   sleep(1);
/*
   message_sent_id = UDP_MESSAGE_SERVER_SYNC_ID;
   int i = 0;
   for(i = 0; i < UDP_MESSAGE_SYNC_COUNT; i++){
      udp_send_char(DEST_IP, message_sent_id);
      sleep(1);
   }

   message_sent_id = UDP_MESSAGE_SERVER_EXIT_ID;
   udp_send_char(DEST_IP, message_sent_id);

   sleep(1);*/

   is_udp_sending = UDP_SEND_OFF;
   is_udp_listening = UDP_LISTEN_OFF;
   time2 = t2; 
}



void test_wifi_delay_main()
{
   printf("demo program launched\n\n");
   is_udp_listening = UDP_LISTEN_ON;
   is_udp_sending = UDP_SEND_ON;
   // send an init message first
   // the drone will receive it and reply (and so on for the next steps)
   message_send_enable = UDP_SEND_ON;
}

#endif

