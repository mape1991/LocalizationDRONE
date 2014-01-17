#include "gui.h"

#ifdef GUI_ON

#include <stdio.h>
#include <stdlib.h>

gui_t *gui = NULL;
 
gui_t *get_gui()
{
  return gui;
}

void button_disconnect_callback()
{
	#if defined(TEST_GUI) || defined(TEST_FULL)
		// connected > disconnected (exit)
		message_send_id = COMM_MESSAGE_EXIT_ID;
		gtk_widget_set_sensitive(gui->button_disconnect, FALSE);
		gtk_widget_set_sensitive(gui->button_connect, TRUE);
		// desactivate loops in the threads
		printf("button %s clicked\n", "disconnect");
		// disable getpos button
		gtk_widget_set_sensitive(gui->button_getpos, FALSE);
		// post semaphore for the thread in test_gui
		// it allows to unlock test_comm_thread_send
		sem_post(&message_sema);       /* up semaphore */
	#endif
}

void button_connect_callback()
{
	#if defined(TEST_GUI) || defined(TEST_FULL)
		// server to stm32/drone via usb/udp
		message_send_id = COMM_MESSAGE_INIT_ID;
		gtk_widget_set_sensitive(gui->button_disconnect, TRUE);
		gtk_widget_set_sensitive(gui->button_connect, FALSE);
		// activate threads loops
		#ifdef USB_ON
			is_usb_reading = 1;
		#endif
		// desactivate loops in the threads
		printf("button %s clicked\n", "connect");
		// enable getpos button
		gtk_widget_set_sensitive(gui->button_getpos, TRUE);
		// post semaphore for the thread in test_gui
		// it allows to unlock test_comm_thread_send
		sem_post(&message_sema);       /* up semaphore */
	#endif
}

void button_getpos_callback()
{
	#if defined(TEST_GUI) || defined(TEST_FULL)
		message_send_id = COMM_MESSAGE_SYNC_ID;
		// post semaphore for the thread in test_gui
		sem_post(&message_sema);       /* up semaphore */
	#endif
}

static void on_destroy(GtkWidget *widget, gpointer data)
{
	/*#ifdef GUI_SCENE_ON
		scene_destroy();
	#endif*/
	vp_os_free(gui);
	gtk_main_quit();
}

void create_main_box()
{
	#ifdef DEBUG_ON
		printf("Creating the Window>SubWindow>MainBox\n");
	#endif
   //vbox for having same Vertical size for each element
   // 10 is the space between two consecutive elements
   gui->box_main = gtk_vbox_new(FALSE, 10); 
   
   // adds the camera image to the main box
   gui->cam = gtk_image_new();
   gtk_box_pack_start(GTK_BOX(gui->box_main), gui->cam, FALSE, FALSE, 0);
   
   // initializes action buttons
   gui->button_connect = gtk_button_new_with_label("Connect");
   g_signal_connect (gui->button_connect, "clicked", G_CALLBACK (button_connect_callback), NULL);

   gui->button_disconnect = gtk_button_new_with_label("Disconnect");
   g_signal_connect (gui->button_disconnect, "clicked", G_CALLBACK (button_disconnect_callback), NULL);
   gtk_widget_set_sensitive(gui->button_disconnect, FALSE); // only on a connect button click can enable the getpos

   gui->button_getpos = gtk_button_new_with_label("Get position");
   g_signal_connect (gui->button_getpos, "clicked", G_CALLBACK (button_getpos_callback), NULL);
   gtk_widget_set_sensitive(gui->button_getpos, FALSE); // only on a connect button click can enable the getpos

   gui->text_server_state = gtk_label_new("none");
   gui->text_drone_state = gtk_label_new("none");
   gui->text_controller_state = gtk_label_new("none");

   gui->text_server_state_label = gtk_label_new("Server state");
   gui->text_drone_state_label = gtk_label_new("Drone state");
   gui->text_controller_state_label = gtk_label_new("Controller state");

   // add the action buttons to the box
   gtk_box_pack_start(GTK_BOX(gui->box_main), gui->button_connect, FALSE, FALSE, 0);
   gtk_box_pack_start(GTK_BOX(gui->box_main), gui->button_disconnect, FALSE, FALSE, 0);
   gtk_box_pack_start(GTK_BOX(gui->box_main), gui->button_getpos, FALSE, FALSE, 0);
   gtk_box_pack_start(GTK_BOX(gui->box_main), gui->text_server_state_label, FALSE, FALSE, 0);
   gtk_box_pack_start(GTK_BOX(gui->box_main), gui->text_server_state, FALSE, FALSE, 0);
   gtk_box_pack_start(GTK_BOX(gui->box_main), gui->text_drone_state_label, FALSE, FALSE, 0);
   gtk_box_pack_start(GTK_BOX(gui->box_main), gui->text_drone_state, FALSE, FALSE, 0);
   gtk_box_pack_start(GTK_BOX(gui->box_main), gui->text_controller_state_label, FALSE, FALSE, 0);
   gtk_box_pack_start(GTK_BOX(gui->box_main), gui->text_controller_state, FALSE, FALSE, 0);
}

