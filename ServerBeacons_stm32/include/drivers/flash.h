/*
 * Copyright (C) INSA Toulouse
 * Author: Sebastien DI MERCURIO
 *
 * This file is part of MinOS.
 *
 * lib_cm3 is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation;
 * either version 2, or (at your option) any later version.
 *
 * lib_cm3 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public
 * License along with lib_cm3; see the file COPYING.  If not,
 * write to the Free Software Foundation, Inc., 51 Franklin Street,
 * Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "stm_regs.h"

#define FLASH_BUSY 		1
#define FLASH_ERROR_PG 	2
#define FLASH_ERROR_WRP	3
#define FLASH_COMPLETE	4
#define FLASH_TIMEOUT	5

int FLASH_EraseAllPages(void) ;
int FLASH_ErasePage(unsigned int addr);
int FLASH_Program(unsigned int addr, u16 *data, int length);


