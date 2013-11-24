/**
 * @file global.h
 * @brief Global file header
 *
 *      This file contains global variables and constants used in 
 *			the application.  
 *
 * @author Martin
 * @version 0.0.1
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
	APP_START,
	APP_STOP
	
} State_APP;

/** Protocol Messages send to UART Server */
#define MSG_ON 'I'
#define MSG_OFF 'X'
#define MSG_START 'S'
#define MSG_STOP 'P'


#endif					/* GLOBAL_H */

