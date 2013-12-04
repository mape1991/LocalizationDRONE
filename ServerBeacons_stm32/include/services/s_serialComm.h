/**
 * @file s_serialComm.h
 * @brief Serial Communication Protocol header file
 *
 *      This file contains the SERVICE serial communication RS232 (UART).
 *			Initialization, configuration and send/receive mode. 
 *
 *	Last modification : 18 Nov 2013 
 *
 * @author Martin
 * @version 0.0.2
 * @date 08 Nov 2013
 */


#ifndef S_SERIALCOMM_H
#define S_SERIALCOMM_H


/******************************************************************************
* 
*   INCLUDED FILES
*
******************************************************************************/

#include "stdint.h"


/******************************************************************************
*
*   CONSTANTS
*
******************************************************************************/ 

/** Baudrate for UART communication */
#define BAUD_RATE 9600

/** Communication UART Interruption Priority */
#define UART_HANDLER_PRIORITY 0

/** Type def for ERROR LED STATE */
typedef enum
{
	LED_ON,
	LED_OFF
	
} State_LED;

/******************************************************************************
*
*   POINTER FUNCTIONS
*
******************************************************************************/

/** Function pointer to call function from the application */
void (* s_serialComm_pFct) (char);


/******************************************************************************
*
*   PRIVATE FUNCTIONS
*
******************************************************************************/

void s_serialComm_it_function(void);
char s_serialComm_initialization(void (*ptrFunction) (char));
char s_serialComm_sendChar(char c);
char s_serialComm_sendString(char string[]);
void s_serialComm_setErrorLED(State_LED);


#endif					/* S_SERIALCOMM_H */
