/**
 * @file fir_filters.c
 * @brief Filters FIR source file
 *
 *      This file contains the functions used for filters correlation
 *			Multiplication filter-signal
 *			Init table of N size with zeros
 *
 * 			Last modification : 16 Jan 2014 
 *
 * @author Martin
 * @version 0.0.1
 * @date 16 Jan 2014
 */
 
/******************************************************************************
* 
*   INCLUDED FILES
*
******************************************************************************/

#include "fir_filters.h"


/******************************************************************************
* 
*   INITIALIZE GLOBAL VARIABLES
*
******************************************************************************/



/******************************************************************************
* 
*   FUNCTIONS
*
******************************************************************************/

/**
 *******************************************************************************
 * init_Tab_E
 *
 *      Initialize table with zeros 
 * 			
 * @param Signal Input table pointer (with size = size param)
 * @param int signal input table size
 * @return void
 ******************************************************************************/
 
void initTab_E (int32_t *signal_input, int size)
{
	int i=0;
	
	// test if pointer exist
	if (signal_input != NULL)
	{
		for (i=0; i<size; i++)
		{
			signal_input[i] = 0;
		}
	}
}


/**
 *******************************************************************************
 * filter_output
 *
 *      Computes filter output
 *			Multiplication of all cases Signal-Filter 
 * 			
 * @param Signal Input table pointer (FILTER_SIZE cases)
 * @param Filter table pointer (FILTER_SIZE cases)
 * @return Filter output 8.24 format (signed int 32 bits)
 ******************************************************************************/

int32_t filter_output(int32_t *signal_input, const int32_t *filter_x_input)
{
	// signed int (64 bits)
	int64_t result = 0;
	int32_t filter_output = 0;
	
	int i = 0;
	
	// test if pointers exists
	if ((signal_input != NULL) && (filter_x_input != NULL))
	{
		// compute filter output
		for (i=0;i<FILTER_SIZE;i++) 
		{	
			result = result + (int64_t) filter_x_input[i]*signal_input[i];
		}
	}
	else
	{
		// default result
		result = 0;
	}
	
	// Conversion from 64 (signed int) to 8.24 format
	filter_output = (int32_t) (result >> 15);
	
	return filter_output;
}
