/**
 * @file main.c
 * @brief Main function core
 *
 *      This file contains the main function that initializes, configurates
 * 			and start all applications with their services. 
 *
 *			Last modification : 30 Nov 2013 
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
#include "Clock.h"
#include "Timer_Systick.h"
#include "stdio.h"
#include "s_beaconSignal.h"
#include "s_serialComm.h"
#include "global.h"


/******************************************************************************
*
*   GLOBAL VARIABLES
*
******************************************************************************/

/** application state */
State_APP state = APP_OFF;
State_APP prev_state = APP_OFF;


/******************************************************************************
*
*   PRIVATE FUNCTIONS
*
******************************************************************************/

/**
 *******************************************************************************
 * app_updateGUI
 *
 *      Update GUI with special character
 *			Called in the main function
 *			Send special character that depends from the app state
 * 			
 * @param void
 * @return 0 if no error
 * @return 1 if error takes place in send char			
 * @sa main()
 ******************************************************************************/
 
char app_updateGUI(void)		
{
	char code_erreur = 0;
	
	switch (state)
	{
		case APP_ON:
			code_erreur = s_serialComm_sendChar(MSG_ON);
			break;
		case APP_SEND:
			code_erreur = s_serialComm_sendChar(MSG_BUSY);
			break;
		case APP_OFF:
			code_erreur = s_serialComm_sendChar(MSG_OFF);
			break;
		default:
			code_erreur = 1; 		
	}
	
	return code_erreur;
}


/*
 *******************************************************************************
 * app_serialCommHandler
 *
 *      Called when serial comm interruption takes place
 *			Analyze received character and update app state
 * 			If character is not valid (protocol not respected)
 *			then the application state rest the same as before
 *
 * @sa main()
 ******************************************************************************/
 
void app_serialCommHandler(char c)		
{
	char error = 0;;
	
	if (c == MSG_ON)
	{
		if (state == APP_OFF)
		{
			prev_state = state;
			state = APP_ON;
		}
		error = app_updateGUI();
	}
	else if (c == MSG_SEND)
	{
		if (state == APP_ON)
		{
			prev_state = state;
			state = APP_SEND;
			s_beaconSignal_reset();
			SysTick_On;
			SysTick_Enable_IT;
		}
		else
		{
			error = app_updateGUI();
		}
	}
	else if (c == MSG_OFF)
	{
		if (state != APP_OFF)
		{
			prev_state = state;
			state = APP_OFF;
			s_beaconSignal_zero();
			SysTick_Disable_IT;
			SysTick_Off;
		}
		error = app_updateGUI();
	}
	else
	{
		error = app_updateGUI();
	}
	
	if (error != 0)
	{
		// update Status LED
		s_serialComm_setErrorLED(LED_ON);
	}
}


/**
 *******************************************************************************
 * app_commBeacons
 *
 *      Called when timeout to systick timer
 *			Update State (character send)
 *			Disable Systick interruption system
 *			Update GUI ('S')
 * 			
 * @sa main()
 ******************************************************************************/
 
void app_commBeacons(void)		
{
	prev_state = state;
	state = APP_ON;
	s_beaconSignal_zero();
	SysTick_Disable_IT;
	SysTick_Off;
	
	if (s_serialComm_sendChar(MSG_SEND) != 0)
	{
		// ERROR LED
		s_serialComm_setErrorLED(LED_ON);
	}
	
}


/**
 *******************************************************************************
 * app_initialization
 *
 *      Configures all clocks and registers.
 *			Initializes all Services
 * 			Configures Systick Timer
 *
 * @sa main()
 ******************************************************************************/

void app_initialization(void)
{	
	vu16 duree;
	char code_Erreur = 0;
	
	// Clock's configuration
	CLOCK_Configure();		// System clock initialization
												// If STM32F107 defined as target, CPU clock = 50MHz
												// Défaut (STM32F103), CPU clock = 40MHz
	
	// service initialization
	s_beaconSignal_initialization();
	code_Erreur = s_serialComm_initialization(app_serialCommHandler);
	if (code_Erreur != 0)
	{
		s_serialComm_setErrorLED(LED_ON);
	}
	
	// Systick clock configuration
	duree = Systick_Period(SYSTICK_PERIOD); //[us]
	Systick_Prio_IT(PRIORITY_BEACONS, app_commBeacons); // Priority 2 for Emission_PWM
}


/******************************************************************************
*
*   MAIN
*
******************************************************************************/

int main (void)
{	
	// Initialization
	app_initialization();

	// Infinity loop 
	while(1)
	{
	}
		
	return 0;
}
