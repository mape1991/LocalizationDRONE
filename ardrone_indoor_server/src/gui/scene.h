/*
 * scene.h
 *
 *  Created on: Dec 22, 2013
 *      Author: julien
 */

#ifndef SCENE_H_
#define SCENE_H_

#include "../global_config.h"

#ifdef GUI_SCENE_ON
#include <gtk/gtk.h>
	// best practice taken from this link
	// http://gtkextra.sourceforge.net/docs/reference/gtkextra-3.0.5/compiling.html
#include <gtkextra/gtkextra.h>
#include "gtk_utils.h"

#define ROOM_MAX_X 10
#define ROOM_MAX_Y 7
#define ROOM_MAX_Z 4

typedef struct scene {

	GtkWidget *box_graph; // box containing the graph and alternative graphical utilities
	GtkWidget *active_plot; // 3D plotter (GtkPlot3d component of the GtkExtra)
	GdkPixmap *pixmap;
	GtkWidget **plots;
	GtkWidget **buttons;
	GtkPlotData *dataset[5];
	gint nlayers;

	GtkWidget *scrollw1;
	GtkWidget *canvas;
	//GtkWidget *button;
	GtkWidget *surface;

	GtkPlotCanvasChild *child;
	gtk_boundaries_t boundaries;
	gint max_points; // the maximum number of plotted points


	gint cnt;
	gint page_width, page_height;
	gfloat scale;


}scene_t;

scene_t *get_scene();

void scene_init();
void scene_destroy();
GtkWidget *scene_new_layer();

#endif
#endif /* SCENE_H_ */
