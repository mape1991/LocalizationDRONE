/**
 * @file s_beaconSignal.c
 * @brief Beacon signal emission file core
 *
 *      This file contains the SERVICE beacon signal emission.
 *			Initializes, configurates and activates timers with PWM mode. 
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
#include "Timer_1234.h"
#include "GPIO.h"
#include "s_beaconSignal.h"


/******************************************************************************
*
*   GLOBAL VARIABLES
*
******************************************************************************/

/** Timer's Resolution */
vu16 resolution_pwm1;
vu16 resolution_pwm2;
vu16 resolution_pwm3;
vu16 resolution_pwm4;

/** pwm state */
State_PWM state_pwm = PWM_ZERO;


/******************************************************************************
*
*   PRIVATE FUNCTIONS
*
******************************************************************************/


/**
 *******************************************************************************
 * s_beaconSignal_initialization
 *
 *      Configure all clocks and registers.
 *			Initialize all GPIOs and Timers
 * 			
 * @param void
 * @return void
 ******************************************************************************/

void s_beaconSignal_initialization(void)
{	
	// GPIOs configuration 
	GPIO_Configure(GPIOA, 8, OUTPUT, ALT_PPULL); 		// PWM	
	GPIO_Configure(GPIOA, 2, OUTPUT, ALT_PPULL); 		// PWM
	GPIO_Configure(GPIOB, 0, OUTPUT, ALT_PPULL); 		// PWM 
	GPIO_Configure(GPIOB, 6, OUTPUT, ALT_PPULL); 		// PWM 
	
	// PWM signals configuration
	resolution_pwm1 = PWM_Init(TIM1, 1, FPWM1);	// timer1 channel1 configuration : mode PWM with Fpwm 
	resolution_pwm1 = resolution_pwm1 + 1;	// (ARRresolution + 1)
	
	resolution_pwm2 = PWM_Init(TIM2, 3, FPWM2);	// timer2 channel3 configuration : mode PWM with Fpwm 
	resolution_pwm2 = resolution_pwm2 + 1;	// (ARRresolution + 1)
	
	resolution_pwm3 = PWM_Init(TIM3, 3, FPWM3);	// timer3 channel3 configuration : mode PWM with Fpwm 
	resolution_pwm3 = resolution_pwm3 + 1;	// (ARRresolution + 1)
	
	resolution_pwm4 = PWM_Init(TIM4, 1, FPWM4);	// timer4 channel1 configuration : mode PWM with Fpwm 
	resolution_pwm4 = resolution_pwm4 + 1;	// (ARRresolution + 1)
}


/**
 *******************************************************************************
 * s_beaconSignal_emission_PWM
 *
 *			Emission PWM to every Beacon for a pre-defined period of time
 * 			Mode ON on each GPIO (output with PWM signal)
 * 			
 * @param int time of emission in [ms]
 * @return void
 ******************************************************************************/
 
void s_beaconSignal_emission_PWM()		
{
	// pwm emission
	switch (state_pwm)
	{
		case PWM_PULSE:
		PWM_Valeur(TIM1,1)=(resolution_pwm1*DUTYCYCLE_50);
		PWM_Valeur(TIM2,3)=(resolution_pwm2*DUTYCYCLE_50);
		PWM_Valeur(TIM3,3)=(resolution_pwm3*DUTYCYCLE_50);
		PWM_Valeur(TIM4,1)=(resolution_pwm4*DUTYCYCLE_50);
		state_pwm = PWM_ZERO;
		break;
		
		case PWM_ZERO:
		PWM_Valeur(TIM1,1)=(resolution_pwm1*DUTYCYCLE_0);
		PWM_Valeur(TIM2,3)=(resolution_pwm2*DUTYCYCLE_0);
		PWM_Valeur(TIM3,3)=(resolution_pwm3*DUTYCYCLE_0);
		PWM_Valeur(TIM4,1)=(resolution_pwm4*DUTYCYCLE_0);
		state_pwm = PWM_PULSE;
		break;
		
		default:
		PWM_Valeur(TIM1,1)=(resolution_pwm1*DUTYCYCLE_50);
		PWM_Valeur(TIM2,3)=(resolution_pwm2*DUTYCYCLE_50);
		PWM_Valeur(TIM3,3)=(resolution_pwm3*DUTYCYCLE_50);
		PWM_Valeur(TIM4,1)=(resolution_pwm4*DUTYCYCLE_50);
		state_pwm = PWM_ZERO;
	}
}	


/**
 *******************************************************************************
 * s_beaconSignal_reset
 *
 *			Reset initial conditions in beaconSignal service
 *			State PWM in mode ZERO
 * 			
 ******************************************************************************/
 
void s_beaconSignal_reset(void)
{
	state_pwm = PWM_ZERO;
	
	// PWM emission before the first Systick interruption
	PWM_Valeur(TIM1,1)=(resolution_pwm1*DUTYCYCLE_50);
	PWM_Valeur(TIM2,3)=(resolution_pwm2*DUTYCYCLE_50);
	PWM_Valeur(TIM3,3)=(resolution_pwm3*DUTYCYCLE_50);
	PWM_Valeur(TIM4,1)=(resolution_pwm4*DUTYCYCLE_50);
}


/**
 *******************************************************************************
 * s_beaconSignal_pwm
 *
 *			beaconSignal service in mode ZERO
 *			State PWM in mode PULSE for next switch
 * 			
 ******************************************************************************/
 
void s_beaconSignal_zero(void)
{
	// PWM emission before the first Systick interruption
	PWM_Valeur(TIM1,1)=(resolution_pwm1*DUTYCYCLE_0);
	PWM_Valeur(TIM2,3)=(resolution_pwm2*DUTYCYCLE_0);
	PWM_Valeur(TIM3,3)=(resolution_pwm3*DUTYCYCLE_0);
	PWM_Valeur(TIM4,1)=(resolution_pwm4*DUTYCYCLE_0);
	
	state_pwm = PWM_PULSE;
}