#include "test_comm.h"


#ifdef TEST_COMM

char message[COMM_MESSAGE_SIZE];
char message_send_enable = UDP_SEND_OFF;
char message_sent_id = COMM_MESSAGE_INIT_ID;
char message_sync_count = 0;

// listen the incoming port, does not act upon other threads
// what makes the difference with the test_gui is the use of messages
// four characters long instead of single character long messages...
void test_comm_thread_udp_read()
{
   while(is_udp_listening){
   	udp_listen_once(message, COMM_MESSAGE_SIZE, PORT_DRONE_TO_SERVER);
      // init
      if (message[0] == COMM_MESSAGE_INIT_ID)
      {
         printf("init received\n");
      }
      // sync
      else if (message[0] == COMM_MESSAGE_SYNC_ID)
      {
         message_sync_count++;
         printf("sync %d received\n", message_sync_count);
      }

      message[0] = COMM_MESSAGE_NONE;
   }
}

// only send via usb and udp
void test_comm_thread_send()
{
   sleep(1);
   // send I
   message_sent_id = COMM_MESSAGE_INIT_ID;
   udp_send_char(DEST_IP_DRONE, message_sent_id, PORT_SERVER_TO_DRONE);
   #ifdef USB_ON
   	   printf("stm32 write msg %c\n", message_sent_id);
   	   usb_write_char(message_sent_id);
   #endif
   sleep(3);
   // send S
   message_sent_id = COMM_MESSAGE_SYNC_ID;
   int i = 0;
   for(i = 0; i < COMM_TEST_COMM_SYNC_COUNT; i++){
      udp_send_char(DEST_IP_DRONE, message_sent_id, PORT_SERVER_TO_DRONE);
      #ifdef USB_ON
         printf("stm32 write msg %c\n", message_sent_id);
         usb_write_char(message_sent_id);
      #endif
      sleep(1);
   }
   // send X
   message_sent_id = COMM_MESSAGE_EXIT_ID;
   udp_send_char(DEST_IP_DRONE, message_sent_id, PORT_SERVER_TO_DRONE);
   #ifdef USB_ON
      printf("stm32 write msg %c\n", message_sent_id);
      usb_write_char(message_sent_id);
   #endif
   sleep(1);
   // send I via usb : put the stm32 back in start mode
   #ifdef USB_ON   
      message_sent_id = COMM_MESSAGE_INIT_ID;
      printf("stm32 write msg %c\n", message_sent_id);
      usb_write_char(message_sent_id);
      sleep(1);
      message_sent_id = COMM_MESSAGE_SYNC_ID;
      printf("stm32 write msg %c\n", message_sent_id);
      usb_write_char(message_sent_id);
      // stop usb reading thread
      is_usb_reading = USB_READING_OFF;
   #endif
   // close sockets
   udp_close_socket();
   usb_close();
   // set to OFF for avoiding test_comm_thread_udp_read looping
   is_udp_listening = UDP_LISTEN_OFF;
}

void test_comm_thread_usb_read()
{
   char buffer[USB_BUFFER_MAX_SIZE];
   while (is_usb_reading) {
      int n = usb_read(buffer, 1);
      if (n > 0){
         printf("stm32 read msg %s\n", buffer);
         strcpy(buffer, "");
         sleep(1);
      }
   }
}

void test_comm_main()
{
   printf("demo program launched\n\n");
   // activate the udp comm
   udp_open_socket();
   is_udp_listening = UDP_LISTEN_ON;
   // activate the usb comm
   usb_init(USB_PORT_NAME);
   is_usb_reading = USB_READING_ON;
}

#endif
