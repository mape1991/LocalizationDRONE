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

#ifdef GUI_SCENE_3D
gdouble test_gui_scene_example_compute_z(gdouble x, gdouble y)
{
 return cos(((x-0.5)*(x-0.5) + (y-0.5)*(y-0.5))*24) / 4. + .5;

/* z = (x*x + y*y) / 9.0;
*/
}

// functions to try the 3d visualization into the canvas by making a custom surface using the func cos(r-r0)^2
void test_gui_scene_example()
{
   gdouble *x = (gdouble *)g_malloc(40*40 * sizeof(gdouble));
   gdouble *y = (gdouble *)g_malloc(40*40 * sizeof(gdouble));
   gdouble *z = (gdouble *)g_malloc(40*40 * sizeof(gdouble));
   gdouble *a = (gdouble *)g_malloc(40*40 * sizeof(gdouble));

   gint n, nx, ny;
   n = 0;
	// make 1600 samples
   for(nx = 0; nx < 40; nx++){
		for(ny = 0; ny < 40; ny++)
		{
			x[n] = 0.025*(gdouble)nx;
			y[n] = 0.025*(gdouble)ny;
			z[n] = test_gui_scene_example_compute_z(x[n], y[n]);
			a[n] = ((x[n]-0.5)*(x[n]-0.5) + (y[n]-0.5)*(y[n]-0.5));
			n++;
		}
   }
	// define the mins and maxs on each axis
	scene->boundaries.xmin=1e99;
	scene->boundaries.xmax=-1e99;
	scene->boundaries.ymin=1e99;
	scene->boundaries.ymax=-1e99;
	scene->boundaries.zmin=1e99;
	scene->boundaries.zmax=-1e99;

	scene->surface = gtk_plot_surface_new();

	gtk_plot_surface_set_points(GTK_PLOT_SURFACE(scene->surface),
			x, y, z, NULL, NULL, NULL, 40, 40);
	gtk_plot_data_set_legend(GTK_PLOT_DATA(scene->surface), "cos ((r-r\\s0\\N)\\S2\\N)");

	gtk_plot_surface_use_amplitud(GTK_PLOT_SURFACE(scene->surface), TRUE);
	gtk_plot_data_set_a(GTK_PLOT_DATA(scene->surface), a);
	gtk_plot_data_gradient_set_visible(GTK_PLOT_DATA(scene->surface), TRUE);

	gtk_plot_add_data(GTK_PLOT(scene->active_plot), GTK_PLOT_DATA(scene->surface));

	gtk_plot3d_autoscale(GTK_PLOT3D(scene->active_plot));
	gtk_widget_show(scene->surface);
}

void test_gui_location()
{
	// define the mins and maxs on each axis
	scene->boundaries.xmin=0;
	scene->boundaries.xmax=ROOM_MAX_X;
	scene->boundaries.ymin=0;
	scene->boundaries.ymax=ROOM_MAX_Y;
	scene->boundaries.zmin=0;
	scene->boundaries.zmax=ROOM_MAX_Z;

	gtk_plot3d_set_xrange(GTK_PLOT3D(scene->active_plot), scene->boundaries.xmin, scene->boundaries.xmax);
	gtk_plot3d_set_yrange(GTK_PLOT3D(scene->active_plot), scene->boundaries.ymin, scene->boundaries.ymax);
	gtk_plot3d_set_zrange(GTK_PLOT3D(scene->active_plot), scene->boundaries.zmin, scene->boundaries.zmax);

	//scene->surface = gtk_plot_surface_new();

	test_gui_location_new_point(NULL, NULL);

/*
	scene->surface = gtk_plot_surface_new();

	// a surface has vertexes defined from a set of points, it is not a list of points to draw!
	gtk_plot_surface_set_points(GTK_PLOT_SURFACE(scene->surface),
			scene->x, scene->y, scene->z, NULL, NULL, NULL, 40, 40);
	gtk_plot_data_set_legend(GTK_PLOT_DATA(scene->surface), "Test location");

	gtk_plot_surface_use_amplitud(GTK_PLOT_SURFACE(scene->surface), TRUE);
	gtk_plot_data_set_a(GTK_PLOT_DATA(scene->surface), scene->a);
	gtk_plot_data_gradient_set_visible(GTK_PLOT_DATA(scene->surface), TRUE);*/

}

