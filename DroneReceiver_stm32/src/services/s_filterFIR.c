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

int32_t toa_0_value = 0;
int32_t toa_1_value = 0;
int32_t toa_2_value = 0;
int32_t toa_3_value = 0;

/** Filter input table of 256 values */
int32_t signal_input[SIGNAL_INPUT_SIZE];

/** Block table of 128 values (2 * block table size) to prevent overwriting of block size */
int32_t buffer_block[BUFFER_SIZE];

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
int32_t output_0_cos[OUTPUT_SIZE];
int32_t output_1_cos[OUTPUT_SIZE];
int32_t output_2_cos[OUTPUT_SIZE];
int32_t output_3_cos[OUTPUT_SIZE];
int32_t module_FIR0[OUTPUT_SIZE];
int32_t module_FIR1[OUTPUT_SIZE];
int32_t module_FIR2[OUTPUT_SIZE];
int32_t module_FIR3[OUTPUT_SIZE];

/** Number of it for output computation */
int nb_it_compute = 0;

/** TOA data is ready to be send to the drone */
char data_ready = 0;

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

void s_filterFIR_IT_ADC (void)
{	
	// Acquisition and storage of ADC sample and conversion of said sample to s32
  buffer_block[buffer_count] = (int32_t) ((Read_ADC(ADC1)-2048)<<4);
  
	if ((buffer_block[buffer_count] > ADC_THRESHOLD_POS || buffer_block[buffer_count] < ADC_THRESHOLD_NEG) && (buffer_block[buffer_count] != 0))
	{	
		GPIO_Set(GPIOB,9);	// set error LED ON
	}
	
	if (adc_samples_count < NB_SAMPLES_TOTAL)
	{
		// Update Buffer counter
		buffer_count++;
		
		if (buffer_count == SAMPLE_BLOCK_SIZE)
		{
			// change state and call filter_output calcul
			state_buffer = BUFFER_PART_2;
			nb_it_compute ++;
			if (toto > 0) GPIO_Set(GPIOB,6);	// set error LED ON
		}

		else if (buffer_count == BUFFER_SIZE)
		{
			// reset buffer count (start from 0 again)
			buffer_count = 0;
			// change state and call filter_output calcul
			state_buffer = BUFFER_PART_1;
			nb_it_compute ++;
			if (toto > 0)
			{
				GPIO_Set(GPIOB,6);	// set error LED ON
			}
		}
	}
	else
	{
		GPIO_Set(GPIOB,7);	// set error LED ON
		// Stop Timer tri conversion and Reset Properties
		Bloque_Timer(TIM1);
		Reset_Timer(TIM1);
	}
	
	// Update nb samples
  adc_samples_count++;
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

void s_filterFIR_computeOutputs(void)
{
	int i = 0;
	int j= 0;
	
	// temporary results of filter computation (signed int 64 bits)
  int64_t result_0 = 0, result_1 = 0, result_2 = 0, result_3 = 0;  
	int64_t result_0_cos = 0, result_1_cos = 0, result_2_cos = 0, result_3_cos = 0; 
	
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
		  j= SAMPLE_BLOCK_SIZE;
			break;
			
		case BUFFER_PART_2 :
			// Buffer_Part_1 is finished
		  j=0;
			break;
			
		default:
			break;
	}			
	
	for (i=SIGNAL_INPUT_SIZE - SAMPLE_BLOCK_SIZE; i<SIGNAL_INPUT_SIZE; i++,j++)
	{
			signal_input[i] = buffer_block[j];
	}
			
	// Compute outputs and Store outputs in tables
	for (i=0;i<FILTER_SIZE;i++) 
	{	
		result_0 = result_0 + (int64_t) filter_0[i]*signal_input[i];
		result_1 = result_1 + (int64_t) filter_1[i]*signal_input[i];
		result_2 = result_2 + (int64_t) filter_2[i]*signal_input[i];
		result_3 = result_3 + (int64_t) filter_3[i]*signal_input[i];
	}
	
	// Compute outputs and Store outputs in tables
	for (i=0;i<FILTER_SIZE;i++) 
	{	
		result_0_cos = result_0_cos + ((int64_t) filter_0_cos[i])*((int64_t)signal_input[i]);
		result_1_cos = result_1_cos + ((int64_t) filter_1_cos[i])*((int64_t)signal_input[i]);
		result_2_cos = result_2_cos + ((int64_t) filter_2_cos[i])*((int64_t)signal_input[i]);
		result_3_cos = result_3_cos + ((int64_t) filter_3_cos[i])*((int64_t)signal_input[i]);
	}

	// Conversion from 64 (signed int) to 8.24 format  and storage of result
  output_0[nb_outputs_count] = (int32_t) (result_0 >> 15);
  output_1[nb_outputs_count] = (int32_t) (result_1 >> 15);
  output_2[nb_outputs_count] = (int32_t) (result_2 >> 15);
  output_3[nb_outputs_count] = (int32_t) (result_3 >> 15);
	
	output_0_cos[nb_outputs_count] = (int32_t) (result_0_cos >> 15);
	output_1_cos[nb_outputs_count] = (int32_t) (result_1_cos >> 15);
	output_2_cos[nb_outputs_count] = (int32_t) (result_2_cos >> 15);
	output_3_cos[nb_outputs_count] = (int32_t) (result_3_cos >> 15);
	
	// Update output count
	nb_outputs_count++;

  // Application d'une atténuation de 0.2 à sortie (par exemple)
	//    out_filter_0_16_48 = ((s64) out_filter_0_8_24) * ((s64)(K_8_24 * 0.2));
	//    out_filter_0_8_24 = (s32) (out_filter_0_16_48>>24);

	if (nb_outputs_count > OUTPUT_SIZE - 1 ) //OUTPUT_SIZE)
	{
		// call function to compute TOA algorithm and finish the reception
		s_filterFIR_it_function(STOP_NORMAL);
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

void s_filterFIR_it_function(Stop_Signal signal)
{
	
	switch (signal)
	{
		case STOP_NOT :
			break;
		
		case STOP_NORMAL :
			// Compute TOA values /TODO
			if (nb_outputs_count >= OUTPUT_SIZE)
			{
				function_TOA();
			}
			else
			{
				toa_0 = 0; // 17 
				toa_1 = 0;
				toa_2 = 0;
				toa_3 = 0;
				
				toa_0_value = 0;
				toa_1_value = 0;
				toa_2_value = 0;
				toa_3_value = 0;
			}
			// Data is ready to be send 
			data_ready = 1;
			
		case STOP_X:
			/** Stop timer */
			Bloque_Timer(TIM1);
			Reset_Timer(TIM1);

			/** Reset tables and variables to zero */
			// Buffer initialization for filter s output
			initTab_E(output_0, OUTPUT_SIZE);
			initTab_E(output_1, OUTPUT_SIZE);
			initTab_E(output_2, OUTPUT_SIZE);
			initTab_E(output_3, OUTPUT_SIZE);
			
			initTab_E(output_0_cos, OUTPUT_SIZE);
			initTab_E(output_1_cos, OUTPUT_SIZE);
			initTab_E(output_2_cos, OUTPUT_SIZE);
			initTab_E(output_3_cos, OUTPUT_SIZE);

			initTab_E(module_FIR0, OUTPUT_SIZE);
			initTab_E(module_FIR1, OUTPUT_SIZE);
			initTab_E(module_FIR2, OUTPUT_SIZE);
			initTab_E(module_FIR3, OUTPUT_SIZE);
			
			// Buffer initialization for input tables
			initTab_E(buffer_block, BUFFER_SIZE);
			initTab_E(signal_input, SIGNAL_INPUT_SIZE);
	
			//TODO Reset variables 	
			/** TOA values for each beacon */
			adc_samples_count = 0;
			nb_outputs_count = 0;
			buffer_count = 0;
			state_buffer = BUFFER_PART_1;
			nb_it_compute = 0;
		
			break;		
		
		default:
			break;
	}
	
	GPIO_Clear(GPIOB,7);	// set error LED OFF
}

/**
 *******************************************************************************
 * s_filterFIR_startReception
 *
 *      Init timer for ADC conversion manually
 *			Start Timer
 *
 * @return void
 ******************************************************************************/

void s_filterFIR_startReception(void)
{
	// Init Timer conversion on the ADC with the Trig Timer
	Init_Conversion_On_Trig_Timer(ADC1 , TIM1_CC1, SAMPLE_FREQUENCY);
	
	// Start Timer
	Run_Timer(TIM1);
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
// TODO Change Init Trig Timer
// Add timeout for robust code

char s_filterFIR_initialization()
{
	char code_Erreur = 0;
	
	// Buffer initialization for filter s output
	initTab_E(output_0, OUTPUT_SIZE);
	initTab_E(output_1, OUTPUT_SIZE);
	initTab_E(output_2, OUTPUT_SIZE);
	initTab_E(output_3, OUTPUT_SIZE);
	initTab_E(output_0_cos, OUTPUT_SIZE);
	initTab_E(output_1_cos, OUTPUT_SIZE);
	initTab_E(output_2_cos, OUTPUT_SIZE);
	initTab_E(output_3_cos, OUTPUT_SIZE);
	initTab_E(module_FIR0, OUTPUT_SIZE);
	initTab_E(module_FIR1, OUTPUT_SIZE);
	initTab_E(module_FIR2, OUTPUT_SIZE);
	initTab_E(module_FIR3, OUTPUT_SIZE);
	
	// Buffer initialization for input tables
	initTab_E(buffer_block, BUFFER_SIZE);
	initTab_E(signal_input, SIGNAL_INPUT_SIZE);
	
	// I/O configuration for ADC input
	GPIO_Configure(GPIOC, 0, INPUT, ANALOG);	// ADC_In10 (channel 10) 
	
	// Initialize ADC with Fe = 128 KHz
	time_conv = Init_TimingADC_ActiveADC( ADC1, 0);
	Single_Channel_ADC(ADC1, CHANNEL_ADC);
	Init_IT_ADC_EOC(ADC1, ADC_HANDLER_PRIORITY, s_filterFIR_IT_ADC);
		
	// debug LED
		
	GPIO_Configure(GPIOB, 5, OUTPUT, OUTPUT_PPULL);	// ERROR LED ADC read value THRESHOLD 2 (On/Off)
	GPIO_Clear(GPIOB,5);	// set error LED OFF
	
	GPIO_Configure(GPIOB, 6, OUTPUT, OUTPUT_PPULL); // ERROR LED ADC it not compute correctly (every block from the buffer)	
	GPIO_Clear(GPIOB,6);	// set error LED OFF
	GPIO_Configure(GPIOB, 7, OUTPUT, OUTPUT_PPULL);	// ERROR LED ADC when not enough samples
	GPIO_Clear(GPIOB,7);	// set error LED OFF
	
	GPIO_Configure(GPIOB, 9, OUTPUT, OUTPUT_PPULL);	// ERROR LED ADC read value THRESHOLD(On/Off)
	GPIO_Clear(GPIOB,9);	// set error LED OFF
	
	
	return code_Erreur;
}


void function_TOA(void)
{
	int i;
	int64_t mean_FIR0 = 0, mean_FIR1 = 0, mean_FIR2 = 0, mean_FIR3 = 0;
	char toa_0_find = 0;
	char toa_1_find = 0;
	char toa_2_find = 0;
	char toa_3_find = 0;
	
	//Calcul du module des filtre FIR
	for(i=0;i<OUTPUT_SIZE;i++)
	{		
    module_FIR0[i] = ((((int64_t)(output_0[i]))*((int64_t)(output_0[i]))+((int64_t)(output_0_cos[i]))*((int64_t)(output_0_cos[i]))) >> 32);
    module_FIR1[i] = ((((int64_t)(output_1[i]))*((int64_t)(output_1[i]))+((int64_t)(output_1_cos[i]))*((int64_t)(output_1_cos[i]))) >> 32);
		module_FIR2[i] = ((((int64_t)(output_2[i]))*((int64_t)(output_2[i]))+((int64_t)(output_2_cos[i]))*((int64_t)(output_2_cos[i]))) >> 32);
		module_FIR3[i] = ((((int64_t)(output_3[i]))*((int64_t)(output_3[i]))+((int64_t)(output_3_cos[i]))*((int64_t)(output_3_cos[i]))) >> 32);
		
		mean_FIR0 += module_FIR0[i]; 
		mean_FIR1 += module_FIR1[i];
		mean_FIR2 += module_FIR2[i];
		mean_FIR3 += module_FIR3[i];		
	}
	//Calcul des moyennes des modules
	mean_FIR0 /= OUTPUT_SIZE;	
	mean_FIR1 /= OUTPUT_SIZE;	
	mean_FIR2 /= OUTPUT_SIZE;	
	mean_FIR3 /= OUTPUT_SIZE;
	
	for(i=0;i<OUTPUT_SIZE-2;i++){	
		if(toa_0_find == 0 && module_FIR0[i] > mean_FIR0 && module_FIR0[i+1] > mean_FIR0 && module_FIR0[i+2] > mean_FIR0 )
		{	
			toa_0 = i*SAMPLE_BLOCK_SIZE;
			toa_0_find = 1;
			toa_0_value = output_0[i];
		}
		if(toa_1_find == 0 && module_FIR1[i] > mean_FIR1 && module_FIR1[i+1] > mean_FIR1 && module_FIR1[i+2] > mean_FIR1)
		{
			toa_1 = i*SAMPLE_BLOCK_SIZE;
			toa_1_find = 1;
			toa_1_value = output_1[i];
		}
		if(toa_2_find == 0 && module_FIR2[i] > mean_FIR2 && module_FIR2[i+1] > mean_FIR2 && module_FIR2[i+2] > mean_FIR2)
		{
			toa_2 = i*SAMPLE_BLOCK_SIZE;
			toa_2_find = 1;
			toa_2_value = output_2[i];
		}
		if(toa_3_find == 0 && module_FIR3[i] > mean_FIR3 && module_FIR3[i+1] > mean_FIR3 && module_FIR3[i+2] > mean_FIR3)
		{
			toa_3 = i*SAMPLE_BLOCK_SIZE;
			toa_3_find = 1;
			toa_3_value = output_3[i];
		}
	}
}	