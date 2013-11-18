/**
 * @file main.c
 * @brief Main function core
 *
 *      This file contains the main function that initializes, configurates
 * 			and start all applications with their services. 
 *
 *	Last modification : 18 Nov 2013 
 *
 * @author Martin
 * @version 0.0.3
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
 * @sa main()
 ******************************************************************************/
 
void app_updateGUI()		
{
	switch (state)
	{
		case APP_ON:
			s_serialComm_sendChar(MSG_ON);
			break;
		case APP_START:
			s_serialComm_sendChar(MSG_START);	
			break;
		case APP_STOP:
			s_serialComm_sendChar(MSG_STOP);
			break;
		case APP_OFF:
		default:	
			break;
	}
}


/**
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
	if (c == MSG_ON)
	{
		if (state == APP_OFF)
		{
			prev_state = state;
			state = APP_ON;
		}
	}
	else if (c == MSG_START)
	{
		if (state != APP_OFF)
		{
			prev_state = state;
			state = APP_START;
			s_beaconSignal_reset();
			SysTick_On;
			SysTick_Enable_IT;
		}
	}
	else if (c == MSG_STOP)
	{
		if (state == APP_START)
		{
			prev_state = state;
			state = APP_STOP;
			s_beaconSignal_zero();
			SysTick_Disable_IT;
			SysTick_Off;
		}
	}
	else
	{
		prev_state = state;
		state = APP_OFF;
		s_beaconSignal_zero();
		SysTick_Disable_IT;
		SysTick_Off;
	}
}


/**
 *******************************************************************************
 * app_commBeacons
 *
 *      Called when timeout to systick timer
 *			Emission PWM to every Beacon for a pre-defined period of time
 * 			
 * @sa main()
 ******************************************************************************/
 
void app_commBeacons(void)		
{
	// PWM emission for pre-defined time interval [ms] 
	s_beaconSignal_emission_PWM();
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
	
	// TODO : error process (next version)
	
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
		// updateGUI
		app_updateGUI();
	}
		
	return 0;
}


