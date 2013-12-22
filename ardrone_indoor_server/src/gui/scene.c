/*
 * scene.c
 *
 * main purpose: fully initialized gtk object for the main gui
 *
 * separated from the gui for splitting concerns and maintainability
 * the code is mainly taken from testgtkplot3d in the gtkextra directory
 * and adapted, keeping the initial example with a define
 *
 *  Created on: Dec 22, 2013
 *      Author: julien
 */
#include "scene.h"

#ifdef GUI_SCENE_ON

// singleton
scene_t *scene = NULL;

scene_t *get_scene() {
	return scene;
}

void quit ()
{
  gtk_main_quit();
}

gdouble test_gui_scene_example_compute_z(gdouble x, gdouble y)
{
 return cos(((x-0.5)*(x-0.5) + (y-0.5)*(y-0.5))*24) / 4. + .5;

/* z = (x*x + y*y) / 9.0;
*/
}

// functions to try the 3d visualization into the canvas by making a custom surface using the func cos(r-r0)^2
void test_gui_scene_example()
{
	// make 1600 samples
   for(scene->nx = 0; scene->nx < 40; scene->nx++){
		for(scene->ny = 0; scene->ny < 40; scene->ny++)
		{
			scene->x[scene->n] = 0.025*(gdouble)scene->nx;
			scene->y[scene->n] = 0.025*(gdouble)scene->ny;
			scene->z[scene->n] = test_gui_scene_example_compute_z(scene->x[scene->n], scene->y[scene->n]);
			scene->a[scene->n] = ((scene->x[scene->n]-0.5)*(scene->x[scene->n]-0.5) + (scene->y[scene->n]-0.5)*(scene->y[scene->n]-0.5));
			scene->n++;
		}
   }
	// define the mins and maxs on each axis
	scene->xmin=1e99;
	scene->xmax=-1e99;
	scene->ymin=1e99;
	scene->ymax=-1e99;
	scene->zmin=1e99;
	scene->zmax=-1e99;

	scene->surface = gtk_plot_surface_new();

	gtk_plot_surface_set_points(GTK_PLOT_SURFACE(scene->surface),
			scene->x, scene->y, scene->z, NULL, NULL, NULL, 40, 40);
	gtk_plot_data_set_legend(GTK_PLOT_DATA(scene->surface), "cos ((r-r\\s0\\N)\\S2\\N)");

	gtk_plot_surface_use_amplitud(GTK_PLOT_SURFACE(scene->surface), TRUE);
	gtk_plot_data_set_a(GTK_PLOT_DATA(scene->surface), scene->a);
	gtk_plot_data_gradient_set_visible(GTK_PLOT_DATA(scene->surface), TRUE);

	gtk_plot_add_data(GTK_PLOT(scene->active_plot), GTK_PLOT_DATA(scene->surface));
}

void test_gui_location()
{
	// define the mins and maxs on each axis
	scene->xmin=0;
	scene->xmax=ROOM_MAX_X;
	scene->ymin=0;
	scene->ymax=ROOM_MAX_Y;
	scene->zmin=0;
	scene->zmax=ROOM_MAX_Z;

	scene->surface = gtk_plot_surface_new();

	// a surface has vertexes defined from a set of points, it is not a list of points to draw!
	gtk_plot_surface_set_points(GTK_PLOT_SURFACE(scene->surface),
			scene->x, scene->y, scene->z, NULL, NULL, NULL, 40, 40);
	gtk_plot_data_set_legend(GTK_PLOT_DATA(scene->surface), "Test location");

	gtk_plot_surface_use_amplitud(GTK_PLOT_SURFACE(scene->surface), TRUE);
	gtk_plot_data_set_a(GTK_PLOT_DATA(scene->surface), scene->a);
	gtk_plot_data_gradient_set_visible(GTK_PLOT_DATA(scene->surface), TRUE);
}

void test_gui_location_new_point(GtkWidget *button, gpointer data)
{
	printf("location new point\n");
	// generate a random position
	gdouble x = ((gdouble)(rand() % (4*ROOM_MAX_X))/4.0); // 0.25 unit of precision
	gdouble y = ((gdouble)(rand() % (4*ROOM_MAX_X))/4.0); // 0.25 unit of precision
	gdouble z = ((gdouble)(rand() % (4*ROOM_MAX_X))/4.0); // 0.25 unit of precision

	printf("add pt x %.2f y %.2f z %.2f\n", x,y,z);
	// add it into the graph
	scene_add_point(x,y,z);
}

