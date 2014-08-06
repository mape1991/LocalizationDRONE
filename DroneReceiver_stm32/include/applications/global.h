/**
 * @file global.h
 * @brief Global file header
 *
 *      This file contains global variables and constants used in 
 *			the application.  
 *
 *			Last modification : 12 Dec 2013 
 *
 * @author Martin
 * @version 0.2.0
 * @date 14 Nov 2013
 */


#ifndef GLOBAL_H
#define GLOBAL_H


/******************************************************************************
* 
*   INCLUDED FILES
*
******************************************************************************/

#include "stm32f10x.h"
#include "stdint.h"


/******************************************************************************
*
*   CONSTANTS
*
******************************************************************************/ 

/** Systick period in [us]*/
#define SYSTICK_PERIOD 100000.0

/** Calibration ADC (offset = 1.55) we need offset = 1.65 */
#define ADC_OFFSET 125

/** ADC read values THRESHOLDS */
//#define ADC_THRESHOLD_POS 8188
//#define ADC_THRESHOLD_NEG -8192
#define ADC_THRESHOLD_POS 6000
#define ADC_THRESHOLD_NEG -6000

/** Frame size for USART Protocol bet*/
#define SIZE_FRAME 17

/** ADC Sample time [us] */
#define SAMPLE_TIME_ADC 7.8125

/** Sampling Frequency [KHz] */
#define SAMPLE_FREQUENCY 128.0

/** Number of samples saved in filter input tables */
#define SIGNAL_INPUT_SIZE 256

/** Number of samples savec in the output tables */
#define OUTPUT_SIZE NB_SAMPLES_TOTAL/SAMPLE_BLOCK_SIZE 

/** Number of samples for 100 ms with Fe=128KHz */
#define NB_SAMPLES_TOTAL 12800

/** Number of samples for one block that updates the filter Input Table */
#define SAMPLE_BLOCK_SIZE 128

/** Number of samples for buffer input table */
#define BUFFER_SIZE 2*SAMPLE_BLOCK_SIZE

/** Constants (fixed point)*/
#define K_8_24 16777216 // constante 2^24
#define K_1_15 32768 // constante 2^15

/** Filter Table size */
#define FILTER_SIZE 256

/** Size protocol USART STM - Drone */
#define FRAME_SIZE 17

/** Protocol Messages between the Client and the Server */
#define MSG_OFF 'X'			// Exit
#define MSG_SEND 'S'		// Send
#define MSG_BUSY 'B'		// Busy
#define MSG_NULL 0		// NULL

/******************************************************************************
*
*   ENUM TYPES
*
******************************************************************************/ 

/** Type def for Application State */
typedef enum
{
	APP_OFF,
	APP_RECEIVE
	
} State_APP;

/** Type def for Input conversion (int to string) : used in the application */
typedef enum
{
	TOA1,
	TOA2,
	TOA3,
	TOA4

} State_Input_Comm;

/** Type def for Buffer block state (first or second block) : used in s_filter_FIR */
typedef enum
{
	BUFFER_PART_1,
	BUFFER_PART_2
	
} State_Block_Buffer;

/** Type def for Signal input block state (first, second, third or forth block) : used in s_filter_FIR */
typedef enum
{
	TAB_E_PART_1,
	TAB_E_PART_2,
	TAB_E_PART_3,
	TAB_E_PART_4
	
} State_Input_Signal;

/** Type def for STOP Signal : used in s_filter_FIR */
typedef enum
{
	STOP_X,
	STOP_NORMAL,
	STOP_NOT
} Stop_Signal;

/******************************************************************************
* 
*   GLOBAL VARIABLES
*
******************************************************************************/

/** Attention int32_t = s32 (but it is not working with extern) */
/** Filter tables : initialized in each FIR_X.c file */
extern const int32_t filter_0[FILTER_SIZE];
extern const int32_t filter_1[FILTER_SIZE];
extern const int32_t filter_2[FILTER_SIZE];
extern const int32_t filter_3[FILTER_SIZE];
extern const int32_t filter_0_cos[FILTER_SIZE];
extern const int32_t filter_1_cos[FILTER_SIZE];
extern const int32_t filter_2_cos[FILTER_SIZE];
extern const int32_t filter_3_cos[FILTER_SIZE];


/** TOA values : initialized in s_filterFIR.c file */
extern int toa_0;
extern int toa_1;
extern int toa_2;
extern int toa_3;

/** TOA values in Filter: initialized in s_filterFIR.c file */
extern int32_t toa_0_value;
extern int32_t toa_1_value;
extern int32_t toa_2_value;
extern int32_t toa_3_value;

/** Filter input table : initialized in s_filterFIR.c file */
extern int32_t signal_input[SIGNAL_INPUT_SIZE];
extern int32_t buffer_block[2*SAMPLE_BLOCK_SIZE];

/** Number of samples : initialized in s_filterFIR.c file */
extern int adc_samples_count;

/** Number of outputs : initialized in s_filterFIR.c file */
extern int nb_outputs_count;

/** Buffer counter : initialized in s_filterFIR.c file */
extern int buffer_count;

/** Buffer state : initialized in s_filterFIR.c file */
extern State_Block_Buffer state_buffer;

/** Filter Output Tables : initialized in s_filter_FIR.c file */
extern int32_t output_0[OUTPUT_SIZE];
extern int32_t output_1[OUTPUT_SIZE];
extern int32_t output_2[OUTPUT_SIZE];
extern int32_t output_3[OUTPUT_SIZE];
extern int32_t output_0_cos[OUTPUT_SIZE];
extern int32_t output_1_cos[OUTPUT_SIZE];
extern int32_t output_2_cos[OUTPUT_SIZE];
extern int32_t output_3_cos[OUTPUT_SIZE];

extern int32_t module_FIR0[OUTPUT_SIZE];
extern int32_t module_FIR1[OUTPUT_SIZE];
extern int32_t module_FIR2[OUTPUT_SIZE];
extern int32_t module_FIR3[OUTPUT_SIZE];

extern int32_t adc_acq0[OUTPUT_SIZE];
extern int32_t adc_acq1[OUTPUT_SIZE];
extern int32_t adc_acq2[OUTPUT_SIZE];
extern int32_t adc_acq3[OUTPUT_SIZE];

extern u16 adc_acq[2000];

/** Number of compute interruptions */
extern int nb_it_compute;

/** Send toa data to drone ready */
extern char data_ready;

/** Led ERROR if ADC over THRESHOLD */
extern char adc_threshold_error;

extern char toto;

#endif					/* GLOBAL_H */



