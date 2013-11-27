#include "gui.h"

#ifdef GUI_ON

#include <stdio.h>
#include <stdlib.h>
 

//FIXME
#define DEST_IP "192.168.1.1"

char msg_is_connected[] = "The server is connected to the devices";
char msg_is_disconnected[] = "The server is disconnected from the devices";



gui_t *gui = NULL;
 
gui_t *get_gui()
{
  return gui;
}

void button_connect_callback()
{
	if (gui->is_connected == STATE_DISCONNECTED){
	   // server to drone via usb
	   #ifdef UDP_ON
		  message_sent_id = COMM_MESSAGE_INIT_ID;
		 // udp_send_char(DEST_IP, message_sent_id);
	   #endif
	   // server to stm32 via usb
	   #ifdef USB_ON
	   #endif
	}else if (gui->is_connected == STATE_CONNECTED){
		//message_sent_id = COMM_MESSAGE_EXIT_ID;
		//udp_send_char(DEST_IP, message_sent_id);
	}
}

void button_onoff_callback()
{
   #ifdef UDP_ON
    //  message_sent_id = COMM_MESSAGE_SYNC_ID;
    //  udp_send_char(DEST_IP, message_sent_id);
   #endif
}

#ifdef TEST_COMM
void on_message_received(char *message) 
{
   gtk_label_set_text(gui->text_test_listen, message);
}

void on_message_sent(char *message) 
{
   gtk_label_set_text(gui->text_test_send, message);
}
#endif

static void button_listen_callback( GtkWidget *widget, gpointer   data )
{
   #ifdef UDP_ON
      if (is_udp_listening == UDP_LISTEN_OFF){
         // unlock the server communication thread processing
         is_udp_listening = UDP_LISTEN_ON;
         // update the button label
         gtk_button_set_label(widget, "Stop Listening");
      }else{
         // block the server communication thread processing
         is_udp_listening = UDP_LISTEN_OFF;
         // update the button label
         gtk_button_set_label(widget, "Start Listening");
      }
   #endif
}

static void button_send_callback( GtkWidget *widget, gpointer   data )
{
   #ifdef UDP_ON
      if (is_udp_sending == UDP_SEND_OFF){
         // unlock the server communication thread processing
         is_udp_sending = UDP_SEND_ON;
         // update the button label
         gtk_button_set_label(widget, "Stop Sending");
      }else{
         // block the server communication thread processing
         is_udp_sending = UDP_SEND_OFF;
         // update the button label
         gtk_button_set_label(widget, "Start Sending");
      } 
   #endif
}
 
static void on_destroy(GtkWidget *widget, gpointer data)
{
  vp_os_free(gui);
  gtk_main_quit();
}

void createMainBox()
{
   //vbox for having same Vertical size for each element
   // 10 is the space between two consecutive elements
   gui->box_main = gtk_vbox_new(FALSE, 10); 
   
   // adds the camera image to the main box
   gui->cam = gtk_image_new();
   gtk_box_pack_start(GTK_BOX(gui->box_main), gui->cam, FALSE, TRUE, 0);
   
   // initializes action buttons
   gui->button_connect = gtk_button_new_with_label("Connect");
   g_signal_connect (gui->button_connect, "clicked", G_CALLBACK (button_connect_callback), NULL);

   gui->button_onoff = gtk_button_new_with_label("Start");
   g_signal_connect (gui->button_onoff, "clicked", G_CALLBACK (button_onoff_callback), NULL);
   gtk_widget_set_sensitive(gui->button_onoff, FALSE); // only on a connect button click can enable the onoff

   // add the action buttons to the box
   gtk_box_pack_start(GTK_BOX(gui->box_main), gui->button_connect, TRUE, TRUE, 0);
   gtk_box_pack_end(GTK_BOX(gui->box_main), gui->button_onoff, TRUE, TRUE, 0);
}

