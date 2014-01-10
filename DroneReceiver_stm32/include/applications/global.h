/**
 * @file global.h
 * @brief Global file header
 *
 *      This file contains global variables and constants used in 
 *			the application.  
 *
 *			Last modification : 12 Dec 2013 
 *
 * @author Martin
 * @version 0.2.0
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

/** Frame size for USART Protocol bet*/
#define SIZE_FRAME 17

/** Type def for Application State */
typedef enum
{
	APP_OFF,
	APP_RECEIVE
	
} State_APP;

/** Type def for Filters */
typedef enum
{
	Filter_ON,
	Filter_OFF
	
} State_Filter;

/** Type def for Input conversion (int to string) */
typedef enum
{
	TOA1,
	TOA2,
	TOA3,
	TOA4

} State_Input_Comm;

/** Protocol Messages between the Client and the Server */
#define MSG_OFF 'X'			// Exit
#define MSG_SEND 'S'		// Send
#define MSG_BUSY 'B'		// Busy
#define MSG_NULL 0		// NULL

#endif					/* GLOBAL_H */

