/******************************************************************************
* File Name          : i2c.h
* Author             : P Acco/ S DI Mercurio
* Version            : V1.0.0
* Date               : 24/06/2011
* Description        : This header file includes all header files from others drivers
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
#ifndef __SPI_H_
#define __SPI_H_

#include "stm_regs.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/*
 * Callback definition 
 * Used in IRQ for indicating end of transfert
 */
typedef void(*spiCallback)(int);

/* Constants for SPI driver */
#define SPI_LSB_FIRST			SPI_LSBFIRST
#define SPI_MSB_FIRST			0
					  
#define SPI_DATA_SIZE_8_BIT		0
#define SPI_DATA_SIZE_16_BIT	SPI_DFF

#define SPI_CLOCK_0_WHEN_IDLE	0
#define SPI_CLOCK_1_WHEN_IDLE	SPI_CPOL

#define SPI_FIRST_CAPTURE_EDGE	0
#define SPI_SECOND_CAPTURE_EDGE	SPI_CPHA

#define SPI_NSS_BY_HARDWARE		SPI_SSOE
#define SPI_NSS_DISABLED		0

#define SPI_PORT_REMAP			1
#define SPI_NO_PORT_REMAP		0

/*
 * SPI_Init
 *
 * Setup of SPI peripheral
 *
 * Input: handler: 	a valid pointer (non null) to an SPI_Handler structure
 * 					spi_channel: pointer to SPI registers. Valid values are: SPI1, SPI2, SPI3
 *					config: a filled configuration structure
 *
 * Output: handler pointed structure filled
 */
void SPI_Init(SPI_TypeDef* spi_channel, 
               int clock_speed, int clock_polarity, int clock_phase, int data_size, int bit_order, int remap);

/*
 * SPI_SetCallback
 *
 * Set callback to be used at end of transmission (when IRQ is configured)
 *
 * Input: handler: a valid pointer to the handler structure corresponding to SPI peripheral
 */
void SPI_SetCallback(SPI_TypeDef* spi_channel,
                     void(*spiCallback)(int));

/*
 * SPI_SetCS
 *
 * Define CS line to be used with SPI module
 *
 * Input: handler: a valid pointer to the handler structure corresponding to SPI peripheral
 */
void SPI_SetCS(SPI_TypeDef* spi_channel,
               GPIO_TypeDef* gpio, int pin, int active_level);

/*
 * SPI_Send
 *
 * Send a buffer of data 
 *
 * Input: handler: a valid pointer to the handler structure corresponding to SPI peripheral
 *        buffer: an 8-bit buffer containing data to be sent
 *        len: number of data to be sent.
 */
void SPI_Send(SPI_TypeDef* spi_channel, 
              void* buffer, int len);

/*
 * SPI_Receive
 *
 * Receive a buffer of data 
 *
 * Input: handler: a valid pointer to the handler structure corresponding to SPI peripheral
 *        buffer: an 8-bit buffer large enough for receiving data
 *        len: number of data to be received.
 */
void SPI_Receive(SPI_TypeDef* spi_channel, 
                 void* buffer, int len);

/*
 * SPI_SendReceive
 *
 * Receive 8-bit data 
 *
 * Input: handler: a valid pointer to the handler structure corresponding to SPI peripheral
 *        buffer: an 8-bit buffer large enough for receiving data
 *        len: number of data to be received.
 */
void SPI_SendReceive (SPI_TypeDef* spi_channel, 
                      void* buffer_in, int len_in,
					  void* buffer_out, int len_out);

/*
 * SPI_Receive
 *
 * Receive a buffer of data 
 *
 * Input: handler: a valid pointer to the handler structure corresponding to SPI peripheral
 *        buffer: an 8-bit buffer large enough for receiving data
 *        len: number of data to be received.
 */
int SPI_TransfertFinished (SPI_TypeDef* spi_channel);