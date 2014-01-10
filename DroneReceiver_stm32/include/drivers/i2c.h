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
#ifndef __I2C_H_
#define __I2C_H_

#include "stm_regs.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* 
 * Constants to be used with I2C_Master_Write function
 */
#define I2C_STOP_AFTER_WRITE	1
#define I2C_NO_STOP_AFTER_WRITE	0

/* 
 * Output state constants returned by I2C_Master_Write and I2C_Master_Read functions
 */
#define I2C_OK				0
#define I2C_ERR_START		-1
#define I2C_ERR_ADDR		-2
#define I2C_ERR_DATA_NACK	-3
#define I2C_ERR_STOP		-4

#define __disable_irq()		SYS_DISABLE_GLOBAL_INTERRUPTS()
#define __enable_irq()		SYS_ENABLE_GLOBAL_INTERRUPTS()

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/*
 * I2C_Master_Read
 *
 * Reads buffer of bytes from the slave.
 *
 * Input: I2C_Channel: Name of I2C unit to use
 *        pBuffer: Buffer of bytes to be read from the slave.
 *        NumByteToRead: Number of bytes to be read by the Master.
 *        SlaveAddress: The address of the slave to be addressed by the Master.
 *
 * Output: status of the operation (see constants defined for returned state
 */

int I2C_Master_Read(I2C_TypeDef* I2C_Channel, unsigned char* pBuffer,  
                    int NumByteToRead,  unsigned int SlaveAddress);

/*
 * I2C_Master_Write
 *
 * Write buffer of bytes from the slave.
 *
 * Input: I2C_Channel: Name of I2C unit to use
 *        pBuffer: Buffer of bytes to be sent to the slave.
 *        NumByteToWrite: Number of bytes to be sent by the Master.
 *        SlaveAddress: The address of the slave to be addressed by the Master.
 *		  Stop: Send stop condition after last byte send or not
 *
 * Output: status of the operation (see constants defined for returned state
 */

int I2C_Master_Write(I2C_TypeDef* I2C_Channel, unsigned char* pBuffer,  
                     int NumByteToWrite, unsigned int SlaveAddress, int Stop);

/*
 * I2C_Init
 *
 * Initializes peripherals: I2Cx, GPIO
 *
 * Input:  I2C_Channel: I2C channel to configure
 *         Speed: the spped to use for I2C (no more than 400000 Hz, otherwise truncated
 *
 * Output: none
 */
void I2C_Init(I2C_TypeDef* I2C_channel, int speed);

#endif /*__I2C_H_ */

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
