#include "test_gui.h"

#ifdef TEST_GUI

char message[COMM_MESSAGE_SIZE];

char message_send_enable = 0;
char message_send_id = COMM_MESSAGE_INIT_ID;
int message_sync_count = 0;
sem_t message_sema;

void test_gui_thread_udp_read()
{
	char text_label[GUI_MAX_LABEL_SIZE];

   while(is_udp_listening){
	  udp_listen_once(message, COMM_MESSAGE_SIZE, PORT_DRONE_TO_SERVER);
	  // tracing
	  if (message[0] != 0)
	  	  printf("udp message %c\n", message[0]);
	  // dispatch decisions through different cases
	  switch(message[0]) {
		  case COMM_MESSAGE_INIT_ID :
				  printf("udp init received\n");
				  gtk_label_set_text(get_gui()->text_drone_state, GUI_DRONE_HINT_INIT);
			  break;
		  case COMM_MESSAGE_SYNC_ID :
				  printf("udp sync %d received\n", message_sync_count);
				  // display the usual message including the sync message id (incremented when the user clicks on Get Position)
				  strcpy(text_label, "");
				  sprintf(text_label, "%s %d", GUI_DRONE_HINT_SYNC, message_sync_count);
				  gtk_label_set_text(get_gui()->text_drone_state, text_label);
			  break;
		  case COMM_MESSAGE_EXIT_ID :
				  printf("udp exit received\n");
				  gtk_label_set_text(get_gui()->text_drone_state, GUI_DRONE_HINT_EXIT);
				  // deactivate threads loops
				  is_udp_listening = 0;
				  is_udp_sending = 0;

				  udp_close_socket();
				break;
		  default:
			  break;
	  }
	  // empty message for new loop
	  message[0] = 0;
   }
}

char x = 0;
// inputs provided by user actions (buttons rule the message enable and the message id
void test_gui_thread_send()
{
	char text_label[GUI_MAX_LABEL_SIZE];

	while (is_udp_sending) {
		/*if (x != message_send_enable){
			x = message_send_enable;
			printf("%c\n", x);
		}*/
		//printf("%c\n", message_send_enable);

	    sem_wait(&message_sema);       /* down semaphore */

			udp_send_char(DEST_IP_DRONE, message_send_id, PORT_SERVER_TO_DRONE);
			#ifdef USB_ON
				// FIXME: do we send the usb packet at the meantime ?
				printf("stm32 write msg %c\n", message_send_id);
				usb_write_char(message_send_id);
			#endif
			// stop sending (send only one message)
			//message_send_enable = 0;
			// display the server state
			switch (message_send_id){
				case COMM_MESSAGE_INIT_ID :
					printf("udp init sent\n");
					gtk_label_set_text(get_gui()->text_server_state, GUI_SERVER_HINT_INIT);
					break;
				case COMM_MESSAGE_SYNC_ID :
					// count the number of sync
					message_sync_count++;
					printf("udp sync %d sent\n", message_sync_count);
					// gui labeling
					strcpy(text_label, "");
					sprintf(text_label, "%s %d", GUI_SERVER_HINT_SYNC, message_sync_count);
					gtk_label_set_text(get_gui()->text_server_state, text_label);
					break;
				case COMM_MESSAGE_EXIT_ID :
					printf("udp exit sent\n");
					gtk_label_set_text(get_gui()->text_server_state, GUI_SERVER_HINT_EXIT);
					break;
				default:
					break;
			}
	   }
	}
}

#ifdef USB_ON

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
					 is_usb_reading = 0;
					 usb_close();
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

#endif

void test_gui_main(int argc, char **argv)
{
   printf("program with ui launched\n\n");
   // activate the udp comm
   udp_open_socket();

   is_udp_listening = 1;
   is_udp_sending = 1;
   // activate the usb comm
	#ifdef USB_ON
   	usb_init(USB_PORT_NAME);
   	is_usb_reading = 1;
	#endif

   sem_init(&message_sema, 0, 0);      /* initialize mutex to 1 - binary semaphore */
   	                                 /* second param = 0 - semaphore is local */

   // initialize the gui
	init_gui(argc, argv);
	// put initial states in labels
	gtk_label_set_text(get_gui()->text_server_state, GUI_SERVER_HINT_EXIT);
	gtk_label_set_text(get_gui()->text_drone_state, GUI_DRONE_HINT_EXIT);
	gtk_label_set_text(get_gui()->text_controller_state, GUI_CONTROLLER_HINT_EXIT);
}

#endif
