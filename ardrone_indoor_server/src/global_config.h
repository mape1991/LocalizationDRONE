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
#define DEBUG_ON
	//#define TEST_COMM             // test comm on
	//#define TEST_WIFI_DELAY
	//#define TEST_GUI
   //#define TEST_GUI_ONLY
	//#define TEST_USB
	#define TEST_FULL

// define which features to use in this test
#ifdef TEST_COMM
	#define UDP_ON
	#define USB_ON
#elif defined TEST_WIFI_DELAY
	#define UDP_ON
#elif defined(TEST_GUI) || defined(TEST_FULL)
	#define GUI_ON
	#define UDP_ON
	#define USB_ON
#elif defined TEST_GUI_ONLY
	#define GUI_ON
	/** display a scene for graphic drone position tracking */
	#define GUI_SCENE_ON

	//#define TEST_GUI_SCENE_EXAMPLE
	#define TEST_GUI_SCENE_LOCATION
	//#define TEST_GUI_SCENE_LOCATION_2D

	/**
	 * we decided to make a 3d view of the environment.
	 * it appeared not to be emphasizing enough the drone position and
	 * we had trouble implementing the view (a simple sphere for a drone position at a specific time)
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

	extern int is_udp_listening;
	extern int is_udp_sending;
#endif

#ifdef USB_ON
	extern int is_usb_reading;
#endif

#endif	/* GLOBAL_CONFIG_H */

