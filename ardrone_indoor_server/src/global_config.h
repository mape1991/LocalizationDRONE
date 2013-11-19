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
// user interface
//#define GUI_ON
   // user interface version
   //#define GUI_VERSION_USER
   //#define GUI_VERSION_TEST
// debug tracking
#define DEBUG_ON
// video
//#define VIDEO_ON
// navigation data
//#define NAV_ON
// server communication
#define UDP_ON
#define USB_ON

// Tests
#define TEST_COMM             // test comm on



#endif	/* GLOBAL_CONFIG_H */

