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

// Tests

// declare global test definition
//#define TEST_COMM             // test comm on
//#define TEST_WIFI_DELAY
#define TEST_GUI
//#define TEST_GUI_ONLY

// define which features to use in this test
#ifdef TEST_COMM
	#define UDP_ON
	#define USB_ON
#elif defined TEST_WIFI_DELAY
	#define UDP_ON
#elif defined TEST_GUI
	#define GUI_ON
	#define UDP_ON
	#define USB_ON
#elif defined TEST_GUI_ONLY
	#define GUI_ON
#endif

// global communication definitions
#ifdef UDP_ON
	#define DEST_IP_DRONE "192.168.1.1"

	#define UDP_LISTEN_ON    1
	#define UDP_LISTEN_OFF   0

	extern int is_udp_listening;

	#define UDP_SEND_ON      1
	#define UDP_SEND_OFF     0

	extern int is_udp_sending;
#endif

#ifdef USB_ON
	#define USB_READING_OFF 0
	#define USB_READING_ON 1

	extern int is_usb_reading;
#endif

#endif	/* GLOBAL_CONFIG_H */

