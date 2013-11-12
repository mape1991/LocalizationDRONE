/**
 * @file s_beaconSignal.c
 * @brief Beacon signal emission file core
 *
 *      This file contains the SERVICE beacon signal emission.
 *			Initializes, configurates and activates timers with PWM mode. 
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


/******************************************************************************
*
*   GLOBAL VARIABLES
*
******************************************************************************/
	
/** Clock counter */
u8 count_clock;


/******************************************************************************
*
*   PRIVATE FUNCTIONS
*
******************************************************************************/


/**
 *******************************************************************************
 * Initialization
 *
 *      Configure all clocks and registers.
 *			Initialize all GPIOs and Timers
 * 			
 * @param void
 * @return void
 ******************************************************************************/

void s_beaconSignal_Initialization(void)
{
	vu16 Resolution;	
	count_clock=0;
		
	// GPIOs configuration 
	GPIO_Configure(GPIOB, 0, OUTPUT, ALT_PPULL); 		// PWM
	GPIO_Configure(GPIOA, 2, OUTPUT, ALT_PPULL); 	// PWM 
	GPIO_Configure(GPIOA, 8, OUTPUT, OUTPUT_PPULL);	// LED (On/Off)
	GPIO_Set(GPIOA,8);	// set ON
	
	// PWM signals configuration (alpha) and (alpha-1) 
	Resolution = PWM_Init(TIM3, 3, s_beaconSignal_Fpwm1);	// timer1 channel2 configuration : mode PWM with Fpwm 
	Resolution = Resolution + 1;	// (ARRresolution + 1)
	PWM_Valeur(TIM3,3)=(Resolution*s_beaconSignal_DutyCycle);
	
	// PWM signals configuration (alpha) and (alpha-1) 
	Resolution = PWM_Init(TIM2, 3, s_beaconSignal_Fpwm2);	// timer1 channel2 configuration : mode PWM with Fpwm 
	Resolution = Resolution + 1;	// (ARRresolution + 1)
	PWM_Valeur(TIM2,3)=(Resolution*s_beaconSignal_DutyCycle);
}


/**
 *******************************************************************************
 * Emission_PWM
 *
 *			Emission PWM to every Beacon for a pre-defined period of time
 * 			Mode ON on each GPIO (output with PWM signal)
 * 			
 * @param int time of emission in [ms]
 * @return void
 ******************************************************************************/
 
void s_beaconSignal_Emission_PWM(int time)		
{
	// toggle LED 
	GPIO_Toggle(GPIOA,8);
}
	
