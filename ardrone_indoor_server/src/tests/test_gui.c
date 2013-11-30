#include "test_gui.h"

#ifdef TEST_GUI

char message[COMM_MESSAGE_SIZE];
char message_send_enable = 0;
char message_send_id = COMM_MESSAGE_INIT_ID;
int message_sync_count = 0;

char msg_is_connected[] = "The server is connected to the devices";
char msg_is_disconnected[] = "The server is disconnected from the devices";

void test_gui_thread_udp_read()
{
	char text_label[GUI_MAX_LABEL_SIZE];

   while(is_udp_listening){
	  udp_listen_once(message, COMM_MESSAGE_SIZE, PORT_DRONE_TO_SERVER);
	  // tracing
	  if (message[0] != 0)
	  	  printf("udp message %c\n", message[0]);
	  // dispatch decisions through different cases
	  switch(message[0])
	  {
	  case COMM_MESSAGE_INIT_ID :
		  	  gtk_label_set_text(get_gui()->text_drone_state, GUI_DRONE_HINT_INIT);
		  break;
	  case COMM_MESSAGE_SYNC_ID :
		  	  // display the usual message including the sync message id (incremented when the user clicks on Get Position)
		  	  strcpy(text_label, "");
		  	  sprintf(text_label, "%s %d", GUI_DRONE_HINT_SYNC, message_sync_count);
		  	  gtk_label_set_text(get_gui()->text_drone_state, text_label);
	 	  break;
	  case COMM_MESSAGE_EXIT_ID :
		  	  gtk_label_set_text(get_gui()->text_drone_state, GUI_DRONE_HINT_EXIT);
		  	  // deactivate threads loops
			  is_udp_listening = UDP_LISTEN_OFF;
		     is_udp_sending = UDP_SEND_OFF;
		  	break;
	  default:
	 	  break;
	  }
	  // empty message for new loop
	  message[0] = 0;
   }
}

// inputs provided by user actions (buttons rule the message enable and the message id
void test_gui_thread_send()
{
	while (is_udp_sending) {
	   if (message_send_enable) {
			udp_send_char(DEST_IP_DRONE, message_send_id, PORT_SERVER_TO_DRONE);
		// FIXME: do we send the usb packet at the meantime ?
			printf("stm32 write msg %c\n", message_send_id);
			usb_write_char(message_send_id);
			// stop sending (send only one message)
			message_send_enable = UDP_SEND_OFF;
			// count the number of sync
			if (message_send_id == COMM_MESSAGE_SYNC_ID){
				message_sync_count++;
			}
	   }
	}
}

void test_gui_thread_usb_read()
{
   char buffer[USB_BUFFER_MAX_SIZE];
   char text_label[GUI_MAX_LABEL_SIZE];
   while (is_usb_reading) {
	  int n = usb_read(buffer, 1);
	  if (n > 0){
		 printf("stm32 read msg %s\n", buffer);
		 // dispatch the decision regarding the incoming character
		 switch(buffer[0])
		 {
		 case COMM_MESSAGE_INIT_ID :
			 	 gtk_label_set_text(get_gui()->text_controller_state, GUI_CONTROLLER_HINT_INIT);
			 break;
		 case COMM_MESSAGE_SYNC_ID :
		  	  	 // display the usual message including the sync message id (incremented when the user clicks on Get Position)
			 	 strcpy(text_label, "");
			  	 sprintf(text_label, "%s %d", GUI_CONTROLLER_HINT_SYNC, message_sync_count);
			  	 gtk_label_set_text(get_gui()->text_controller_state, text_label);
		 	 break;
		 case COMM_MESSAGE_EXIT_ID :
			    // deactivate threads loops
			 	 gtk_label_set_text(get_gui()->text_controller_state,GUI_CONTROLLER_HINT_EXIT);
			 	 is_usb_reading = USB_READING_OFF;
			 break;
		 default:
			 break;
		 }
		 // empty buffer for further use
		 strcpy(buffer, "");
		 sleep(1);
	  }
   }
}

void test_gui_main()
{
   printf("program with ui launched\n\n");
   // activate the udp comm
   udp_open_socket();
   message_send_enable = UDP_SEND_OFF; // avoid sending when initializing
   // activate the usb comm
   usb_init(USB_PORT_NAME);
}

#endif
