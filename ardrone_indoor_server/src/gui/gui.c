#include "gui.h"

#ifdef GUI_ON

#include <stdio.h>
#include <stdlib.h>
 
gui_t *gui = NULL;
 
gui_t *get_gui()
{
  return gui;
}
 
/* If the drone is landed, only start is clickable,
   if the drone is in the air, only stop is clickable
*/
/*static void toggleButtonsState(void)
{
  gboolean start_state = gtk_widget_get_sensitive(gui->button_start);
 
  gtk_widget_set_sensitive(gui->button_start, !start_state);
  gtk_widget_set_sensitive(gui->button_stop, start_state);
}*/
 
static void buttons_callback( GtkWidget *widget, gpointer   data )
{
    // FIXME: make the drone start
}

void button_init_callback() 
{
   char msg[1];
   sprintf(msg, "%c", 'I');
   #ifdef DEBUG_ON
      printf("    Button init pressed : send init signal : %s\n", msg);
   #endif
   udp_send(DEST_IP, msg, 1);
}

void button_sync_callback() 
{
   char msg[1];
   sprintf(msg, "%c", 'S');
   #ifdef DEBUG_ON
      printf("    Button sync pressed : send sync signal : %s\n", msg);
   #endif
   udp_send(DEST_IP, msg, 1);
}

void button_exit_callback() 
{
   char msg[1];
   sprintf(msg, "%c", 'X');
   #ifdef DEBUG_ON
      printf("    Button exit pressed : send exit signal : %s\n", msg);
   #endif
   udp_send(DEST_IP, msg, 1);
}

void on_drone_message_received(char *message) 
{
   /*
      struct timeval *tbalise = (struct timeval *) message;
      int i;
      for (i = 0; i < NUM_BEACONS; i++)
      {
         char time_value[16];
         sprintf(time_value, "%d %d", (int)tbalise[i].tv_sec, (int)tbalise[i].tv_usec);
         gtk_label_set_text(gui->label_beacon_timevals[i], time_value);
      }*/
}

static void button_listen_callback( GtkWidget *widget, gpointer   data )
{
   #ifdef SERVER_COMM_ON
      if (is_server_listening == SERVER_LISTEN_OFF){
         // unlock the server communication thread processing
         is_server_listening = SERVER_LISTEN_ON;
         // update the button label
         gtk_button_set_label(widget, "Stop Listening");
      }else{
         // block the server communication thread processing
         is_server_listening = SERVER_LISTEN_OFF;
         // update the button label
         gtk_button_set_label(widget, "Start Listening");
      }
   #endif
}

static void button_send_callback( GtkWidget *widget, gpointer   data )
{
   #ifdef SERVER_COMM_ON
      if (is_server_sending == SERVER_SEND_OFF){
         // unlock the server communication thread processing
         is_server_sending = SERVER_SEND_ON;
         // update the button label
         gtk_button_set_label(widget, "Stop Sending");
      }else{
         // block the server communication thread processing
         is_server_sending = SERVER_SEND_OFF;
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
   gui->button_start = gtk_button_new_with_label("Init");
   g_signal_connect (gui->button_start, "clicked", G_CALLBACK (button_init_callback), NULL);

   gui->button_sync = gtk_button_new_with_label("Sync");
   g_signal_connect (gui->button_sync, "clicked", G_CALLBACK (button_sync_callback), NULL);

   gui->button_exit = gtk_button_new_with_label("Exit");
   g_signal_connect (gui->button_exit, "clicked", G_CALLBACK (button_exit_callback), NULL);

   // add the action buttons to the box
   gtk_box_pack_start(GTK_BOX(gui->box_main), gui->button_start, TRUE, TRUE, 0);
   gtk_box_pack_start(GTK_BOX(gui->box_main), gui->button_sync, TRUE, TRUE, 0);
   gtk_box_pack_start(GTK_BOX(gui->box_main), gui->button_exit, TRUE, TRUE, 0);
   
   gui->button_listen = gtk_button_new_with_label("Start Listening");
   g_signal_connect (gui->button_listen, "clicked", G_CALLBACK (button_listen_callback), NULL);

   // disable the listening if server communication disabled
   #ifndef SERVER_COMM_ON
      gtk_widget_set_sensitive(gui->button_listen, FALSE);
   #endif
   gtk_box_pack_end(GTK_BOX(gui->box_main), gui->button_listen, TRUE, TRUE, 0);
   
   // button_send
   gui->button_send = gtk_button_new_with_label("Start Sending");
   g_signal_connect (gui->button_send, "clicked", G_CALLBACK (button_send_callback), NULL);

   // disable the listening if server communication disabled
   #ifndef SERVER_COMM_ON
      gtk_widget_set_sensitive(gui->button_send, FALSE);
   #endif
   gtk_box_pack_end(GTK_BOX(gui->box_main), gui->button_send, TRUE, TRUE, 0);
}

void createTestBox()
{
   #ifdef GUI_VERSION_TEST
      gui->box_test = gtk_vbox_new(FALSE, 10);
      // send testing
      // create the box containing the textbox and the scrollbar
      //GtkWidget *text_test_send_hbox = gtk_hbox_new(FALSE, 10);
      //GtkAdjustment *vadj = gtk_adjustment_new(0, 0, )
      gui->text_test_send = gtk_text_view_new();

      //GtkWidget *vscrollbar = gtk_vscrollbar_new((GtkText*)(gui->text_test_send)->vadj); //GTK_TEXT 
      //gtk_box_pack_start(GTK_BOX(text_test_send_hbox), gui->text_test_send, FALSE, FALSE, 0);
      //gtk_box_pack_end(GTK_BOX(text_test_send_hbox), vscrollbar, FALSE, FALSE, 0);
      // populate the box including the box with the textbox and the scrollbar
      gtk_box_pack_start(GTK_BOX(gui->box_test), gui->text_test_send, FALSE, FALSE, 0);
      // listen testing
      // create the box containing the textbox and the scrollbar
      //GtkWidget *text_test_listen_hbox = gtk_hbox_new(FALSE, 10);
      gui->text_test_listen = gtk_text_view_new();
      
      //gtk_text_view_

      //GtkWidget *vscrollbar2 = gtk_vscrollbar_new(GTK_TEXT (gui->text_test_listen)->vadj);
      //gtk_box_pack_start(GTK_BOX(text_test_listen_hbox), gui->text_test_listen, FALSE, FALSE, 0);
      //gtk_box_pack_end(GTK_BOX(text_test_listen_hbox), vscrollbar, FALSE, FALSE, 0);
      // populate the box including the box with the textbox and the scrollbar
      gtk_box_pack_start(GTK_BOX(gui->box_test), gui->text_test_listen, FALSE, FALSE, 0);  
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
   #ifdef GUI_VERSION_TEST
      createTestBox();
   #endif 
   createBeaconsBox();
   createDroneBox();
 
   gtk_box_pack_start(GTK_BOX(gui->box_window), gui->box_main, TRUE, TRUE, 0);
   #ifdef GUI_VERSION_TEST
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