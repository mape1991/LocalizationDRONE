/*
 * gtk_utils.c
 *
 * Main purpose: provides additional functionalities to draw custom shapes
 *
 * J: I had issues to find how to draw spheres,
 * it seems that gtkplot3d is the only means to plot any 3d shapes
 * it does not make any spheres, so we need to
 *
 *  Created on: Dec 22, 2013
 *      Author: julien
 */

#include "gtk_utils.h"

void gtk_plot_surface_draw_sphere(GtkWidget *plot, gdouble radius)
{

	/*GtkWidget *surface = gtk_plot_surface_new();

	gtk_plot_surface_set_points(GTK_PLOT_SURFACE(surface),
			x, y, z, NULL, NULL, NULL, 40, 40);
	gtk_plot_data_set_legend(GTK_PLOT_DATA(surface), "cos ((r-r\\s0\\N)\\S2\\N)");

	gtk_plot_surface_use_amplitud(GTK_PLOT_SURFACE(surface), TRUE);
	gtk_plot_data_set_a(GTK_PLOT_DATA(surface), a);
	gtk_plot_data_gradient_set_visible(GTK_PLOT_DATA(surface), TRUE);

	gtk_plot_add_data(GTK_PLOT(plot), GTK_PLOT_DATA(surface));*/
}