void scene_add_point(gdouble x, gdouble y, gdouble z)
{
	// store the point into the set
	scene->x[scene->n] = x;
	scene->y[scene->n] = y;
	scene->z[scene->n] = z;
	scene->n++;

	// plot a new eclipse on the spot
	//gtk_plot_canvas_put_ellipse(scene->canvas, x, y);
	gtk_plot_surface_set_points(GTK_PLOT_SURFACE(scene->surface),
			scene->x, scene->y, scene->z, NULL, NULL, NULL, 40, 40);
	gtk_plot_data_set_legend(GTK_PLOT_DATA(scene->surface), "Test location");

	gtk_plot_surface_use_amplitud(GTK_PLOT_SURFACE(scene->surface), TRUE);
	gtk_plot_data_set_a(GTK_PLOT_DATA(scene->surface), scene->a);
	gtk_plot_data_gradient_set_visible(GTK_PLOT_DATA(scene->surface), TRUE);
}

GtkWidget *scene_new_layer()
{
	gchar label[10];
	GtkRequisition req;
	gint size;

	scene->nlayers++;

	scene->buttons = (GtkWidget **)g_realloc(scene->buttons, scene->nlayers * sizeof(GtkWidget *));
	scene->plots = (GtkWidget **)g_realloc(scene->plots, scene->nlayers * sizeof(GtkWidget *));

	sprintf(label, "%d", scene->nlayers);

	scene->buttons[scene->nlayers-1] = gtk_toggle_button_new_with_label(label);
	/* gtk_button_set_relief(GTK_BUTTON(buttons[nlayers-1]), GTK_RELIEF_NONE);
	*/
	gtk_widget_size_request(scene->buttons[scene->nlayers-1], &req);
	size = MAX(req.width,req.height);
	gtk_widget_set_size_request(scene->buttons[scene->nlayers-1], size, size);
	gtk_fixed_put(GTK_FIXED(scene->canvas), scene->buttons[scene->nlayers-1], (scene->nlayers-1)*20, 0);
	gtk_widget_show(scene->buttons[scene->nlayers-1]);

	scene->plots[scene->nlayers-1] = gtk_plot3d_new_with_size(NULL, .70, .70);

	gtk_widget_show(scene->plots[scene->nlayers-1]);

	return scene->plots[scene->nlayers-1];
}

void scene_rotate_x(GtkWidget *button, gpointer data)
{
	gtk_plot3d_rotate_x(GTK_PLOT3D(scene->active_plot), 10.);

	gtk_plot_canvas_paint(GTK_PLOT_CANVAS(data));
	gtk_plot_canvas_refresh(GTK_PLOT_CANVAS(data));
}

void scene_rotate_y(GtkWidget *button, gpointer data)
{
	gtk_plot3d_rotate_y(GTK_PLOT3D(scene->active_plot), 10.);

	gtk_plot_canvas_paint(GTK_PLOT_CANVAS(data));
	gtk_plot_canvas_refresh(GTK_PLOT_CANVAS(data));
}

void scene_rotate_z(GtkWidget *button, gpointer data)
{
	gtk_plot3d_rotate_z(GTK_PLOT3D(scene->active_plot), 10.);

	gtk_plot_canvas_paint(GTK_PLOT_CANVAS(data));
	gtk_plot_canvas_refresh(GTK_PLOT_CANVAS(data));
}


void scene_init(){
	scene = vp_os_malloc(sizeof (scene_t));

	scene->pixmap = NULL;
	scene->cnt = 0;
	scene->scale = 1.;
	scene->nlayers = 0;
	scene->max_points = 1600;

	scene->page_width = GTK_PLOT_LETTER_W * scene->scale;
	scene->page_height = GTK_PLOT_LETTER_H * scene->scale;

	scene->box_graph=gtk_vbox_new(FALSE,5);

	scene->scrollw1=gtk_scrolled_window_new(NULL, NULL);
	gtk_container_border_width(GTK_CONTAINER(scene->scrollw1),0);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scene->scrollw1),
				GTK_POLICY_ALWAYS,GTK_POLICY_ALWAYS);
	gtk_box_pack_start(GTK_BOX(scene->box_graph),scene->scrollw1, TRUE, TRUE,0);

	scene->canvas = gtk_plot_canvas_new(scene->page_width, scene->page_height, scene->scale);
	GTK_PLOT_CANVAS_SET_FLAGS(GTK_PLOT_CANVAS(scene->canvas), GTK_PLOT_CANVAS_DND_FLAGS);
	gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scene->scrollw1), scene->canvas);

