/*
 * gtk_utils.h
 *
 *  Created on: Dec 22, 2013
 *      Author: julien
 */

#ifndef GTK_UTILS_H_
#define GTK_UTILS_H_

#include "../global_config.h"

#ifdef GUI_SCENE_ON
#include <gtk/gtk.h>
	// best practice taken from this link
	// http://gtkextra.sourceforge.net/docs/reference/gtkextra-3.0.5/compiling.html
#include <gtkextra/gtkextra.h>
#endif

/** The boundaries of a gtkplot3d with minimum and maximum on each space axis */
typedef struct gtk_boundaries{
	gdouble xmin; /** */
	gdouble xmax; /** */
	gdouble ymin; /** */
	gdouble ymax; /** */
	gdouble zmin; /** */
	gdouble zmax; /** */
}gtk_boundaries_t;

/**
 * Bounds the (x,y,z) position to the boundaries-delimited space.
 */
void gtk_plot_surface_apply_boundaries(gtk_boundaries_t *boundaries, gdouble *x, gdouble *y, gdouble *z);
/**
 * Creates a sphere from a (x,y,z) position, a (r) radius and the boundaries of the 3d space.
 */
void gtk_plot_surface_plot_sphere(GtkWidget *surface, gtk_boundaries_t *boundaries, gdouble x, gdouble y, gdouble z, gdouble r);


#endif /* GTK_UTILS_H_ */
