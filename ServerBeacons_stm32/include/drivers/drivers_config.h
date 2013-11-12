/******************************************************************************
* File Name          : drivers_config.h
* Author             : P Acco/ S DI Mercurio
* Version            : V1.0.0
* Date               : 24/06/2011
* Description        : This header file contains functions description for SPI driver
*                     
********************************************************************************
* Thoses drivers are free software; you can redistribute them and/or
* modify them under the terms of the GNU General Public
* License as published by the Free Software Foundation;
* either version 2, or (at your option) any later version.
*
* Thoses drivers are distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied
* warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
* PURPOSE.  See the GNU General Public License for more
* details.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DRIVERS_CONFIG_H_
#define __DRIVERS_CONFIG_H_

/* Configuration for RCC (clock distribution) --------------------------------*/

/* Configuration for I2C -----------------------------------------------------*/

/* _I2C_USE_IRQ_
 * Set this line active in order to have I2C driver use IRQ and not polling
 * Comment this line in order to have I2C driver work in polling
 */
#define _I2C_USE_IRQ_

/* _I2C_IRQ_PRIO
 * Set IRQ priority: value must be between 0 an 15
 */
#define _I2C_IRQ_PRIO  3

/* _I2C_DONT_USE_I2C1_
 * _I2C_DONT_USE_I2C2_
 * Activate define corresponding to I2C unit you don't plan to use, in order to reduce code size
 * At least one I2C unit need to be kept active, otherwise don't include I2C driver in your build
 */
//#define _I2C_DONT_USE_I2C1_
//#define _I2C_DONT_USE_I2C2_

/* _I2C_DUTY_CYCLE_
 * Define duty cycle when running aove 100Khz (Fast mode)
 */
#define _I2C_DUTY_CYCLE_ 	I2C_DUTY_2

/* Configuration for SPI -----------------------------------------------------*/

/* _SPI_USE_IRQ_
 * Set this line active in order to have SPI driver use IRQ and not polling
 * Comment this line in order to have SPI driver work in polling
 */
#define _SPI_USE_IRQ_

/* _SPI_IRQ_PRIO
 * Set IRQ priority: value must be between 0 an 15
 */
#define _SPI_IRQ_PRIO  3

/* _SPI_DONT_USE_SPI1_
 * _SPI_DONT_USE_SPI2_
 * _SPI_DONT_USE_SPI2_
 * Activate define corresponding to SPI unit you don't plan to use, in order to reduce code size
 * At least one I2C unit need to be kept active, otherwise don't include SPI driver in your build
 */
//#define _SPI_DONT_USE_SPI1_
//#define _SPI_DONT_USE_SPI2_
//#define _SPI_DONT_USE_SPI2_

#endif /* __DRIVERS_CONFIG_H_ */

/******************* INSA 2010 ***************/
