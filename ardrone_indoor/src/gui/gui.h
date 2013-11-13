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

#include "../com/commons_comm.h"
#include "../com/server_comm.h"

#include <gtk/gtk.h>

#define NUM_COORDINATES 3

typedef struct gui
{
  GtkWidget *window;
  GtkWidget *box_window; // contain all the containers
  
  // main content
  GtkWidget *box_main; // container
  
  GtkWidget *button_start;
  GtkWidget *button_stop;
  GtkWidget *button_listen;
  GtkWidget *button_send;

  GtkWidget *cam; // camera image
  
  // beacons content
  GtkWidget *box_beacons;
  
  GtkWidget *label_beacon_ids[NUM_BEACONS];
  GtkWidget *label_beacon_timevals[NUM_BEACONS];
  
  // drone content
  GtkWidget *box_drone_pos;
  
  GtkWidget *label_drone_pos_ids[NUM_COORDINATES];
  GtkWidget *label_drone_pos_values[NUM_COORDINATES];
  
} gui_t;
 
gui_t *get_gui();
 
void on_drone_message_received(char *message);
void init_gui(int argc, char **argv);
#endif

#endif	/* GUI_H */

