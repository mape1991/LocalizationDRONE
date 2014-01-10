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
//#include "FIR_Filter.h"


/******************************************************************************
*
*   CONSTANTS
*
******************************************************************************/ 

/** ADC Sample time [us] */
#define SAMPLE_TIME_ADC 7.8125

/** Sampling Frequency [KHz] */
#define SAMPLE_FREQUENCY 128.0

/** EOC ADC Interruption Priority */
#define ADC_HANDLER_PRIORITY 1

/** ADC Channel */
#define CHANNEL_ADC 10

/** Number of samples saved in filter tables and ADC samples tables */
#define SIZE_TABLE_SAMPLES 256

/** Number of samples for 100 ms with Fe=128KHz */
#define NB_SAMPLES_TOTAL 12800

/** Number of samples for one block that updates the filter Input Table */
#define SAMPLE_BLOCK_SIZE 64


/******************************************************************************
*
*   POINTER FUNCTIONS
*
******************************************************************************/


/******************************************************************************
*
*   PRIVATE FUNCTIONS
*
******************************************************************************/
char s_filterFIR_initialization(void);

//void s_filterFIR_computeIter(void);

#endif					/* S_SERIALCOMM_H */
