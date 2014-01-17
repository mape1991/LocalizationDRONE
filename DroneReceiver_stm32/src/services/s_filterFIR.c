/**
 * @file s_filterFIR.c
 * @brief Filter FIR file core
 *
 *      This file contains the filter API for signal processing
 *			Initialization, configuration, computing filter output
 *			real time algorithm to detect the rising edge 
 *
 * 			Last modification : 16 Jan 2014 
 *
 * @author Martin
 * @version 0.0.1
 * @date 12 Dec 2013
 */


/******************************************************************************
* 
*   INCLUDED FILES
*
******************************************************************************/

#include "stm32f10x.h"
#include "ADC_DMA.h"
#include "TIMER_1234.h"
#include "GPIO.h"

// include all filters
#include "s_filterFIR.h"

//TODO:
// Init tables to zero
// init global variables
// Init every buffer, indicator, count, table


/******************************************************************************
*
*   GLOBAL VARIABLES
*
******************************************************************************/

/** TOA values for each beacon */
int toa_0 = 0;
int toa_1 = 0;
int toa_2 = 0;
int toa_3 = 0;

/** Filter input table of 256 values */
int32_t signal_input[SIGNAL_INPUT_SIZE];

/** Block table of 128 values (2 * block table size) to prevent overwriting of block size */
int32_t buffer_block[2*SAMPLE_BLOCK_SIZE];

/** Number of samples */
int adc_samples_count = 0;

/** Number of outputs */
int nb_outputs_count = 0;

/** Real time adc conversion */
float time_conv = 0;

/** Buffer counter (start from end to beginning) */
int buffer_count = 0;

/** Buffer state block */
State_Block_Buffer state_buffer = BUFFER_PART_1;

/** Filter Output Tables of 200 values */
int32_t output_0[OUTPUT_SIZE];
int32_t output_1[OUTPUT_SIZE];
int32_t output_2[OUTPUT_SIZE];
int32_t output_3[OUTPUT_SIZE];


/******************************************************************************
*
*   FUNCTIONS
*
******************************************************************************/

/**
 *******************************************************************************
 * s_filterFIR_IT_ADC
 *
 *      IT Function called when ADC End of Conversion interruption takes place
 *			Read ADC value
 *			Store value in buffer_block
 *			Update nb_samples_total
 *			Update buffer_count
 *			Check buffer state and if changes call calculations for Filter
 * 			
 * @return void
 ******************************************************************************/

// TODO : 
// Convert READ ADC from 1.15 to 8.24
// ADD check conditions for MAX nb_samples_count
// Check if call s_filterFIR_computeOutputs blocks the next interruption .... See option if in while with global var

void s_filterFIR_IT_ADC (void)
{
	// Acquisition and storage of ADC sample and conversion of said sample to s32
  buffer_block[buffer_count] = (s16) ((Read_ADC(ADC1)-2048)<<4);

	// Update nb samples
  adc_samples_count++;
  
	// Update Buffer counter
	buffer_count++;
	
	switch (state_buffer)
	{
		case BUFFER_PART_1 :
			if (buffer_count > SAMPLE_BLOCK_SIZE-1)
			{
				// change state and call filter_output calcul
				state_buffer = BUFFER_PART_2;
				s_filterFIR_computeOutputs();
			}
			break;
			
		case BUFFER_PART_2 :
			if (buffer_count > 2*SAMPLE_BLOCK_SIZE-1)
			{
				// reset buffer count (start from 0 again)
				buffer_count = 0;
				// change state and call filter_output calcul
				state_buffer = BUFFER_PART_1;
				s_filterFIR_computeOutputs();
			}
		default:
			break;
	}			
}


/**
 *******************************************************************************
 * s_filterFIR_computeOutputs
 *
 *      Function called when buffer block is ready and output needs to be computed
 *			First rearrange the table (Delete the oldest block)
 *			Add new block
 * 			Compute outputs (check if filters ON or OFF) to optimize
 *			Store data to output tables
 *			
 *			ATTENTION : This table is inversed (Little Endian) in order to respect the
 *									fir Filter multiplications : e(0)*h(255) = output(255)
 * 			
 * @param void
 * @return 0 if no error
 * @return 1 if no error
 ******************************************************************************/
// TODO
// Test limits (intervalles)

