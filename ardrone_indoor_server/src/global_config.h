/* 
 * File:   global_config.h
 * Author: julien
 *
 * Created on November 12, 2013, 8:49 AM
 */

#ifndef GLOBAL_CONFIG_H
#define	GLOBAL_CONFIG_H

/*
 * This file serves the whole project
 * If one of the definitions is commented,
 * this will disable the attached functionality
 * i.e. if GUI_ON is commented, nothing related to user interface will be activated 
 */

// DEFINE these FEATURES IN YOUR test define if you use them

// user interface
//#define GUI_ON
// debug tracking
//#define DEBUG_ON
// video
//#define VIDEO_ON
// navigation data
//#define NAV_ON
// server communication
//#define UDP_ON
// usb communication
//#define USB_ON



// Tests

// declare global test definition
//#define TEST_COMM             // test comm on
//#define TEST_WIFI_DELAY
#define TEST_GUI

// define which features to use in this test
#ifdef TEST_COMM
    #include "tests/test_comm.h"
    #define UDP_ON
    #define USB_ON
#elif defined TEST_WIFI_DELAY
    #include "tests/test_wifi_delay.h"
    #define UDP_ON
#elif defined TEST_GUI
	#define GUI_ON
#endif

#ifdef UDP_ON
#endif

#ifdef USB_ON
	#define USB_READING_OFF 0
	#define USB_READING_ON 1

	extern int is_usb_reading;
#endif


#endif	/* GLOBAL_CONFIG_H */