void createTestBox()
{
   #ifdef TEST_COMM
      /*gui->box_test = gtk_vbox_new(FALSE, 10);
      // send testing
      // create the box containing the textbox and the scrollbar
      //GtkWidget *text_test_send_hbox = gtk_hbox_new(FALSE, 10);
      //GtkAdjustment *vadj = gtk_adjustment_new(0, 0, )
      gui->text_test_send = gtk_text_view_new("No send");

      //GtkWidget *vscrollbar = gtk_vscrollbar_new((GtkText*)(gui->text_test_send)->vadj); //GTK_TEXT 
      //gtk_box_pack_start(GTK_BOX(text_test_send_hbox), gui->text_test_send, FALSE, FALSE, 0);
      //gtk_box_pack_end(GTK_BOX(text_test_send_hbox), vscrollbar, FALSE, FALSE, 0);
      // populate the box including the box with the textbox and the scrollbar
      gtk_box_pack_start(GTK_BOX(gui->box_test), gui->text_test_send, TRUE, TRUE, 0);
      // listen testing
      // create the box containing the textbox and the scrollbar
      //GtkWidget *text_test_listen_hbox = gtk_hbox_new(FALSE, 10);
      gui->text_test_listen = gtk_text_view_new("No receive");
      
      //gtk_text_view_

      //GtkWidget *vscrollbar2 = gtk_vscrollbar_new(GTK_TEXT (gui->text_test_listen)->vadj);
      //gtk_box_pack_start(GTK_BOX(text_test_listen_hbox), gui->text_test_listen, FALSE, FALSE, 0);
      //gtk_box_pack_end(GTK_BOX(text_test_listen_hbox), vscrollbar, FALSE, FALSE, 0);
      // populate the box including the box with the textbox and the scrollbar
      gtk_box_pack_end(GTK_BOX(gui->box_test), gui->text_test_listen, TRUE, TRUE, 0);  */
   #endif
}

void createBeaconsBox()
{
   gui->box_beacons = gtk_vbox_new(FALSE, 10);
   
   int beacon_id = 0;
   for (; beacon_id < NUM_BEACONS; beacon_id++)
   {
      // init the content of the labels per beacon
      gchar label_text[10];
      sprintf(label_text, "Beacon %d", beacon_id);
      gui->label_beacon_ids[beacon_id] = gtk_label_new(label_text);
      gui->label_beacon_timevals[beacon_id] = gtk_label_new("none");
      // add labels to the box for beacons
      gtk_box_pack_start(GTK_BOX(gui->box_beacons), gui->label_beacon_ids[beacon_id], TRUE, TRUE, 0);
      if (beacon_id < NUM_BEACONS-1)
      {
         gtk_box_pack_start(GTK_BOX(gui->box_beacons), gui->label_beacon_timevals[beacon_id], TRUE, TRUE, 0);
      }
   }
   gtk_box_pack_end(GTK_BOX(gui->box_beacons), gui->label_beacon_timevals[beacon_id-1], TRUE, TRUE, 0);
}

void createDroneBox()
{
   gui->box_drone_pos = gtk_vbox_new(FALSE, 5);

   // X axis
   gui->label_drone_pos_ids[0] = gtk_label_new("X ");
   // Y axis 
   gui->label_drone_pos_ids[1] = gtk_label_new("Y ");
   // Z axis 
   gui->label_drone_pos_ids[2] = gtk_label_new("Z ");
   int pos_id = 0;
   for (; pos_id < NUM_COORDINATES; pos_id++)
   {
      // add label ids to the container
      gtk_box_pack_start(GTK_BOX(gui->box_drone_pos), gui->label_drone_pos_ids[pos_id], TRUE, TRUE, 0);
      // init values to default
      gui->label_drone_pos_values[pos_id] = gtk_label_new("none");
      if (pos_id < NUM_COORDINATES-1)
         gtk_box_pack_start(GTK_BOX(gui->box_drone_pos), gui->label_drone_pos_values[pos_id], TRUE, TRUE, 0);
   }
   gtk_box_pack_end(GTK_BOX(gui->box_drone_pos), gui->label_drone_pos_values[pos_id-1], TRUE, TRUE, 0);
}
 
void init_gui(int argc, char **argv)
{
   gui = vp_os_malloc(sizeof (gui_t));

   // what is this function ? Undefined reference if uncommented
   //g_thread_init(NULL);
   gdk_threads_init();
   gtk_init(&argc, &argv);

   gui->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   // callback on window destroy event
   g_signal_connect(G_OBJECT(gui->window), "destroy", G_CALLBACK(on_destroy), NULL);
  
   gui->box_window = gtk_hbox_new(FALSE, 15);
   
   createMainBox();
   #ifdef TEST_COMM
      createTestBox();
   #endif 
   createBeaconsBox();
   createDroneBox();
 
   gtk_box_pack_start(GTK_BOX(gui->box_window), gui->box_main, TRUE, TRUE, 0);
   #ifdef TEST_COMM
      gtk_box_pack_start(GTK_BOX(gui->box_window), gui->box_test, TRUE, TRUE, 0);
   #endif
   gtk_box_pack_start(GTK_BOX(gui->box_window), gui->box_beacons, TRUE, TRUE, 0);
   gtk_box_pack_end(GTK_BOX(gui->box_window), gui->box_drone_pos, TRUE, TRUE, 0);

   // pack all the window box into the main window
   gtk_container_add (GTK_CONTAINER (gui->window), gui->box_window);
   
   // show all elements of the window
   gtk_widget_show_all(gui->window);
}

#endif // ifdef GUI_ON