void s_filterFIR_computeOutputs(void)
{
	int32_t out_filter_0_8_24 = 0;
	int32_t out_filter_1_8_24 = 0;
	int32_t out_filter_2_8_24 = 0;
	int32_t out_filter_3_8_24 = 0;

	int i = 0;
	
	// Table rearrangement
	for (i = SAMPLE_BLOCK_SIZE; i<SIGNAL_INPUT_SIZE; i++)
	{
		signal_input[i-SAMPLE_BLOCK_SIZE] = signal_input[i];
	}
	
	// Add new block in the right order
	switch (state_buffer)
	{
		// initial condition : Called when buffer_part_x = 2
		case BUFFER_PART_1 :
			// Buffer_Part_2 is finished
			for (i=SIGNAL_INPUT_SIZE - SAMPLE_BLOCK_SIZE ; i<SIGNAL_INPUT_SIZE; i++)
			{
				signal_input[i] = buffer_block[i - 2*SAMPLE_BLOCK_SIZE];
			}
			break;
			
		case BUFFER_PART_2 :
			// Buffer_Part_1 is finished
			for (i=SIGNAL_INPUT_SIZE - SAMPLE_BLOCK_SIZE ; i<SIGNAL_INPUT_SIZE; i++)
			{
				signal_input[i] = buffer_block[i - 3*SAMPLE_BLOCK_SIZE];
			}
			break;
			
		default:
			break;
	}			
	
	// Compute outputs
	if (fir_0_ON == FILTER_ON)
	{
		out_filter_0_8_24 = filter_output(signal_input, filter_0);
	}
	if (fir_1_ON == FILTER_ON)
	{
		out_filter_1_8_24 = filter_output(signal_input, filter_1);
	}
	if (fir_2_ON == FILTER_ON)
	{
		out_filter_2_8_24 = filter_output(signal_input, filter_2);
	}
	if (fir_3_ON == FILTER_ON)
	{
		out_filter_3_8_24 = filter_output(signal_input, filter_3);
	}
	
	// Store outputs in tables
	output_0[nb_outputs_count] = out_filter_0_8_24;
	output_1[nb_outputs_count] = out_filter_1_8_24;
	output_2[nb_outputs_count] = out_filter_2_8_24;
	output_3[nb_outputs_count] = out_filter_3_8_24;
	
	// Update output count
	nb_outputs_count++;

  // Application d'une atténuation de 0.2 à sortie (par exemple)
	//    out_filter_0_16_48 = ((s64) out_filter_0_8_24) * ((s64)(K_8_24 * 0.2));
	//    out_filter_0_8_24 = (s32) (out_filter_0_16_48>>24);

	if (nb_outputs_count >= OUTPUT_SIZE)
	{
		fir_0_ON = FILTER_OFF;
		fir_1_ON = FILTER_OFF;
		fir_2_ON = FILTER_OFF;
		fir_3_ON = FILTER_OFF;
		
		// call function to compute TOA algorithm and finish the reception
		s_filterFIR_it_function();
	}
}


/**
 *******************************************************************************
 * s_filterFIR_it_function
 *
 *      IT Function called when Timeout interruption takes place 
 * 			
 * @return void
 ******************************************************************************/
// TODO 
void s_filterFIR_it_function(void)
{
	// check Timer
	//time = CNT(TIM3);
	//Stop_DMA1;
	// call Function to compute all filter multiplications
	//s_filterFIR_computeIter();
}


/**
 *******************************************************************************
 * s_filterFIR_initialization
 *
 *      Initialization ADC, Timer, IT_EOC_ADC function
 *			Initialize all filters (init with zeros).
 *			Initialize all input's tables with zeros
 *
 * @param void
 * @return 0 if no error
 * @return 1 if error takes place in the initialization
 ******************************************************************************/

char s_filterFIR_initialization()
{
	char code_Erreur = 0;
	
	// Buffer initialization for filter s output
	initTab_E(output_0, OUTPUT_SIZE);
	initTab_E(output_1, OUTPUT_SIZE);
	initTab_E(output_2, OUTPUT_SIZE);
	initTab_E(output_3, OUTPUT_SIZE);
	
	// Buffer initialization for input tables
	initTab_E(buffer_block, 2*SAMPLE_BLOCK_SIZE);
	initTab_E(signal_input, SIGNAL_INPUT_SIZE);
	
	// I/O configuration for ADC input
	GPIO_Configure(GPIOC, 0, INPUT, ANALOG);	// ADC_In10 (channel 10) 
	
	// Initialize ADC with Fe = 128 KHz
	time_conv = Init_TimingADC_ActiveADC( ADC1, 0);
	Single_Channel_ADC(ADC1, CHANNEL_ADC);
	Init_IT_ADC_EOC(ADC1, ADC_HANDLER_PRIORITY, s_filterFIR_IT_ADC);
	Init_Conversion_On_Trig_Timer(ADC1 , TIM1_CC1, SAMPLE_FREQUENCY);
	
	
	return code_Erreur;
}