/*
 gdk_color_parse("light blue", &color);
 gdk_color_alloc(gtk_widget_get_colormap(canvas), &color);
 gtk_plot_canvas_set_background(GTK_PLOT_CANVAS(canvas), &color);
*/
   printf("creating plot\n");

   scene->active_plot = scene_new_layer(scene);

   scene->child = gtk_plot_canvas_plot_new(GTK_PLOT(scene->active_plot));
   gtk_plot_canvas_put_child(GTK_PLOT_CANVAS(scene->canvas), scene->child, .10, .06, .85, .85);
   printf("creating test samples\n");

   scene->x = (gdouble *)g_malloc(scene->max_points * sizeof(gdouble));
   scene->y = (gdouble *)g_malloc(scene->max_points * sizeof(gdouble));
   scene->z = (gdouble *)g_malloc(scene->max_points * sizeof(gdouble));
   scene->a = (gdouble *)g_malloc(scene->max_points * sizeof(gdouble));

   // number of initialized points
   scene->n = 0;

   // init each sample
	#ifdef TEST_GUI_SCENE_EXAMPLE
		test_gui_scene_example();
	#else
		test_gui_location();
	#endif

	gtk_plot3d_set_minor_ticks(GTK_PLOT3D(scene->active_plot), GTK_PLOT_AXIS_X, 1);
	gtk_plot3d_set_minor_ticks(GTK_PLOT3D(scene->active_plot), GTK_PLOT_AXIS_Y, 1);
	gtk_plot3d_show_ticks(GTK_PLOT3D(scene->active_plot),
									 GTK_PLOT_SIDE_XY,
									 GTK_PLOT_TICKS_OUT, GTK_PLOT_TICKS_OUT);
	gtk_plot3d_show_ticks(GTK_PLOT3D(scene->active_plot),
									 GTK_PLOT_SIDE_XZ,
									 GTK_PLOT_TICKS_OUT, GTK_PLOT_TICKS_OUT);
	gtk_plot3d_show_ticks(GTK_PLOT3D(scene->active_plot),
									 GTK_PLOT_SIDE_YX,
									 GTK_PLOT_TICKS_OUT, GTK_PLOT_TICKS_OUT);
	gtk_plot3d_show_ticks(GTK_PLOT3D(scene->active_plot),
									 GTK_PLOT_SIDE_YZ,
									 GTK_PLOT_TICKS_OUT, GTK_PLOT_TICKS_OUT);
	gtk_plot3d_show_ticks(GTK_PLOT3D(scene->active_plot),
									 GTK_PLOT_SIDE_ZX,
									 GTK_PLOT_TICKS_OUT, GTK_PLOT_TICKS_OUT);
	gtk_plot3d_show_ticks(GTK_PLOT3D(scene->active_plot),
									 GTK_PLOT_SIDE_ZY,
									 GTK_PLOT_TICKS_OUT, GTK_PLOT_TICKS_OUT);

	/* if a file was specified on the commandline, try to open and display it: */
	printf("condition\n");
	//if (argc!=2 || !(f=fopen(argv[1],"r"))) {
/*
   x= y= z= NULL;
   surface = gtk_plot_surface_new_function((GtkPlotFunc3D) function);
   gtk_plot_surface_set_xstep(GTK_PLOT_SURFACE(surface), .025);
   gtk_plot_surface_set_ystep(GTK_PLOT_SURFACE(surface), .025);
   gtk_plot_data_set_legend(GTK_PLOT_DATA(surface), "cos ((r-r\\s0\\N)\\S2\\N)");
*/

 //} else {
	 /*
	 printf("start simulating\n");
   x= g_new(gdouble,MAXNODES);
   y= g_new(gdouble,MAXNODES);
   z= g_new(gdouble,MAXNODES);
   while (fgets(buffer,1000,f) && cnt<MAXNODES)
     if (sscanf(buffer,"%lf %lf %lf", &x[cnt], &y[cnt], &z[cnt])==3) {
       z[cnt]*=10;
       if (xmin>x[cnt]) xmin= x[cnt];
       if (xmax<x[cnt]) xmax= x[cnt];
       if (ymin>y[cnt]) ymin= y[cnt];
       if (ymax<y[cnt]) ymax= y[cnt];
       if (zmin>z[cnt]) zmin= z[cnt];
       if (zmax<z[cnt]) zmax= z[cnt];
       cnt++;
     }
   fclose(f);
   dx= (xmax-xmin)*.02;
   dy= (ymax-ymin)*.02;
   dz= (zmax-zmin)*.02;

   /* start the triangulation */
   //fprintf(stderr,"data ranges from (%g,%g) to (%g,%g)\n",
	//   xmin,ymin,xmax,ymax);

   /*
   gtk_plot3d_set_xrange(GTK_PLOT3D(active_plot), xmin-dx, xmax+dx);
   gtk_plot3d_set_yrange(GTK_PLOT3D(active_plot), ymin-dy, ymax+dy);
   gtk_plot3d_set_zrange(GTK_PLOT3D(active_plot), zmin-dz, zmax+dz);
   gtk_plot3d_set_ticks(GTK_PLOT3D(active_plot), GTK_PLOT_AXIS_X, (xmax-xmin)/10., 1);
   gtk_plot3d_set_ticks(GTK_PLOT3D(active_plot), GTK_PLOT_AXIS_Y, (ymax-ymin)/10., 1);
   gtk_plot3d_set_ticks(GTK_PLOT3D(active_plot), GTK_PLOT_AXIS_Z, (zmax-zmin)/10., 1);
   */

