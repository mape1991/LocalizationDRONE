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
#include <math.h>

void gtk_plot_surface_apply_boundaries(gtk_boundaries_t *boundaries, gdouble *x, gdouble *y, gdouble *z) {
   if (boundaries->xmin>*x) *x=boundaries->xmin;
   if (boundaries->xmax<*x) *x=boundaries->xmax;
   if (boundaries->ymin>*y) *y=boundaries->ymin;
   if (boundaries->ymax<*y) *y=boundaries->ymax;
   if (boundaries->zmin>*z) *z=boundaries->zmin;
   if (boundaries->zmax<*z) *z=boundaries->zmax;
}
/**
 * x : center position on x axis
 * y : center position on y axis
 * z : center position on z axis
 * edge : size of one edge
 */
void gtk_plot_surface_plot_sphere(GtkWidget *surface, gtk_boundaries_t *boundaries, gdouble x, gdouble y, gdouble z, gdouble r)
{
	// WARNING: maybe impossible to do

	gdouble theta, phi;
	gdouble x_arr[16];
	gdouble y_arr[16];
	gdouble z_arr[16];
	gdouble a_arr[16];

	gint n = 0;
	// turns around Z axis every 45 degrees
	for (theta = 0.0; theta < 2*M_PI; theta+=M_PI*0.25){
		// turns around X/Y axis using phi between 0 and 180 degrees, every 45 degrees
		for(phi = -M_PI*0.5; phi < 0; phi+=M_PI*0.25){ //M_PI*0.5
			x_arr[n] = x + r*cos(theta)*sin(phi);
			y_arr[n] = y + r*sin(theta)*sin(phi);
			z_arr[n] = z + r*cos(phi);
			a_arr[n] = 1.0;

			gtk_plot_surface_apply_boundaries(boundaries, &x_arr[n], &y_arr[n], &z_arr[n]);

			printf("x %.2f y %.2f z %.2f \n", x_arr[n], y_arr[n], z_arr[n]);
			n++;
		}
	}

	// surface
	gtk_plot_surface_set_points(GTK_PLOT_SURFACE(surface),
			x_arr, y_arr, z_arr, NULL, NULL, NULL, 4, 4);
	printf("points set\n");
	gtk_plot_surface_use_amplitud(GTK_PLOT_SURFACE(surface), TRUE);
	gtk_plot_data_set_a(GTK_PLOT_DATA(surface), a_arr);
	gtk_plot_data_gradient_set_visible(GTK_PLOT_DATA(surface), TRUE);
}


void gtk_plot_surface_plot_square(GtkWidget *surface, gtk_boundaries_t *boundaries, gdouble x, gdouble y, gdouble z, gdouble r)
{
	gdouble theta, phi;
	gdouble x_arr[4];
	gdouble y_arr[4];
	gdouble z_arr[4];
	gdouble a_arr[4];

	gint n = 0;
	// turns around Z axis every 45 degrees
	for (theta = -r/2; theta < r/2; theta+=r/2){
		// turns around X/Y axis using phi between 0 and 180 degrees, every 45 degrees
		for(phi = -r/2; phi < r/2; phi+=r/2){ //M_PI*0.5
			x_arr[n] = x + theta;
			y_arr[n] = y + phi;
			z_arr[n] = z;
			a_arr[n] = 1.0;

			gtk_plot_surface_apply_boundaries(boundaries, &x_arr[n], &y_arr[n], &z_arr[n]);

			printf("x %.2f y %.2f z %.2f \n", x_arr[n], y_arr[n], z_arr[n]);
			n++;
		}
	}

	// surface
	gtk_plot_surface_set_points(GTK_PLOT_SURFACE(surface),
			x_arr, y_arr, z_arr, NULL, NULL, NULL, 2, 2);
	printf("points set\n");
	//gtk_plot_surface_use_amplitud(GTK_PLOT_SURFACE(surface), TRUE);
	//gtk_plot_data_set_a(GTK_PLOT_DATA(surface), a_arr);
	//gtk_plot_data_gradient_set_visible(GTK_PLOT_DATA(surface), TRUE);
}


