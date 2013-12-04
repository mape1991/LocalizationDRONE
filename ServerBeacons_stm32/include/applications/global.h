/**
 * @file global.h
 * @brief Global file header
 *
 *      This file contains global variables and constants used in 
 *			the application.  
 *
 *			Last modification : 30 Nov 2013 
 *
 * @author Martin
 * @version 0.1.1
 * @date 14 Nov 2013
 */


#ifndef GLOBAL_H
#define GLOBAL_H


/******************************************************************************
* 
*   INCLUDED FILES
*
******************************************************************************/

#include "stm32f10x.h"
#include "stdint.h"


/******************************************************************************
*
*   CONSTANTS
*
******************************************************************************/ 

/** Systick period in [us]*/
#define SYSTICK_PERIOD 100000.0

/** priority for beacons task */
#define PRIORITY_BEACONS 1

/** Type def for Application State */
typedef enum
{
	APP_ON,
	APP_OFF,
	APP_SEND
	
} State_APP;

/** Protocol Messages between the Client and the Server */
#define MSG_ON 'I'			// Init
#define MSG_OFF 'X'			// Exit
#define MSG_SEND 'S'		// Send
#define MSG_BUSY 'B'		// Busy


#endif					/* GLOBAL_H */

