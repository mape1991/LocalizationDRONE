#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "com/commons_comm.h"
      
#ifdef UDP_COMM_ON
   #include "com/udp_comm.h"

   #ifdef TEST_DEMO_COMM_LOCAL
      char message[UDP_MESSAGE_SERVER_SIZE];
      char message_send_enable = 0;
      char message_sent_id[UDP_MESSAGE_DRONE_SIZE];
      char message_sync_count = 0;
   #endif
/*
   DEFINE_THREAD_ROUTINE(udp_listen_comm, data)
   {
      // the server can listen (port 7000)
      if (is_udp_listening)
      {
         #ifdef TEST_DEMO_COMM_LOCAL
            udp_listen_once(message, UDP_MESSAGE_SERVER_SIZE);
            if (message == UDP_MESSAGE_SERVER_INIT_ID)
            {
               printf("message init received\n");
               strcpy(message_sent_id, UDP_MESSAGE_DRONE_INIT_ID);
               message_send_enable = UDP_SEND_ON;
            }
            else if (message == UDP_MESSAGE_SERVER_SYNC_ID)
            {
               message_sync_count++;
               printf("message sync %d received\n", message_sync_count);
               if (message_sync_count < UDP_MESSAGE_SYNC_COUNT){
                  strcpy(message_sent_id, UDP_MESSAGE_DRONE_SYNC_ID);
                  // we exit the demo by an exit message after a certain number of sync messages
               }else{
                  strcpy(message_sent_id, UDP_MESSAGE_DRONE_EXIT_ID);
               }
               message_send_enable = UDP_SEND_ON;
            }
            else if (message == UDP_MESSAGE_SERVER_EXIT_ID)
            {
               printf("message exit received\n");
               JOIN_THREAD(udp_send_comm);
               JOIN_THREAD(udp_listen_comm);
            }
         #else
            // TODO
         #endif
      }
   }
   
   DEFINE_THREAD_ROUTINE(udp_send_comm, data)
   {   
      // and the server can send at the meantime (port 7001)
      if (is_udp_sending)
      {
         #ifdef TEST_DEMO_COMM_LOCAL
            if (message_send_enable)
            {
               udp_send_char(DEST_IP, message_sent_id);
               message_send_enable = UDP_SEND_OFF; // send once
            }
         #endif
      }
   }*/
#endif

int main(int argc, char ** argv){
	
   #ifdef TEST_DEMO_COMM_LOCAL
      printf("demo program launched\n\n");
      
      /*START_THREAD(udp_listen_comm, NULL);
      START_THREAD(udp_send_comm, NULL);*/
      
      
      
      // wait for listen
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
      
   #endif
   return 0;
}
