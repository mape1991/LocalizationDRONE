/**
 * @file fir_filters.h
 * @brief Filters FIR header file
 *
 *      This file contains the filters corresponding to each frequency (beacon id)
 *			They have 256 values that describe a sin signal with the corresponding frequency
 *			of each beacon [39.25, 29.75, 40.25, 40.75] Khz
 *
 * 			Last modification : 16 Jan 2014 
 *
 * @author Martin
 * @version 0.0.1
 * @date 16 Jan 2014
 */

#ifndef _FIR_FILTERS_H__
#define _FIR_FILTERS_H__


/******************************************************************************
* 
*   INCLUDED FILES
*
******************************************************************************/

#include "stdint.h"
#include "stdlib.h"
#include "global.h"


/******************************************************************************
* 
*   FUNCTIONS
*
******************************************************************************/

void initTab_E (int32_t *signal_input, int size);
int32_t filter_output(int32_t *signal_input, const int32_t *filter_x_input);


#endif