void create_beacons_box()
{
	#ifdef DEBUG_ON
		printf("Creating the Window>SubWindow>BeaconsBox\n");
	#endif
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
      gtk_box_pack_start(GTK_BOX(gui->box_beacons), gui->label_beacon_ids[beacon_id], FALSE, FALSE, 0);
      if (beacon_id < NUM_BEACONS-1)
      {
         gtk_box_pack_start(GTK_BOX(gui->box_beacons), gui->label_beacon_timevals[beacon_id], FALSE, FALSE, 0);
      }
   }
   gtk_box_pack_start(GTK_BOX(gui->box_beacons), gui->label_beacon_timevals[beacon_id-1], FALSE, FALSE, 0);
}

void create_drone_box()
{
	#ifdef DEBUG_ON
		printf("Creating the Window>SubWindow>DroneBox\n");
	#endif
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
      gtk_box_pack_start(GTK_BOX(gui->box_drone_pos), gui->label_drone_pos_ids[pos_id], FALSE, FALSE, 0);
      // init values to default
      gui->label_drone_pos_values[pos_id] = gtk_label_new("none");
      if (pos_id < NUM_COORDINATES-1)
         gtk_box_pack_start(GTK_BOX(gui->box_drone_pos), gui->label_drone_pos_values[pos_id], FALSE, FALSE, 0);
   }
   gtk_box_pack_start(GTK_BOX(gui->box_drone_pos), gui->label_drone_pos_values[pos_id-1], FALSE, FALSE, 0);
}

#ifdef GUI_SCENE_ON
void create_graph_box()
{
	#ifdef DEBUG_ON
		printf("Creating the Window>GraphBox\n");
	#endif

	scene_init();
}
#endif


void create_sub_window()
{
	#ifdef DEBUG_ON
		printf("Creating the Window>SubWindow\n");
	#endif
	gui->box_subwindow = gtk_hbox_new(FALSE, 15);

	create_main_box();
	create_beacons_box();
	create_drone_box();

	gtk_box_pack_start(GTK_BOX(gui->box_subwindow), gui->box_main, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(gui->box_subwindow), gui->box_beacons, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(gui->box_subwindow), gui->box_drone_pos, FALSE, FALSE, 0);
}

void create_window()
{
	#ifdef DEBUG_ON
		printf("Creating the Window\n");
	#endif

	gui->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size(gui->window, 800, 600);
	// callback on window destroy event
	g_signal_connect(G_OBJECT(gui->window), "destroy", G_CALLBACK(on_destroy), NULL);
	// create window box
	gui->box_window = gtk_hbox_new(FALSE, 15);
	// create sub window
   create_sub_window();
   // create the scene
	#ifdef GUI_SCENE_ON
		create_graph_box();
   	gui->scene = get_scene();
	#endif
   // add boxes onto the window
	gtk_box_pack_start(GTK_BOX(gui->box_window), gui->box_subwindow, FALSE, FALSE, 0);
	#ifdef GUI_SCENE_ON
		gtk_box_pack_end(GTK_BOX(gui->box_window), gui->scene->box_graph, TRUE, TRUE, 0);
	#endif
}

void init_gui(int argc, char **argv)
{
   gui = vp_os_malloc(sizeof (gui_t));
   // what is this function ? Undefined reference if uncommented
   //g_thread_init(NULL);
   gdk_threads_init();
   gtk_init(&argc, &argv);
   // create all elements with subboxes
   create_window();
   // pack all the window box into the main window
   gtk_container_add (GTK_CONTAINER (gui->window), gui->box_window);
   // show all elements of the window
   gtk_widget_show_all(gui->window);
}

#endif // ifdef GUI_ON
