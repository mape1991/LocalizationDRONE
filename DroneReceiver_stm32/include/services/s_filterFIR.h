/**
 * @file s_filterFIR.c
 * @brief Filter FIR header file
 *
 *      This file contains the filter API for each iteration
 *			Initialization, configuration and computing the iteration 
 *
 * 			Last modification : 12 Dec 2013 
 *
 * @author Martin
 * @version 0.0.1
 * @date 12 Dec 2013
 */


#ifndef S_FILTERFIR_H
#define S_FILTERFIR_H


/******************************************************************************
* 
*   INCLUDED FILES
*
******************************************************************************/

#include "stdint.h"
#include "fir_filters.h"
#include "global.h"


/******************************************************************************
*
*   CONSTANTS
*
******************************************************************************/ 

/** EOC ADC Interruption Priority */
#define ADC_HANDLER_PRIORITY 1

/** ADC Channel */
#define CHANNEL_ADC 10


/******************************************************************************
*
*   PRIVATE FUNCTIONS
*
******************************************************************************/

void s_filterFIR_it_function(Stop_Signal signal);
void s_filterFIR_IT_ADC (void);
void s_filterFIR_computeOutputs(void);
void s_filterFIR_startReception(void);
char s_filterFIR_initialization(void);
void s_filterFIR_computeOutputs(void);
void s_filterFIR_IT_samplecount(void);

#endif					/* S_SERIALCOMM_H */
