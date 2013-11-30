#include "test_wifi_delay.h"

#ifdef TEST_WIFI_DELAY

char message[COMM_MESSAGE_SIZE];
char message_send_enable = 0;
char message_sent_id = COMM_MESSAGE_INIT_ID;
char message_sync_count = 0;
double time1 = 0;
double time2 = 0;

void test_wifi_delay_udp_read()
{
    while(is_udp_listening){
      udp_listen_once(message, COMM_MESSAGE_SIZE, PORT_DRONE_TO_SERVER);
      if (message[0] == COMM_MESSAGE_INIT_ID)
      {
         struct timeval tim;
         gettimeofday(&tim,NULL);
         double t1=tim.tv_sec*1000.0+(tim.tv_usec);
         time1 = t1;
         printf("time1: %d\n",tim.tv_usec);
         printf("init received\n");
         message[0]  = COMM_MESSAGE_NONE;
         printf("%.6lf seconds elapsed\n", time1-time2);
         return;
      }
    }
}


void test_wifi_delay_udp_send()
{
   sleep(1);
   message_sent_id = COMM_MESSAGE_INIT_ID;
   struct timeval tim;
   gettimeofday(&tim,NULL);
   double t2=tim.tv_sec*1000.0+(tim.tv_usec);
   time2 = t2;
   printf("time2: %d\n",tim.tv_usec);
   udp_send_char(DEST_IP_DRONE, message_sent_id,PORT_SERVER_TO_DRONE);

   is_udp_listening = UDP_LISTEN_OFF;

   udp_close_socket();
}



void test_wifi_delay_main()
{
   printf("demo program launched\n\n");

   udp_open_socket();
   is_udp_listening = UDP_LISTEN_ON;
}

#endif
