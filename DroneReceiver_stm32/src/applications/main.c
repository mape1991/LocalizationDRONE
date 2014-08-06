/**
 * @file main.c
 * @brief Main function core
 *
 *      This file contains the main function that initializes, configurates
 * 			and start all applications with their services. 
 *
 *			Last modification : 30 Nov 2013 
 *
 * @author Martin
 * @version 0.1.1
 * @date 08 Nov 2013
 */


/******************************************************************************
* 
*   INCLUDED FILES
*
******************************************************************************/

#include "stm32f10x.h"
#include "Clock.h"
#include "Timer_Systick.h"
#include "stdio.h"
#include "s_serialComm.h"
#include "s_filterFIR.h"
#include "global.h"

#include "GPIO.h"
#include "ADC_DMA.h"

//#include "FIR_Filter.h"
//#include "fir_filters.h"


/******************************************************************************
*
*   GLOBAL VARIABLES
*
******************************************************************************/

/** application state */
State_APP state = APP_OFF;
State_APP prev_state = APP_OFF;

char toto = 0;

/******************************************************************************
*
*   PRIVATE FUNCTIONS
*
******************************************************************************/

char app_updateDrone(void);
void app_serialCommHandler(char c);
void app_responseDrone(void)		;
void app_send_droneData(int toa1, int toa2, int toa3, int toa4);
void app_initialization(void);
void app_send_droneData_filter_output(int64_t value);


/**
 *******************************************************************************
 * app_updateDrone
 *
 *      Update Drone with special character
 *			Send special character that depends on the app state
 * 			
 * @param void
 * @return 0 if no error
 * @return 1 if error takes place in send char			
 * @sa main()
 ******************************************************************************/
 
char app_updateDrone(void)		
{
	char code_erreur = 0;
	
	switch (state)
	{
		case APP_OFF:
			code_erreur = s_serialComm_sendChar(MSG_OFF);
			break;
		case APP_RECEIVE:
			code_erreur = s_serialComm_sendChar(MSG_BUSY);
			break;
		default:
			code_erreur = 1; 		
	}
	
	return code_erreur;
}


/*
 *******************************************************************************
 * app_serialCommHandler
 *
 *      Called when serial comm interruption takes place
 *			Analyze received character and update app state
 * 			If character is not valid (protocol not respected)
 *			then the application state rest the same as before
 *
 * @sa main()
 ******************************************************************************/
 
void app_serialCommHandler(char c)		
{
	char error = 0;
	
	if (c == MSG_OFF)
	{
		if (state != APP_OFF)
		{
			prev_state = state;
			state = APP_OFF;
			// KILL STOP SIGNAL
			s_filterFIR_it_function(STOP_X);
		}
		error = app_updateDrone();
	}
	else if (c == MSG_SEND)
	{
		if (state == APP_OFF)
		{
			prev_state = state;
			state = APP_RECEIVE;
			toa_0 = 0;
			toa_1 = 0;
			toa_2 = 0;
			toa_3 = 0;
			data_ready = 0;
			s_filterFIR_startReception();
		}
		else
		{
			error = app_updateDrone();
		}
	}
	else
	{
		error = app_updateDrone();
	}
	
	if (error != 0)
	{
		// update Status LED
		s_serialComm_setErrorLED(LED_ON);
	}
}


/**
 *******************************************************************************
 * app_responseDrone
 *
 *      Called when results are ready to be send to the Drone
 *			Send Data
 *			Update App State 
 *			ATTENTION : Use Systick for timeout robustness (interruption system)
 * 			
 * @sa main()
 ******************************************************************************/
 
void app_responseDrone(void)		
{
	prev_state = state;
	int i=0;
	char code_Error = 0;
	
	// Send data to drone
	app_send_droneData(toa_0, toa_1, toa_2, toa_3);
	app_send_droneData(toa_0_value, toa_1_value, toa_2_value, toa_3_value);
	
	s_serialComm_sendChar('S');
	/**
	for (i=0; i<OUTPUT_SIZE; i++)
	{
		app_send_droneData_filter_output(module_FIR0[i]);
		//app_send_droneData_filter_output(100);
		//app_send_droneData_filter_output(adc_acq0[i]);
	}
	for (i=0; i<OUTPUT_SIZE; i++)
	{
		app_send_droneData_filter_output(module_FIR1[i]);
		//app_send_droneData_filter_output(0);
		//app_send_droneData_filter_output(adc_acq1[i]);
	}
	for (i=0; i<OUTPUT_SIZE; i++)
	{
		app_send_droneData_filter_output(module_FIR2[i]);
		//app_send_droneData_filter_output(267);
		//app_send_droneData_filter_output(adc_acq2[i]);
	}
	for (i=0; i<OUTPUT_SIZE; i++)
	{
		app_send_droneData_filter_output(module_FIR3[i]);
		//app_send_droneData_filter_output(333);
		//app_send_droneData_filter_output(adc_acq3[i]);
	}
	*/
	
	char string[2];
	string[0] = MSG_NULL;
	string[1] = MSG_NULL;
	
	// send ADC pure value
	for (i=0; i<4000; i++)
	{

		string[0] = (char) (adc_acq[i]);
		string[1] = (char) (adc_acq[i] >> 8);
			
		// Send string to drone
		code_Error = s_serialComm_sendString(string, 2);
	}
	
	data_ready = 0;
	
	// Update App state
	state = APP_OFF;
}


