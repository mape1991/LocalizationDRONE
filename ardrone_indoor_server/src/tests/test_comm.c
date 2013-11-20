



#include "test_comm.h"


int test_comm_main(int argc, char** argv)
{
   printf("demo program launched\n\n");
   is_udp_listening = UDP_LISTEN_ON;
   is_udp_sending = UDP_SEND_ON;
   // send an init message first
   // the drone will receive it and reply (and so on for the next steps)
   message_send_enable = UDP_SEND_ON;

   #ifdef USB_ON
      usb_init(USB_PORT_NAME);

      is_usb_reading = USB_READING_ON;
   #endif

   return ardrone_tool_main(argc, argv);
}