//   surface = gtk_plot_csurface_new();
//   gtk_plot_add_data(GTK_PLOT(active_plot), GTK_PLOT_DATA(surface));
//   gtk_plot_surface_set_points(GTK_PLOT_SURFACE(surface),
//			       x, y, z, NULL, NULL, NULL, cnt, 1);
//   gtk_plot_data_set_legend(GTK_PLOT_DATA(surface), argv[1]);
  // gtk_plot_data_gradient_autoscale_z(GTK_PLOT_DATA(surface));
// }
/*
 gtk_plot_surface_use_height_gradient(GTK_PLOT_SURFACE(surface), TRUE);
 gtk_plot_data_set_gradient(GTK_PLOT_DATA(surface), .2, .8, 6, 0);
*/

/*
 gtk_plot_data_gradient_set_scale(GTK_PLOT_DATA(surface),GTK_PLOT_SCALE_LOG10);
*/

 gtk_plot3d_autoscale(GTK_PLOT3D(scene->active_plot));
/*
 gtk_plot3d_set_xrange(GTK_PLOT3D(active_plot), -0.5,1);
 gtk_plot3d_set_yrange(GTK_PLOT3D(active_plot), -0.2,1);
 gtk_plot3d_set_zrange(GTK_PLOT3D(active_plot), 0.,1);
*/
/*
 gtk_plot3d_rotate_x(GTK_PLOT3D(active_plot), GTK_PLOT3D(active_plot)->a1+360-300);
 gtk_plot3d_rotate_y(GTK_PLOT3D(active_plot), GTK_PLOT3D(active_plot)->a2+360-360);
 gtk_plot3d_rotate_z(GTK_PLOT3D(active_plot), GTK_PLOT3D(active_plot)->a3+360-330);
*/
 gtk_widget_show(scene->surface);


 GtkWidget *button;

 button = gtk_button_new_with_label("Rotate X");
 gtk_fixed_put(GTK_FIXED(scene->canvas), button, 80, 0);

 g_signal_connect(GTK_OBJECT(button), "clicked",
                    GTK_SIGNAL_FUNC(scene_rotate_x), scene->canvas);

 button = gtk_button_new_with_label("Rotate Y");
 gtk_fixed_put(GTK_FIXED(scene->canvas), button, 160, 0);

 g_signal_connect(GTK_OBJECT(button), "clicked",
                    GTK_SIGNAL_FUNC(scene_rotate_y), scene->canvas);

 button = gtk_button_new_with_label("Rotate Z");
 gtk_fixed_put(GTK_FIXED(scene->canvas), button, 240, 0);

 g_signal_connect(GTK_OBJECT(button), "clicked",
                    GTK_SIGNAL_FUNC(scene_rotate_z), scene->canvas);

	#ifdef TEST_GUI_SCENE_LOCATION
		 button = gtk_button_new_with_label("Add point");
		 gtk_fixed_put(GTK_FIXED(scene->canvas), button, 320, 0);

		 g_signal_connect(GTK_OBJECT(button), "clicked",
								  GTK_SIGNAL_FUNC(test_gui_location_new_point), scene->canvas);
	#endif
/*
 gtk_plot3d_set_xfactor(GTK_PLOT3D(active_plot), 0.5);
*/
/*
 gtk_plot3d_set_xrange(GTK_PLOT3D(active_plot), 0., 2.0);
*/


/*
 gtk_plot_canvas_export_ps(GTK_PLOT_CANVAS(canvas), "demo3d.ps", 0, 0,
                           GTK_PLOT_LETTER);
*/

/* gtk_plot_canvas_export_ps_with_size(GTK_PLOT_CANVAS(canvas), "demo3d.ps",
				     0, 0, GTK_PLOT_PSPOINTS,
                            	     GTK_PLOT_LETTER_W, GTK_PLOT_LETTER_H);
*/
printf("fully initialized\n");
}

void scene_destroy()
{
	vp_os_free(scene);
}


#endif