void test_gui_location_new_point(GtkWidget *button, gpointer data)
{
	printf("location new point\n");
	// generate a random position
	gdouble x = ((gdouble)(rand() % (4*ROOM_MAX_X))/4.0); // 0.25 unit of precision
	gdouble y = ((gdouble)(rand() % (4*ROOM_MAX_Y))/4.0); // 0.25 unit of precision
	gdouble z = ((gdouble)(rand() % (4*ROOM_MAX_Z))/4.0); // 0.25 unit of precision

	printf("add pt x %.2f y %.2f z %.2f\n", x,y,z);

	// add it into the graph //sphere
	//gtk_plot_surface_plot_square(scene->surface, x,y,z, 2.0);
	//scene__point(x,y,z);
	//gtk_plot_add_data(GTK_PLOT(scene->active_plot), GTK_PLOT_DATA(scene->surface));



	//gtk_plot_surface_plot_plane(GTK_PLOT(scene->active_plot), x,y,z, 2.0);
	GdkColor color;
	gdk_color_parse("light blue", &color);
	gdk_color_alloc(gtk_widget_get_colormap(scene->canvas), &color);

	gtk_plot_surface_plot_plane(GTK_PLOT3D(scene->active_plot), x,y,z, 2.0, color);

	//gtk_widget_show(scene->surface);
}
/*
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
*/


/**
 * Called on a click of rotateX button
 * Rotate 10 degrees counterclockwise around the X axis
 */
void scene_rotate_x(GtkWidget *button, gpointer data)
{
	gtk_plot3d_rotate_x(GTK_PLOT3D(scene->active_plot), 10.);

	gtk_plot_canvas_paint(GTK_PLOT_CANVAS(data));
	gtk_plot_canvas_refresh(GTK_PLOT_CANVAS(data));
}


/**
 * Called on a click of rotateY button
 * Rotate 10 degrees counterclockwise around the Y axis
 */
void scene_rotate_y(GtkWidget *button, gpointer data)
{
	gtk_plot3d_rotate_y(GTK_PLOT3D(scene->active_plot), 10.);

	gtk_plot_canvas_paint(GTK_PLOT_CANVAS(data));
	gtk_plot_canvas_refresh(GTK_PLOT_CANVAS(data));
}

/**
 * Called on a click of rotateX button
 * Rotate 10 degrees counterclockwise around the Z axis
 */
void scene_rotate_z(GtkWidget *button, gpointer data)
{
	gtk_plot3d_rotate_z(GTK_PLOT3D(scene->active_plot), 10.);

	gtk_plot_canvas_paint(GTK_PLOT_CANVAS(data));
	gtk_plot_canvas_refresh(GTK_PLOT_CANVAS(data));
}
#endif


void test_gui_location_2d(GtkWidget *plot)
{


	/*
	gtk_plot_set_pc(plot,GTK_PLOT_PC(gtk_plot_cairo_new_with_drawable(GTK_PLOT(plot)->drawable)));

	 gdk_color_alloc(gtk_widget_get_colormap(plot), &background);
	 	gdk_color_parse ("#444444", &background);

	 	pc = GTK_PLOT(plot)->pc;
	 	gtk_plot_pc_set_color(pc, &background);
	 	gtk_plot_pc_draw_ellipse(pc, TRUE, px, py, 2.0, 2.0);*/

	test_gui_location_2d_new_point(NULL, NULL);
}

void test_gui_location_2d_new_point(GtkWidget *button, gpointer data)
{
	printf("location new point\n");
	// generate a random position
	gdouble x = ((gdouble)(rand() % (4*ROOM_MAX_X))/4.0); // 0.25 unit of precision
	gdouble y = ((gdouble)(rand() % (4*ROOM_MAX_Y))/4.0); // 0.25 unit of precision
	gdouble z = ((gdouble)(rand() % (4*ROOM_MAX_Z))/4.0); // 0.25 unit of precision

	printf("add pt x %.2f y %.2f z %.2f\n", x,y,z);

	GdkColor color1, color2;
	static gdouble px[]={3.0};
	static gdouble py[]={3.0};
	static gdouble pz[]={4.0};
   static gdouble dx[]={0.1};
   static gdouble dy[]={0.1};

	GtkPlotPC *pc;

	GtkPlotData *dataset = GTK_PLOT_DATA(gtk_plot_box_new(GTK_ORIENTATION_VERTICAL));

	 gtk_plot_add_data(GTK_PLOT(scene->active_plot), dataset);
	 gtk_widget_show(GTK_WIDGET(dataset));
	 gtk_plot_data_set_points(dataset, px, py, dx, dy, 1);

	 gtk_plot_data_set_z(dataset, pz);

	 gtk_plot_data_show_zerrbars(dataset);

	 gdk_color_parse("red", &color1);
	 gdk_color_alloc(gdk_colormap_get_system(), &color1);
	 gdk_color_parse("yellow", &color2);
	 gdk_color_alloc(gdk_colormap_get_system(), &color2);

	 gtk_plot_data_set_symbol(dataset,
	                          GTK_PLOT_SYMBOL_CIRCLE,
				  GTK_PLOT_SYMBOL_FILLED,
	                          10, 2, &color2, &color1);
	 gtk_plot_data_set_line_attributes(dataset,
	                                   GTK_PLOT_LINE_NONE,
	                                   0, 0, 1, &color1);

	 gtk_plot_data_set_legend(dataset, "Boxes");

	 gtk_widget_show(scene->canvas);
	 gtk_widget_show(scene->active_plot);
}

