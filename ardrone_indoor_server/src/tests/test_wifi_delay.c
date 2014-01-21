#include "test_wifi_delay.h"

#ifdef TEST_WIFI_DELAY

char message[COMM_MESSAGE_SIZE];
char message_send_enable = 0;
char message_sent_id = COMM_MESSAGE_INIT_ID;
char message_sync_count = 0;
double time1 = 0.0;
double time2 = 0.0;

void test_wifi_delay_udp_send()
{
   sleep(1);
   message_sent_id = COMM_MESSAGE_INIT_ID;
   struct timeval tim;
   gettimeofday(&tim,NULL);
   time2=tim.tv_sec*1000.0+(tim.tv_usec/1000.0);
   printf("time2: %f\n",time2);
   udp_send_char(DEST_IP_DRONE, message_sent_id,PORT_SERVER_TO_DRONE);

   udp_listen_once(message, COMM_MESSAGE_SIZE, PORT_DRONE_TO_SERVER);
	printf("toto\n");
	if (message[0] == COMM_MESSAGE_INIT_ID)
	{
		gettimeofday(&tim,NULL);
		time1=tim.tv_sec*1000.0+(tim.tv_usec/1000.0);
		printf("time1: %f\n",time1);
		printf("init received\n");
		message[0]  = COMM_MESSAGE_NONE;
		printf("%.6lf ms elapsed\n", time1-time2);
		udp_close_socket();
		return;
	}
         else printf("merde");
   sleep(3);
}



void test_wifi_delay_main()
{
   printf("demo program launched\n\n");

   udp_open_socket();
}

#endif
