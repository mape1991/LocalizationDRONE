/**
 * @file s_filterFIR.c
 * @brief Filter FIR file core
 *
 *      This file contains the filter API for each iteration
 *			Initialization, configuration and computing the iteration 
 *
 * 			Last modification : 05 Jan 2014 
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
#include "s_filterFIR.h"
#include "ADC_DMA.h"
//#include "ADC.h"
#include "TIMER_1234.h"
#include "GPIO.h"


/******************************************************************************
*
*   GLOBAL VARIABLES
*
******************************************************************************/

/** Filter input table of 256 values */
s32 in_filter_8_24[SIZE_TABLE_SAMPLES];

/** Block table of 128 values (2 * block table size) */
s32 buffer_block[2*SAMPLE_BLOCK_SIZE];	// to prevent overwriting of block size 

/** Real time adc conversion */
float time_conv = 0;

/** Index for buffer block */
u16 buffer_index = 0;

/** */
u16 adc_sample_count = 0;

/******************************************************************************
*
*   PRIVATE FUNCTIONS
*
******************************************************************************/

/**
 *******************************************************************************
 * s_filterFIR_it_function
 *
 *      IT Function called when DMA interruption takes place 
 * 			
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


void s_filterFIR_IT_ADC (void)
{
	// Update nb samples
	adc_sample_count ++;
	
	
	//Start_ADC_Conversion(ADC1);
	
}


/**
 *******************************************************************************
 * s_filterFIR_initialization
 *
 *      Initialization ADC, Timer, DMA
 *			Configuration of all filters (init with zeros).
 * 			
 * @param void
 * @return 0 if no error
 * @return 1 if error takes place in the initialization
 ******************************************************************************/

char s_filterFIR_initialization()
{
	char code_Erreur = 0;
	char Circ = 1;	// 0 or 1 (1 if reconfiguration)
/*	
	// Buffer initialization for filters
  Init_FIR_asm(0); // Add zeros in the tables
  Init_FIR_asm(1); 
	Init_FIR_asm(2);
	Init_FIR_asm(3);
*/	
	// I/O configuration for ADC input
	GPIO_Configure(GPIOC, 0, INPUT, ANALOG);	// ADC_In10 (channel 10) 
	
	// Initialize ADC with Fe = 128 KHz
	time_conv = Init_TimingADC_ActiveADC( ADC1, 0);
	Single_Channel_ADC(ADC1, CHANNEL_ADC);
	Init_IT_ADC_EOC(ADC1, ADC_HANDLER_PRIORITY, s_filterFIR_IT_ADC);
	Init_Conversion_On_Trig_Timer(ADC1 , TIM1_CC1, SAMPLE_FREQUENCY);
	
	
	return code_Erreur;
}


/**
 *******************************************************************************
 * s_filterFIR_computeIter
 *
 *      IT Function called when ADC interruption takes place
 *			Get received value from the ADC register
 *			Compute the  
 * 			
 * @param void
 * @return 0 if no error
 * @return 1 if no error
 ******************************************************************************/
/*
void s_filterFIR_computeIter(void)
//{
	//... dans une des taches du tourniquet
    u16 echantillon = 0;   
		//s16 in_signal = 0;
 
		s32 out_filter_0_8_24 = 0;
		s32 out_filter_1_8_24 = 0;
		s32 out_filter_2_8_24 = 0;
		s32 out_filter_3_8_24 = 0;
/*	
		s64 out_filter_0_16_48;
		s64 out_filter_1_16_48;
		s64 out_filter_2_16_48;
		s64 out_filter_3_16_48;
*	
    //Read value from ADC register
    //echantillon=Read_ADC(ADC1);
		echantillon = 4096;
		// conversion from u16.0 to s1.15
    s16 Entree = (echantillon-2048)<<4; // format s1.15 [sxxx xxxx xxxx 0000] 
 
    // Filter iteration (i)
//    out_filter_0_8_24 = FIR_asm(0,Entree); // out in format [s8.24]
//		out_filter_1_8_24 = FIR_asm(1,Entree); // out in format [s8.24]
//		out_filter_2_8_24 = FIR_asm(2,Entree); // out in format [s8.24]
//		out_filter_3_8_24 = FIR_asm(3,Entree); // out in format [s8.24]
 
 /**
    // Application d'une atténuation de 0.2 à sortie (par exemple)
    out_filter_0_16_48 = ((s64) out_filter_0_8_24) * ((s64)(K_8_24 * 0.2));
    out_filter_0_8_24 = (s32) (out_filter_0_16_48>>24);
 
   
	 // Adaptation au u16 (12 bits significatifs à gauche) pour le DAC 
   // ... à vous de jouer	
		int a;
		int b;
		a = 10;
		b = 20;
		if (b > a)
		{
			a = 20;
		}
}
*/


