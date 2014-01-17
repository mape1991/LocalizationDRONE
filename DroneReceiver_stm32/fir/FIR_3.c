/**
 * @file FIR_3.c
 * @brief Filter FIR 3 source file
 *
 *      This file contains the filter 3 values written in the table
 *
 * 			Last modification : 16 Jan 2014 
 *
 * @author Martin
 * @version 0.0.1
 * @date 12 Jan 2014
 */


/******************************************************************************
* 
*		INCLUDED FILES 
*
******************************************************************************/

#include "stm32f10x.h"
#include "fir_filters.h"


/******************************************************************************
* 
*		DESCRIPTION DU FILTRE FIR_3, 8.24 
*
******************************************************************************/

const int32_t filter_3[FILTER_SIZE]= {
K_8_24*9.091680e-01,
K_8_24*-7.572088e-01,
K_8_24*-2.785197e-01,
K_8_24*9.891765e-01,
K_8_24*-5.453250e-01,
K_8_24*-5.349976e-01,
K_8_24*9.909026e-01,
K_8_24*-2.902847e-01,
K_8_24*-7.491364e-01,
K_8_24*9.142098e-01,
K_8_24*-1.227154e-02,
K_8_24*-9.039893e-01,
K_8_24*7.651673e-01,
K_8_24*2.667128e-01,
K_8_24*-9.873014e-01,
K_8_24*5.555702e-01,
K_8_24*5.245897e-01,
K_8_24*-9.924795e-01,
K_8_24*3.020059e-01,
K_8_24*7.409511e-01,
K_8_24*-9.191139e-01,
K_8_24*2.454123e-02,
K_8_24*8.986745e-01,
K_8_24*-7.730105e-01,
K_8_24*-2.548657e-01,
K_8_24*9.852776e-01,
K_8_24*-5.657318e-01,
K_8_24*-5.141027e-01,
K_8_24*9.939070e-01,
K_8_24*-3.136817e-01,
K_8_24*-7.326543e-01,
K_8_24*9.238795e-01,
K_8_24*-3.680722e-02,
K_8_24*-8.932243e-01,
K_8_24*7.807372e-01,
K_8_24*2.429802e-01,
K_8_24*-9.831055e-01,
K_8_24*5.758082e-01,
K_8_24*5.035384e-01,
K_8_24*-9.951847e-01,
K_8_24*3.253103e-01,
K_8_24*7.242471e-01,
K_8_24*-9.285061e-01,
K_8_24*4.906767e-02,
K_8_24*8.876396e-01,
K_8_24*-7.883464e-01,
K_8_24*-2.310581e-01,
K_8_24*9.807853e-01,
K_8_24*-5.857979e-01,
K_8_24*-4.928982e-01,
K_8_24*9.963126e-01,
K_8_24*-3.368899e-01,
K_8_24*-7.157308e-01,
K_8_24*9.329928e-01,
K_8_24*-6.132074e-02,
K_8_24*-8.819213e-01,
K_8_24*7.958369e-01,
K_8_24*2.191012e-01,
K_8_24*-9.783174e-01,
K_8_24*5.956993e-01,
K_8_24*4.821838e-01,
K_8_24*-9.972905e-01,
K_8_24*3.484187e-01,
K_8_24*7.071068e-01,
K_8_24*-9.373390e-01,
K_8_24*7.356456e-02,
K_8_24*8.760701e-01,
K_8_24*-8.032075e-01,
K_8_24*-2.071114e-01,
K_8_24*9.757021e-01,
K_8_24*-6.055110e-01,
K_8_24*-4.713967e-01,
K_8_24*9.981181e-01,
K_8_24*-3.598950e-01,
K_8_24*-6.983762e-01,
K_8_24*9.415441e-01,
K_8_24*-8.579731e-02,
K_8_24*-8.700870e-01,
K_8_24*8.104572e-01,
K_8_24*1.950903e-01,
K_8_24*-9.729400e-01,
K_8_24*6.152316e-01,
K_8_24*4.605387e-01,
K_8_24*-9.987955e-01,
K_8_24*3.713172e-01,
K_8_24*6.895405e-01,
K_8_24*-9.456073e-01,
K_8_24*9.801714e-02,
K_8_24*8.639729e-01,
K_8_24*-8.175848e-01,
K_8_24*-1.830399e-01,
K_8_24*9.700313e-01,
K_8_24*-6.248595e-01,
K_8_24*-4.496113e-01,
K_8_24*9.993224e-01,
K_8_24*-3.826834e-01,
K_8_24*-6.806010e-01,
K_8_24*9.495282e-01,
K_8_24*-1.102222e-01,
K_8_24*-8.577286e-01,
K_8_24*8.245893e-01,
K_8_24*1.709619e-01,
K_8_24*-9.669765e-01,
K_8_24*6.343933e-01,
K_8_24*4.386162e-01,
K_8_24*-9.996988e-01,
K_8_24*3.939920e-01,
K_8_24*6.715590e-01,
K_8_24*-9.533060e-01,
K_8_24*1.224107e-01,
K_8_24*8.513552e-01,
K_8_24*-8.314696e-01,
K_8_24*-1.588581e-01,
K_8_24*9.637761e-01,
K_8_24*-6.438315e-01,
K_8_24*-4.275551e-01,
K_8_24*9.999247e-01,
K_8_24*-4.052413e-01,
K_8_24*-6.624158e-01,
K_8_24*9.569403e-01,
K_8_24*-1.345807e-01,
K_8_24*-8.448536e-01,
K_8_24*8.382247e-01,
K_8_24*1.467305e-01,
K_8_24*-9.604305e-01,
K_8_24*6.531728e-01,
K_8_24*4.164296e-01,
K_8_24*-1,
K_8_24*4.164296e-01,
K_8_24*6.531728e-01,
K_8_24*-9.604305e-01,
K_8_24*1.467305e-01,
K_8_24*8.382247e-01,
K_8_24*-8.448536e-01,
K_8_24*-1.345807e-01,
K_8_24*9.569403e-01,
K_8_24*-6.624158e-01,
K_8_24*-4.052413e-01,
K_8_24*9.999247e-01,
K_8_24*-4.275551e-01,
K_8_24*-6.438315e-01,
K_8_24*9.637761e-01,
K_8_24*-1.588581e-01,
K_8_24*-8.314696e-01,
K_8_24*8.513552e-01,
K_8_24*1.224107e-01,
K_8_24*-9.533060e-01,
K_8_24*6.715590e-01,
K_8_24*3.939920e-01,
K_8_24*-9.996988e-01,
K_8_24*4.386162e-01,
K_8_24*6.343933e-01,
K_8_24*-9.669765e-01,
K_8_24*1.709619e-01,
K_8_24*8.245893e-01,
K_8_24*-8.577286e-01,
K_8_24*-1.102222e-01,
K_8_24*9.495282e-01,
K_8_24*-6.806010e-01,
K_8_24*-3.826834e-01,
K_8_24*9.993224e-01,
K_8_24*-4.496113e-01,
K_8_24*-6.248595e-01,
K_8_24*9.700313e-01,
K_8_24*-1.830399e-01,
K_8_24*-8.175848e-01,
K_8_24*8.639729e-01,
K_8_24*9.801714e-02,
K_8_24*-9.456073e-01,
K_8_24*6.895405e-01,
K_8_24*3.713172e-01,
K_8_24*-9.987955e-01,
K_8_24*4.605387e-01,
K_8_24*6.152316e-01,
K_8_24*-9.729400e-01,
K_8_24*1.950903e-01,
K_8_24*8.104572e-01,
K_8_24*-8.700870e-01,
K_8_24*-8.579731e-02,
K_8_24*9.415441e-01,
K_8_24*-6.983762e-01,
K_8_24*-3.598950e-01,
K_8_24*9.981181e-01,
K_8_24*-4.713967e-01,
K_8_24*-6.055110e-01,
K_8_24*9.757021e-01,
K_8_24*-2.071114e-01,
K_8_24*-8.032075e-01,
K_8_24*8.760701e-01,
K_8_24*7.356456e-02,
K_8_24*-9.373390e-01,
K_8_24*7.071068e-01,
K_8_24*3.484187e-01,
K_8_24*-9.972905e-01,
K_8_24*4.821838e-01,
K_8_24*5.956993e-01,
K_8_24*-9.783174e-01,
K_8_24*2.191012e-01,
K_8_24*7.958369e-01,
K_8_24*-8.819213e-01,
K_8_24*-6.132074e-02,
K_8_24*9.329928e-01,
K_8_24*-7.157308e-01,
K_8_24*-3.368899e-01,
K_8_24*9.963126e-01,
K_8_24*-4.928982e-01,
K_8_24*-5.857979e-01,
K_8_24*9.807853e-01,
K_8_24*-2.310581e-01,
K_8_24*-7.883464e-01,
K_8_24*8.876396e-01,
K_8_24*4.906767e-02,
K_8_24*-9.285061e-01,
K_8_24*7.242471e-01,
K_8_24*3.253103e-01,
K_8_24*-9.951847e-01,
K_8_24*5.035384e-01,
K_8_24*5.758082e-01,
K_8_24*-9.831055e-01,
K_8_24*2.429802e-01,
K_8_24*7.807372e-01,
K_8_24*-8.932243e-01,
K_8_24*-3.680722e-02,
K_8_24*9.238795e-01,
K_8_24*-7.326543e-01,
K_8_24*-3.136817e-01,
K_8_24*9.939070e-01,
K_8_24*-5.141027e-01,
K_8_24*-5.657318e-01,
K_8_24*9.852776e-01,
K_8_24*-2.548657e-01,
K_8_24*-7.730105e-01,
K_8_24*8.986745e-01,
K_8_24*2.454123e-02,
K_8_24*-9.191139e-01,
K_8_24*7.409511e-01,
K_8_24*3.020059e-01,
K_8_24*-9.924795e-01,
K_8_24*5.245897e-01,
K_8_24*5.555702e-01,
K_8_24*-9.873014e-01,
K_8_24*2.667128e-01,
K_8_24*7.651673e-01,
K_8_24*-9.039893e-01,
K_8_24*-1.227154e-02,
K_8_24*9.142098e-01,
K_8_24*-7.491364e-01,
K_8_24*-2.902847e-01,
K_8_24*9.909026e-01,
K_8_24*-5.349976e-01,
K_8_24*-5.453250e-01,
K_8_24*9.891765e-01,
K_8_24*-2.785197e-01,
K_8_24*-7.572088e-01,
K_8_24*9.091680e-01,
K_8_24*5.193617e-14,
};
