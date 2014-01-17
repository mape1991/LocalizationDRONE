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
			a_arr[n] = 0.5;

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



static void
gtk_plot3d_draw_plane(GtkPlot3D *plot,
                      GtkPlotVector v1,
                      GtkPlotVector v2,
                      GtkPlotVector v3,
                      GtkPlotVector v4,
                      GdkColor background)
{
  GtkWidget *widget;
  GdkPixmap *pixmap;
  GtkPlotPC *pc;
  GtkPlotVector v[4];
  GtkPlotPoint p[4];
  gdouble px, py, pz;
  gint i;

  widget = GTK_WIDGET(plot);
  if(!gtk_widget_get_visible(widget)) return;

  pixmap = GTK_PLOT(plot)->drawable;
  pc = GTK_PLOT(plot)->pc;

  gtk_plot_pc_set_color(pc, &background);

  v[0] = v1;
  v[1] = v2;
  v[2] = v3;
  v[3] = v4;
  printf("get pixels from space 3d onto 2d space\n");
  for(i = 0; i < 4; i++){
    gtk_plot3d_get_pixel(plot, v[i].x, v[i].y, v[i].z, &px, &py, &pz);
    p[i].x = px;
    p[i].y = py;
  }

  gtk_plot_pc_draw_polygon(pc, TRUE, p, 4);
  gtk_plot_pc_set_color(pc, &plot->frame.color);

  gtk_plot_pc_set_lineattr(pc,
                           plot->frame.line_width,
                           plot->frame.line_style == GTK_PLOT_LINE_SOLID ? GDK_LINE_SOLID : GDK_LINE_ON_OFF_DASH,
                           0, 0);

  if(plot->frame.line_style != GTK_PLOT_LINE_NONE)
    gtk_plot_pc_draw_polygon(pc, FALSE, p, 4);

}

void gtk_plot_surface_plot_plane(GtkPlot3D *plot, gdouble x, gdouble y, gdouble z, gdouble r) {

	GtkPlotVector v1 = {x-r/2, y-r/2, z};
	GtkPlotVector v2 = {x+r/2, y-r/2, z};
	GtkPlotVector v3 = {x+r/2, y+r/2, z};
	GtkPlotVector v4 = {x-r/2, y+r/2, z};
	GdkColor background;
	gboolean parsed = gdk_color_parse ("#00000000", &background);

	printf("parsed %d\n", (parsed ? 1 : 0));

	  GtkWidget *widget;
	  GdkPixmap *pixmap;
	  GtkPlotPC *pc;
	  GtkPlotVector v[4];
	  GtkPlotPoint p[4];
	  gdouble px, py, pz;
	  gint i;

	  widget = GTK_WIDGET(plot);
	  if(!gtk_widget_get_visible(widget)) return;

	  pixmap = GTK_PLOT(plot)->drawable;
	  pc = GTK_PLOT(plot)->pc;

	  gtk_plot_pc_set_color(pc, &background);

	  v[0] = v1;
	  v[1] = v2;
	  v[2] = v3;
	  v[3] = v4;
	  printf("get pixels from space 3d onto 2d space\n");
	  for(i = 0; i < 4; i++){
		  gtk_plot3d_get_pixel(plot, v[i].x, v[i].y, v[i].z, &px, &py, &pz);
		  printf("%.2f %.2f %.2fpts position from %.2f %.2f\n", v[i].x, v[i].y, v[i].z, px, py);

		  p[i].x = px;
	    p[i].y = py;

	  }

	 // plot->frame.line_style = GTK_PLOT_LINE_SOLID;

	  gtk_plot_pc_draw_polygon(pc, TRUE, p, 4);
	  /*gtk_plot_pc_set_color(pc, &plot->frame.color);

	  gtk_plot_pc_set_lineattr(pc,
	                           plot->frame.line_width,
	                           plot->frame.line_style == GTK_PLOT_LINE_SOLID ? GDK_LINE_SOLID : GDK_LINE_ON_OFF_DASH,
	                           0, 0);

	  if(plot->frame.line_style != GTK_PLOT_LINE_NONE)
	    gtk_plot_pc_draw_polygon(pc, FALSE, p, 4);*/
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