/**
 * Called in the scene_init.
 * Initializes the buttons on the canvas and the related button callbacks.
 */
void scene_init_buttons()
{
	GtkWidget *button;

	#ifdef GUI_SCENE_3D
	// rotate X
	button = gtk_button_new_with_label("Rotate X");
	gtk_fixed_put(GTK_FIXED(scene->canvas), button, 80, 0);

	g_signal_connect(GTK_OBJECT(button), "clicked",
						  GTK_SIGNAL_FUNC(scene_rotate_x), scene->canvas);
	// rotate Y
	button = gtk_button_new_with_label("Rotate Y");
	gtk_fixed_put(GTK_FIXED(scene->canvas), button, 160, 0);

	g_signal_connect(GTK_OBJECT(button), "clicked",
						  GTK_SIGNAL_FUNC(scene_rotate_y), scene->canvas);
	// rotate Z
	button = gtk_button_new_with_label("Rotate Z");
	gtk_fixed_put(GTK_FIXED(scene->canvas), button, 240, 0);

	g_signal_connect(GTK_OBJECT(button), "clicked",
						  GTK_SIGNAL_FUNC(scene_rotate_z), scene->canvas);

		// add point according to the scenario
		#ifdef TEST_GUI_SCENE_LOCATION
			 button = gtk_button_new_with_label("Add point");
			 gtk_fixed_put(GTK_FIXED(scene->canvas), button, 320, 0);

			 g_signal_connect(GTK_OBJECT(button), "clicked",
									  GTK_SIGNAL_FUNC(test_gui_location_new_point), scene->canvas);
		#endif
	#elif defined (TEST_GUI_SCENE_LOCATION_2D)
		 button = gtk_button_new_with_label("Add point");
		 gtk_fixed_put(GTK_FIXED(scene->canvas), button, 320, 0);

		 g_signal_connect(GTK_OBJECT(button), "clicked",
								  GTK_SIGNAL_FUNC(test_gui_location_2d_new_point), scene->canvas);
	#endif
}


void scene_init_2d(GtkWidget *active_plot)
{
 GdkColor color1, color2;
 static gdouble px[]={0., 0.2, 0.4, 0.6, 0.8, 1.0};
 static gdouble py[]={.243,.045,.075,.213,.05,.124};
 static gdouble pz[]={.56, .12, .123, .5, .2, .21};
 static gdouble dx[]={.10, .18, .17, .16, .082, .34};
 static gdouble dy[]={.1,.1,.1,.1,.1,.1};

 GtkPlotData *dataset[5];

 dataset[0] = GTK_PLOT_DATA(gtk_plot_box_new(GTK_ORIENTATION_VERTICAL));

/*
 dataset[0] = GTK_PLOT_DATA(gtk_plot_data_new());
 gtk_plot_autoscale(GTK_PLOT(active_plot));
*/

 gtk_plot_add_data(GTK_PLOT(active_plot), dataset[0]);
 gtk_widget_show(GTK_WIDGET(dataset[0]));
 gtk_plot_data_set_points(dataset[0], px, py, dx, dy, 6);
 gtk_plot_data_set_z(dataset[0], pz);

 gtk_plot_data_show_zerrbars(dataset[0]);

 gdk_color_parse("red", &color1);
 gdk_color_alloc(gdk_colormap_get_system(), &color1);
 gdk_color_parse("yellow", &color2);
 gdk_color_alloc(gdk_colormap_get_system(), &color2);

 gtk_plot_data_set_symbol(dataset[0],
                          GTK_PLOT_SYMBOL_CIRCLE,
			  GTK_PLOT_SYMBOL_FILLED,
                          10, 2, &color2, &color1);
 gtk_plot_data_set_line_attributes(dataset[0],
                                   GTK_PLOT_LINE_NONE,
                                   0, 0, 1, &color1);
/*
 gtk_plot_data_set_x_attributes(dataset[0],
                                GTK_PLOT_LINE_SOLID,
                                0, &active_plot->style->black);
 gtk_plot_data_set_y_attributes(dataset[0],
                                GTK_PLOT_LINE_SOLID,
                                0, &active_plot->style->black);
*/

 gtk_plot_data_set_legend(dataset[0], "Boxes");

}

