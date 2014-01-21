/**
 * @file s_serialComm.c
 * @brief Serial Communication Protocol file core
 *
 *      This file contains the SERVICE serial communication RS232 (UART).
 *			Initialization, configuration and send/receive mode. 
 *
 * 			Last modification : 30 Nov 2013 
 *
 * @author Martin
 * @version 0.1.1
 * @date 08 Nov 2013
 */


/******************************************************************************
* 
*   INCLUDED FILES
*
******************************************************************************/

#include "stm32f10x.h"
#include "gpio.h"
#include "usart.h"
#include "s_serialComm.h"


/******************************************************************************
*
*   PRIVATE FUNCTIONS
*
******************************************************************************/

/**
 *******************************************************************************
 * s_serialComm_it_function
 *
 *      IT Function called when UART interruption takes place
 *			Get received character from the UART register
 *			Call function in the main application in order to process the
 *			received data. 
 * 			
 ******************************************************************************/
 
void s_serialComm_it_function(void)
{
	char data =0;

	// get received data
	data = Data_USART(USART1) ;
	
	if ((data != 0) && (s_serialComm_pFct != 0))
	{
			// call function in the application to analyze data
			(*s_serialComm_pFct)(data);
	}
	else
	{
		s_serialComm_setErrorLED(LED_ON);
	}
}


/**
 *******************************************************************************
 * s_serialComm_initialization
 *
 *      Configure all clocks and registers.
 *			Initialize all GPIOs and Timers
 * 			
 * @param void
 * @return 0 if no error
 * @return 1 if error takes place in the initialization
 ******************************************************************************/

char s_serialComm_initialization(void (*ptrFunction) (char))
{
	char code_Erreur = 0; 

	// I/O configuration for Error_Code
	GPIO_Configure(GPIOB, 8, OUTPUT, OUTPUT_PPULL);	// USART ERROR LED (On/Off)
	
	s_serialComm_setErrorLED(LED_OFF);
	
	if ((code_Erreur = Init_USART (USART1, BAUD_RATE, DISABLED, STOP_BIT_MODE1, WORD_LENGTH_8BITS)) != 0 )
	{
		// error in initialization process
		return code_Erreur;
	}
	
	// I/O configuration for Tx
	GPIO_Configure(GPIOA, 9, OUTPUT, ALT_PPULL);	// Tx 
	GPIO_Configure(GPIOA, 11, OUTPUT, OUTPUT_PPULL);	// CTS 
	
	// I/O configuration for Rx
	GPIO_Configure(GPIOA, 10, INPUT, INPUT_FLOATING);	// Rx 
	GPIO_Configure(GPIOA, 12, INPUT, INPUT_FLOATING);	// RTS 
	
	// active interruption on serial comm (for reception)
	Active_Interrupt_Received_Data_available (USART1, UART_HANDLER_PRIORITY, s_serialComm_it_function);
	// configure pointer to call function in the application
	if (ptrFunction != 0)
	{
		s_serialComm_pFct = ptrFunction;
	}
	else
	{
		code_Erreur = 1;
	}
	
	return code_Erreur;
}


/**
 *******************************************************************************
 * s_serialComm_sendChar
 *
 *			Send character to USART serie protocol
 * 			
 * @param char to send  
 * @return 0 if no error
 * @return 1 if error during send
 ******************************************************************************/
	
char s_serialComm_sendChar(char c)
{ 		
	char code_Erreur = 0;
	
	GPIO_Set(GPIOA,11);	// set ON
	
	if ( (code_Erreur = Send_Data_USART(USART1, c)) != 0 )
	{
		// error when send data string
		GPIO_Clear(GPIOA,11);	// set OFF
		return code_Erreur;
	}
	
	GPIO_Clear(GPIOA,11);	// set OFF 
	return code_Erreur;
}


/**
 *******************************************************************************
 * s_serialComm_sendString
 *
 *			Send words (strings) to USART serie protocol
 * 			
 * @param string to send  
 * @return 0 if no error
 * @return 1 if error during send
 ******************************************************************************/
	
char s_serialComm_sendString(char string[], u16 size)
{ 		
	char code_Erreur = 0;
	int i = 0;
	
	GPIO_Set(GPIOA,11);	// set ON
	
	for (i=0; i < size; i++ )
	{
			// Affichage du caractere 
			if ((code_Erreur = Send_Data_USART(USART1, string[i])) != 0)
			{
				// error when send data string
				GPIO_Clear(GPIOA,11);	// set OFF 
				return code_Erreur;
			}
	} 
	
	GPIO_Clear(GPIOA,11);	// set OFF 
	return code_Erreur;
}


/**
*******************************************************************************
* s_serialComm_setErrorLED
*
*			Set ON the error LED
* 			
******************************************************************************/

void s_serialComm_setErrorLED(State_LED state)
{
	switch(state)
	{
		case LED_ON:
			GPIO_Set(GPIOB,8);	// set error LED ON
			break;
		case LED_OFF:
			GPIO_Clear(GPIOB,8);	// set error LED OFF
			break;
		default:
			GPIO_Set(GPIOB,8);	// set error LED ON
	}
}