/**
 *******************************************************************************
 * app_send_droneData
 *
 *      Function called when STM needs to send data to drone
 *			Test input data parameters and convert into string
 *			Call function in the service comm in order to send the data
 * 			
 ******************************************************************************/

void app_send_droneData(int toa1, int toa2, int toa3, int toa4)
{
	char string[SIZE_FRAME];
	int i = 0;
	State_Input_Comm etat_input = TOA1;
	char code_Error = 0;
	
	// Init string
	string[0] = MSG_SEND;
	for(i=1;i<SIZE_FRAME; i++)
	{
		string[i] = MSG_NULL;
	}
	
	// Convert input data in string
	for (i=1;i<SIZE_FRAME;i++)
	{
		switch (etat_input)
		{
			case TOA1 :
				string[i] = (char) (toa1 >> (8*(i-1)));
			  break;
			case TOA2 :
				string[i] = (char) (toa2 >> (8*(i-5)));
			  break;
			case TOA3 :
				string[i] = (char) (toa3 >> (8*(i-9)));
			  break;
			case TOA4 :
				string[i] = (char) (toa4 >> (8*(i-13)));
			  break;
			
			default:
				s_serialComm_setErrorLED(LED_ON);
		}
		
		if (!(i % 4))
		{
			etat_input++;		
		}
	}
	
	// Send string to drone
	code_Error = s_serialComm_sendString(string, FRAME_SIZE);
	
	// update Status LED if error
	if (code_Error != 0)
	{
		s_serialComm_setErrorLED(LED_ON);
	}
}


/** test output filter values */
void app_send_droneData_filter_output(int64_t value)
{
	char string[8];
	int i = 0;
	int j=0;
	
	char code_Error = 0;
	
	// Init string
	for(i=0;i<8; i++)
	{
		string[i] = MSG_NULL;
	}
	
	// Convert input data in string
	for (j=0;j<8;j++)
	{
			string[j] = (char) (value >> j*8);
	}
	
	// Send string to drone
	code_Error = s_serialComm_sendString(string, 8);
	
	// update Status LED if error
	if (code_Error != 0)
	{
		GPIO_Set(GPIOB,5);
	}
}


/**
 *******************************************************************************
 * app_initialization
 *
 *      Configures all clocks and registers.
 *			Initializes all Services
 * 			Configures Systick Timer
 *
 * @sa main()
 ******************************************************************************/

void app_initialization(void)
{	
	//vu16 duree;
	char code_Erreur = 0;
	
	// Clock's configuration
	CLOCK_Configure();		// System clock initialization
												// If STM32F107 defined as target, CPU clock = 50MHz
												// D�faut (STM32F103), CPU clock = 40MHz
	
	// service initialization
	code_Erreur = s_serialComm_initialization(app_serialCommHandler);
	if (code_Erreur != 0)
	{
		s_serialComm_setErrorLED(LED_ON);
	}
	
	code_Erreur = s_filterFIR_initialization();
	if (code_Erreur != 0)
	{
		s_serialComm_setErrorLED(LED_ON);
	}

}


/******************************************************************************
*
*   MAIN
*
******************************************************************************/

/*
void test_extern_include_tables(void)
{
	initTab_E (signal_input, FILTER_SIZE);

	int32_t test_output1 = filter_output(signal_input, filter_0);
	int32_t test_output2 = filter_output(signal_input, filter_1);
	int32_t test_output3 = filter_output(signal_input, filter_2);
	int32_t test_output4 = filter_output(signal_input, filter_3);
	
	if (!test_output1 && !test_output2 && !test_output3 && test_output4)
	{
		s_serialComm_setErrorLED(LED_ON);
	}
	else
	{
		s_serialComm_setErrorLED(LED_OFF);
	}
}
*/

/*
void test_ADC(void)
{
	int32_t test_ADC_read_value = (int32_t) ((Read_ADC(ADC1)-2048)<<4);
  
	int32_t test_1 = 0;
	int32_t test_2 = -1;
	int32_t test_3 = -127;
	int32_t test_4 = 127;
	int test_5 = -1;
	int32_t test_6 = 255;
	
	test_ADC_read_value = - test_ADC_read_value;
	
	if (test_ADC_read_value > ADC_THRESHOLD_POS || test_ADC_read_value < ADC_THRESHOLD_NEG)
	{	
		GPIO_Set(GPIOB,9);	// set error LED ON
	}
	
	if (test_1 != test_2)
	{	
		GPIO_Set(GPIOB,9);	// set error LED OFF
	}
	
	if (test_3 != test_4)
	{	
		GPIO_Clear(GPIOB,9);	// set error LED OFF
	}
	
	if (test_5 != test_6)
	{	
		GPIO_Clear(GPIOB,9);	// set error LED OFF
	}
}
*/

int main (void)
{	
	// Initialization
	app_initialization();
		
	// Infinity loop 
	while(1)
	{
		// Compute filter outputs 
		if(nb_outputs_count < nb_it_compute)
		{
			toto++;	
			s_filterFIR_computeOutputs();
			toto = 0;
			
		}
		// check if drone data is read to be send
		if (data_ready == 1) 
		{
			app_responseDrone();
		}
		//test_ADC();
		// test data tables
		//app_send_droneData_filter_output(100);
	}
	

	
	return 0;
}