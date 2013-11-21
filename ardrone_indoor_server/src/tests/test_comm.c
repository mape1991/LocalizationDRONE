#include "test_comm.h"

void test_comm_thread_udp_read()
{
   while(is_udp_listening){
      udp_listen_once(message, UDP_MESSAGE_DRONE_SIZE);
      if (strcmp(message, UDP_MESSAGE_DRONE_INIT_ID) == 0)
      {
         printf("init received\n");
         strcpy(message, "");
         message_sent_id = COMM_MESSAGE_SYNC_ID;
         message_send_enable = UDP_SEND_ON;
      }
      else if (strcmp(message, UDP_MESSAGE_DRONE_SYNC_ID) == 0)
      {
         strcpy(message, "");
         // initial demo
         message_sync_count++;
         printf("sync %d received\n", message_sync_count);
         if (message_sync_count <= UDP_MESSAGE_SYNC_COUNT){
            message_sent_id = COMM_MESSAGE_SYNC_ID;
            // we exit the demo by an exit message after a certain number of sync messages
         }else{
            message_sent_id = COMM_MESSAGE_EXIT_ID;
         }
         message_send_enable = UDP_SEND_ON;
      }
   }
}


void test_comm_thread_write()
{
   sleep(1);
   
   message_sent_id = COMM_MESSAGE_INIT_ID;
   udp_send_char(DEST_IP, message_sent_id);

   printf("stm32 write msg %c\n", message_sent_id);
   usb_write_char(message_sent_id);

   sleep(1);

   message_sent_id = COMM_MESSAGE_SYNC_ID;
   int i = 0;
   for(i = 0; i < UDP_MESSAGE_SYNC_COUNT; i++){   
      udp_send_char(DEST_IP, message_sent_id);
      #ifdef USB_ON
         printf("stm32 write msg %c\n", message_sent_id);
         usb_write_char(message_sent_id);
      #endif
      sleep(1);
   }

   message_sent_id = COMM_MESSAGE_EXIT_ID;
   udp_send_char(DEST_IP, message_sent_id);
   #ifdef USB_ON
      printf("stm32 write msg %c\n", message_sent_id);
      usb_write_char(message_sent_id);
   #endif
   sleep(1);
   
   // put the stm32 back in start mode
   #ifdef USB_ON   
      message_sent_id = COMM_MESSAGE_INIT_ID;
      printf("stm32 write msg %c\n", message_sent_id);
      usb_write_char(message_sent_id);
      sleep(1);
      message_sent_id = COMM_MESSAGE_SYNC_ID;
      printf("stm32 write msg %c\n", message_sent_id);
      usb_write_char(message_sent_id);
      
      is_usb_reading = USB_READING_OFF;
   #endif

   is_udp_sending = UDP_SEND_OFF;
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

int test_comm_main(int argc, char** argv)
{
   printf("demo program launched\n\n");
   is_udp_listening = UDP_LISTEN_ON;
   is_udp_sending = UDP_SEND_ON;
   // send an init message first
   // the drone will receive it and reply (and so on for the next steps)
   message_send_enable = UDP_SEND_ON;

   usb_init(USB_PORT_NAME);
   is_usb_reading = USB_READING_ON;

   return ardrone_tool_main(argc, argv);
}
