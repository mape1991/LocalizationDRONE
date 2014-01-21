/* 
 * File:   global_config.h
 * Author: julien
 *
 * Created on November 12, 2013, 8:49 AM
 */

#ifndef GLOBAL_CONFIG_H
#define GLOBAL_CONFIG_H

/*
 * This file serves the whole project
 * If one of the definitions is commented,
 * this will disable the attached functionality
 * i.e. if GUI_ON is commented, nothing related to user interface will be activated 
 */

// DEFINE these FEATURES IN YOUR test define if you use them

// Tests

// declare global test definition
#define DEBUG_ON // this constant is not implemented... should uncomment the printf when debugging
	//#define TEST_COMM             // test comm on
	//#define TEST_WIFI_DELAY
	//#define TEST_GUI
   //#define TEST_GUI_ONLY
	//#define TEST_USB
	#define TEST_FULL
// test_full with double sized packet
#define TEST_FULL_DOUBLE_SIZE

// define which features to use in this test
#ifdef TEST_COMM
	#define UDP_ON
	#define USB_ON
#elif defined TEST_WIFI_DELAY
	#define UDP_ON
#elif defined(TEST_GUI) || defined(TEST_FULL) || defined (TEST_GUI_ONLY)

	#ifndef TEST_GUI_ONLY
		#define UDP_ON
		#define USB_ON
	#endif

	#define GUI_ON
	#define GUI_SCENE_ON

	//#define TEST_GUI_SCENE_EXAMPLE
	#define TEST_GUI_SCENE_LOCATION
	//#define TEST_GUI_SCENE_LOCATION_2D

	/**
	 * we decided to make a 3d view of the environment.
	 * it appeared not to be emphasizing enough the drone position and
	 * we had trouble implementing the view (the gtkextra component expects an input static dataset
	 * and does not update the view with input positions!!!)
	 * this improvement could be reviewed for next developments.
	 * In the meantime, for simplicity and deadlines, we use a 2d canvas for displaying a XxY space
	 * and a side verticalbar for the Z dimension
	 */
	#ifndef TEST_GUI_SCENE_LOCATION_2D
		#define GUI_SCENE_3D
	#endif
#elif defined TEST_USB
	#define USB_ON
#endif

// global communication definitions
#ifdef UDP_ON
	#define DEST_IP_DRONE "192.168.1.1"
#endif

#endif	/* GLOBAL_CONFIG_H */

