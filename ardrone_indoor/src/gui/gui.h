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

#include <gtk/gtk.h>

typedef struct gui
{
  GtkWidget *window;
  GtkWidget *start;
  GtkWidget *stop;
  GtkWidget *box;
  GtkWidget *cam;
} gui_t;
 
gui_t *get_gui();
 
void init_gui(int argc, char **argv);
#endif

#endif	/* GUI_H */

