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
// debug tracking
#define DEBUG_ON
// udp communication
//#define UDP_ON

// Tests
#define TEST_COMM             // test comm on
//#define TEST_WIFI_DELAY

#ifdef TEST_COMM
    #define UDP_ON
#elif defined TEST_WIFI_DELAY
    #define UDP ON
#endif



#endif	/* GLOBAL_CONFIG_H */