void scene_init(){
	scene = vp_os_malloc(sizeof (scene_t));

	scene->pixmap = NULL;
	scene->cnt = 0;
	scene->scale = 1.;

	scene->page_width = GTK_PLOT_LETTER_W * scene->scale;
	scene->page_height = GTK_PLOT_LETTER_H * scene->scale;

	// contains the whole scene into a box
	scene->box_graph=gtk_vbox_new(FALSE,5);
	// scroller along vertical and horizontal axis
	scene->scrollw1=gtk_scrolled_window_new(NULL, NULL);
	gtk_container_border_width(GTK_CONTAINER(scene->scrollw1),0);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scene->scrollw1),
				GTK_POLICY_ALWAYS,GTK_POLICY_ALWAYS);
	gtk_box_pack_start(GTK_BOX(scene->box_graph),scene->scrollw1, TRUE, TRUE,0);
	// canvas for rendering the scene connected to scrollers
	scene->canvas = gtk_plot_canvas_new(scene->page_width, scene->page_height, scene->scale);
	GTK_PLOT_CANVAS_SET_FLAGS(GTK_PLOT_CANVAS(scene->canvas), GTK_PLOT_CANVAS_DND_FLAGS);
	gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scene->scrollw1), scene->canvas);
	gtk_widget_show(scene->canvas);
	/*
	 gdk_color_parse("light blue", &color);
	 gdk_color_alloc(gtk_widget_get_colormap(canvas), &color);
	 gtk_plot_canvas_set_background(GTK_PLOT_CANVAS(canvas), &color);
	*/
   printf("creating plot\n");
   // configures the plot
	#ifdef GUI_SCENE_3D
   	scene->active_plot = gtk_plot3d_new_with_size(NULL, .70, .70);
      // we display an axis every 50cms (refering to the default initialization in gtkplot3d.c of the gtkextra lib)
      GTK_PLOT3D(scene->active_plot)->ax->ticks.step = 0.5;
      GTK_PLOT3D(scene->active_plot)->ay->ticks.step = 0.5;
      GTK_PLOT3D(scene->active_plot)->az->ticks.step = 0.5;
	#else
   	scene->active_plot = gtk_plot_new_with_size(NULL, .70, .70);

   	gtk_plot_set_range(GTK_PLOT(scene->active_plot), 0.0, ROOM_MAX_X, 0.0, ROOM_MAX_Y);
   	gtk_plot_axis_set_ticks(GTK_PLOT(scene->active_plot)->bottom, 0.5, 0.25);
   	gtk_plot_axis_set_ticks(GTK_PLOT(scene->active_plot)->left, 0.5, 0.25);
   	gtk_plot_axis_set_ticks(GTK_PLOT(scene->active_plot)->right, 0.5, 0.25);
   	gtk_plot_axis_set_ticks(GTK_PLOT(scene->active_plot)->top, 0.5, 0.25);
	#endif
   // adds a child as a new 3d figure
   scene->child = gtk_plot_canvas_plot_new(GTK_PLOT(scene->active_plot));
   gtk_plot_canvas_put_child(GTK_PLOT_CANVAS(scene->canvas), scene->child, .10, .06, .85, .85);
   gtk_widget_show(scene->active_plot);

   printf("creating test samples\n");
   // creates the scene according to the selected scenario
	#ifdef TEST_GUI_SCENE_EXAMPLE
		test_gui_scene_example();
	#elif defined (TEST_GUI_SCENE_LOCATION)
		test_gui_location();
	#elif defined (TEST_GUI_SCENE_LOCATION_2D)
		//scene_init_2d(scene->active_plot);
		test_gui_location_2d(scene->active_plot);
	#endif

	// adds the buttons (rotate, addpoint if needed) on the canvas
	scene_init_buttons();

	gtk_widget_show(scene->active_plot);
/*
	scene->active_plot = scene->plots[1];

   scene->child = gtk_plot_canvas_plot_new(GTK_PLOT(scene->active_plot));
   gtk_plot_canvas_put_child(GTK_PLOT_CANVAS(scene->canvas), scene->child, .10, .06, .85, .85);
   gtk_widget_show(scene->active_plot);
*/

	//build_example1(scene->active_plot);
	/*
	 gtk_plot_canvas_export_ps(GTK_PLOT_CANVAS(canvas), "demo3d.ps", 0, 0,
										GTK_PLOT_LETTER);
	*/

	printf("fully initialized\n");
}

void scene_destroy()
{
	vp_os_free(scene);
}


#endif

