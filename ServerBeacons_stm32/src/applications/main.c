/**
 * @file main.c
 * @brief Main function core
 *
 *      This file contains the main function that initializes, configurates
 * 			and start all applications with their services. 
 *
 * @author Martin
 * @version 0.0.1
 * @date 08 Nov 2013
 */


/******************************************************************************
* 
*   INCLUDED FILES
*
******************************************************************************/

#include "s_beaconSignal.h"
#include "stm32f10x.h"
#include "Clock.h"
#include "Timer_Systick.h"
#include "stdint.h"
#include "stdio.h"


/******************************************************************************
*
*   LOCAL VARIABLES AND CONSTANTS
*
******************************************************************************/

/** Systick period in [us]*/
#define SYSTICK_PERIOD 250000.0

/** priority for beacons task */
#define PRIORITY_BEACONS 2

/** Length of beacons signal in [ms] */
#define SIGNAL_LENGTH_BEACONS 10


/******************************************************************************
*
*   PRIVATE FUNCTIONS
*
******************************************************************************/

/**
 *******************************************************************************
 * commBeacons
 *
 *      Called when timeout to systick timer
 *			Emission PWM to every Beacon for a pre-defined period of time
 * 			
 *
 * @sa main()
 ******************************************************************************/
 
void signalBeacons(void)		
{
	// PWM emission for pre-defined time interval [ms] 
	s_beaconSignal_Emission_PWM(SIGNAL_LENGTH_BEACONS);
}


/**
 *******************************************************************************
 * Initialization
 *
 *      Configure all clocks and registers.
 *			Initialize all Services
 * 			
 *
 * @sa main()
 ******************************************************************************/

void Initialization(void)
{	
	vu16 duree;
	
	// Clock's configuration
	CLOCK_Configure();		// System clock initialization
												// If STM32F107 defined as target, CPU clock = 50MHz
												// Défaut (STM32F103), CPU clock = 40MHz
	
	// beacon Signal service initialization
	s_beaconSignal_Initialization();
	
	// Systick clock configuration
	duree = Systick_Period(SYSTICK_PERIOD); //[us]
	Systick_Prio_IT(PRIORITY_BEACONS, signalBeacons); // Priority 2 for Emission_PWM
	SysTick_On;									// mode ON
}


/******************************************************************************
*
*   MAIN
*
******************************************************************************/

int main (void)
{	
	// Initialization
	Initialization();
	
	// Enable Systick timer
	SysTick_Enable_IT;					
		
	// Infinity loop 
	while(1)
	{
		// TODO 
		// execution of something for the rest of the time (~250 ms)
	}
	
		
	return 0;
}


