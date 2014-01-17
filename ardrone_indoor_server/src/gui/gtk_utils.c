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

#ifdef GUI_SCENE_ON
void gtk_plot_surface_apply_boundaries(gtk_boundaries_t *boundaries, gdouble *x, gdouble *y, gdouble *z) {
   if (boundaries->xmin>*x) *x=boundaries->xmin;
   if (boundaries->xmax<*x) *x=boundaries->xmax;
   if (boundaries->ymin>*y) *y=boundaries->ymin;
   if (boundaries->ymax<*y) *y=boundaries->ymax;
   if (boundaries->zmin>*z) *z=boundaries->zmin;
   if (boundaries->zmax<*z) *z=boundaries->zmax;
}
#endif
/**
 * x : center position on x axis
 * y : center position on y axis
 * z : center position on z axis
 * edge : size of one edge
 */

#ifdef GUI_SCENE_3D
void gtk_plot_surface_plot_circle(GtkPlot3D *plot, gdouble x, gdouble y, gdouble z, gdouble r)
{
	GdkColor background;
	gdouble px, py, pz;
	GtkWidget *widget;
	GtkPlotPC *pc;

	gtk_plot3d_get_pixel(plot, x, y, z, &px, &py, &pz);

	widget = GTK_WIDGET(plot);
	if(!gtk_widget_get_visible(widget)) return;

	gdk_color_alloc(gtk_widget_get_colormap(widget), &background);
	gdk_color_parse ("black", &background);

	pc = GTK_PLOT(plot)->pc;
	gtk_plot_pc_set_color(pc, &background);
	gtk_plot_pc_draw_ellipse(pc, TRUE, px, py, 2*r, 2*r);
}

void gtk_plot_surface_plot_plane(GtkPlot3D *plot, gdouble x, gdouble y, gdouble z, gdouble r, GdkColor background) {


#define NUM_PTS 4
	  GtkWidget *widget;
	  GdkPixmap *pixmap;
	  GtkPlotPC *pc;
	  GtkPlotVector v[NUM_PTS] = {
			  {x-r/2, y-r/2, z},
			  {x+r/2, y-r/2, z},
			  {x+r/2, y+r/2, z},
			  {x-r/2, y+r/2, z}
	  };
	  GtkPlotPoint p[NUM_PTS];
	  gdouble px, py, pz;
	  gint i;

	  widget = GTK_WIDGET(plot);
	  if(!gtk_widget_get_visible(widget)) return;

	  pixmap = GTK_PLOT(plot)->drawable;

	  // apply background color on plot
	  pc = GTK_PLOT(plot)->pc;
	  gtk_plot_pc_gsave(pc);
	  gtk_plot_pc_set_color(pc, &background);

	  // get pixel position from 3d position
	  printf("get pixels from space 3d onto 2d space\n");
	  for(i = 0; i < NUM_PTS; i++){
		  gtk_plot3d_get_pixel(plot, v[i].x, v[i].y, v[i].z, &px, &py, &pz);
		  p[i].x = px;
	     p[i].y = py;
	  }

	  for (i=0; i < NUM_PTS; i++){
		  printf("%d %.2f %.2f\n", i, p[i].x, p[i].y);
	  }
	  // draw filled polygon
	  gtk_plot_pc_draw_polygon(pc, TRUE, p, NUM_PTS);

	  // draw boundaries
	  gtk_plot_pc_set_color(pc, &plot->frame.color);
	  gtk_plot_pc_set_lineattr(pc,
	                           plot->frame.line_width,
	                           plot->frame.line_style == GTK_PLOT_LINE_SOLID ? GDK_LINE_SOLID : GDK_LINE_ON_OFF_DASH,
	                           0, 0);

	  if(plot->frame.line_style != GTK_PLOT_LINE_NONE)
	    gtk_plot_pc_draw_polygon(pc, FALSE, p, NUM_PTS);
}

void gtk_plot_surface_plot_square(GtkWidget *surface, gdouble x, gdouble y, gdouble z, gdouble r)
{

#define NUM_POINTS 10
/*
	gdouble *x_arr = (gdouble *)g_malloc(NUM_POINTS*NUM_POINTS * sizeof(gdouble)); //[NUM_POINTS*NUM_POINTS];
	gdouble *y_arr = (gdouble *)g_malloc(NUM_POINTS*NUM_POINTS * sizeof(gdouble));
	gdouble *z_arr = (gdouble *)g_malloc(NUM_POINTS*NUM_POINTS * sizeof(gdouble));
	gdouble *a_arr = (gdouble *)g_malloc(NUM_POINTS*NUM_POINTS * sizeof(gdouble));

	gint n = 0;
	gint i, j;
	// turns around Z axis every 45 degrees
	for (i = 0; i < NUM_POINTS; i++){
		// turns around X/Y axis using phi between 0 and 180 degrees, every 45 degrees
		for(j = 0; j < NUM_POINTS; j++){ //M_PI*0.5
			x_arr[n] = x + (i-NUM_POINTS/2)*(r/NUM_POINTS);
			y_arr[n] = y + (j-NUM_POINTS/2)*(r/NUM_POINTS);
			z_arr[n] = z;
			a_arr[n] = 0.5;

			//gtk_plot_surface_apply_boundaries(boundaries, &x_arr[n], &y_arr[n], &z_arr[n]);

			printf("%d x %.2f y %.2f z %.2f \n", n, x_arr[n], y_arr[n], z_arr[n]);
			n++;
		}
	}

	// surface
	gtk_plot_surface_set_points(GTK_PLOT_SURFACE(surface),
			x_arr, y_arr, z_arr, NULL, NULL, NULL, 40, 40);
	//gtk_plot_data_set_legend(GTK_PLOT_DATA(surface), "legend");

	printf("points set\n");*/

	//gtk_plot_surface_use_amplitud(GTK_PLOT_SURFACE(surface), TRUE);
	//gtk_plot_data_set_a(GTK_PLOT_DATA(surface), a_arr);
	//gtk_plot_data_gradient_set_visible(GTK_PLOT_DATA(surface), TRUE);
}

#endif


