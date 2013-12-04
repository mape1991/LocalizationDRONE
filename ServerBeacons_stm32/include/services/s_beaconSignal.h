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


#ifndef S_BEACONSIGNAL_H
#define S_BEACONSIGNAL_H


/******************************************************************************
* 
*   INCLUDED FILES
*
******************************************************************************/

#include "stdint.h"


/******************************************************************************
*
*   CONSTANTS AND ENUM TYPES
*
******************************************************************************/

/** Signal frequency PWM [KHz] */
#define FPWM1 39.25  
#define FPWM2 39.75       				 
#define FPWM3 40.25
#define FPWM4 40.75

/** Duty Cycle for PWM signal */
#define DUTYCYCLE_50 0.5
#define DUTYCYCLE_0 0.0

/** Type def for PWM State */
typedef enum
{
	PWM_PULSE,
	PWM_ZERO
	
} State_PWM;


/******************************************************************************
*
*   PRIVATE FUNCTIONS
*
******************************************************************************/

void s_beaconSignal_initialization(void);
void s_beaconSignal_emission_PWM(void);	
void s_beaconSignal_reset(void);
void s_beaconSignal_zero(void);


#endif					/* S_BEACONSIGNAL_H */
