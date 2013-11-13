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
#define GUI_ON
// debug tracking
#define DEBUG_ON
// video
//#define VIDEO_ON
// navigation data
//#define NAV_ON
// server communication
#define SERVER_COMM_ON
// drone communication
//#define DRONE_COMM_ON


// Tests
#define TEST_GUI_STANDALONE

#endif	/* GLOBAL_CONFIG_H */

