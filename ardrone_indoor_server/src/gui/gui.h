/* 
 * File:   gui.h
 * Author: julien
 *
 * Created on November 12, 2013, 2:18 AM
 */
#ifndef GUI_H
#define	GUI_H

#include "../global_config.h"

#ifdef GUI_ON

#include "../../../ardrone_indoor_commons/global_com.h"
#include "../../../ardrone_indoor_commons/com/udp_comm.h"
#include <gtk/gtk.h>

#ifdef GUI_SCENE_ON
	#include "scene.h"
#endif

#ifdef TEST_GUI
	#include "../tests/test_gui.h"
#elif defined TEST_FULL
	#include "../tests/test_full.h"
#endif

#define NUM_COORDINATES 3

#define STATE_DISCONNECTED 0
#define STATE_CONNECTED 1

typedef struct gui
{
  GtkWidget *window;
  GtkWidget *box_window; // contain all the containers
  GtkWidget *box_subwindow; // contains the mainbox, beaconsbox, dronepositionbox, except the graphbox
  
  // main content
  GtkWidget *box_main; // container
  
  GtkWidget *button_disconnect;
  GtkWidget *button_connect; // the connect is responsible for the states Connect and Disconnect of the system
  GtkWidget *button_getpos; // the getpos is responsible for the state Sync of the system

  // communication content
  GtkWidget *label_udp_ip;   // the ip address of the system target (i.e. drone)
  GtkWidget *text_udp_ip;
  /*GtkWidget *label_udp_port; // the target port
  GtkWidget *text_udp_port;
  GtkWidget *label_usb_port_name; // the target usb port name
  GtkWidget *text_usb_port_name;*/

  GtkWidget *cam; // camera image
  
  // beacons content
  GtkWidget *box_beacons;
  
  GtkWidget *label_beacon_ids[NUM_BEACONS];
  GtkWidget *label_beacon_timevals[NUM_BEACONS];
  
  // drone content
  GtkWidget *box_drone_pos;
  
  GtkWidget *label_drone_pos_ids[NUM_COORDINATES];
  GtkWidget *label_drone_pos_values[NUM_COORDINATES];

  // message text to inform the user on the system state
  GtkWidget *text_server_state;
  GtkWidget *text_drone_state;
  GtkWidget *text_controller_state;
  GtkWidget *text_server_state_label;
  GtkWidget *text_drone_state_label;
  GtkWidget *text_controller_state_label;

  // 3D graph for position
  #ifdef GUI_SCENE_ON
  	  scene_t *scene;
  #endif

} gui_t;
 
gui_t *get_gui();

void init_gui(int argc, char **argv);

void button_connect_callback();
void button_disconnect_callback();
void button_getpos_callback();

#endif

#endif	/* GUI_H */

