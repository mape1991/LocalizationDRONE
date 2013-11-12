/**
 * @file s_beaconSignal.h
 * @brief Beacon signal emission file header
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

#include "stm32f10x.h"
#include "stdint.h"
#include "Clock.h"
#include "Timer_Systick.h"
#include "Timer_1234.h"
#include "GPIO.h"


/******************************************************************************
*
*   CONSTANTS
*
******************************************************************************/

/** Signal frequency PWM [KHz] */
#define s_beaconSignal_Fpwm1 39.5       				 
#define s_beaconSignal_Fpwm2 40.5    

/** Signal period PWM	[us] */
#define s_beaconSignal_Tpwm 1000/Fpwm

/** Duty Cycle for PWM signal */
#define s_beaconSignal_DutyCycle 0.5


/******************************************************************************
*
*   PRIVATE FUNCTIONS
*
******************************************************************************/

void s_beaconSignal_Initialization(void);
void s_beaconSignal_Emission_PWM(int);	

