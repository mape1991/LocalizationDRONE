/* File: startup_ARMCM3.S
 * Purpose: startup file for Cortex-M3 devices. Should use with 
 *   GCC for ARM Embedded Processors
 * Version: V1.2
 * Date: 15 Nov 2011
 * 
 * Copyright (c) 2011, ARM Limited
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the ARM Limited nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL ARM LIMITED BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
    .syntax unified
    .arch armv7-m
	.cpu cortex-m3
	.fpu softvfp
	
    .section .stack
    .align 3
#ifdef __STACK_SIZE
    .equ    Stack_Size, __STACK_SIZE
#else
    .equ    Stack_Size, 0xc00
#endif
    .globl    __StackTop
    .globl    __StackLimit
__StackLimit:
    .space    Stack_Size
    .size __StackLimit, . - __StackLimit
__StackTop:
    .size __StackTop, . - __StackTop

    .section .heap
    .align 3
#ifdef __HEAP_SIZE
    .equ    Heap_Size, __HEAP_SIZE
#else
    .equ    Heap_Size, 0x800
#endif
    .globl    __HeapBase
    .globl    __HeapLimit
__HeapBase:
    .space    Heap_Size
    .size __HeapBase, . - __HeapBase
__HeapLimit:
    .size __HeapLimit, . - __HeapLimit
    
    .section .isr_vector
    .align 2
    .globl __isr_vector
__isr_vector:
    .long    __StackTop            /* Top of Stack */
    .long    Reset_Handler         /* Reset Handler */
    .long    NMI_Handler           /* NMI Handler */
    .long    HardFault_Handler     /* Hard Fault Handler */
    .long    MemManage_Handler     /* MPU Fault Handler */
    .long    BusFault_Handler      /* Bus Fault Handler */
    .long    UsageFault_Handler    /* Usage Fault Handler */
    .long    0                     /* Reserved */
    .long    0                     /* Reserved */
    .long    0                     /* Reserved */
    .long    0                     /* Reserved */
    .long    SVC_Handler           /* SVCall Handler */
    .long    DebugMon_Handler      /* Debug Monitor Handler */
    .long    0                     /* Reserved */
    .long    PendSV_Handler        /* PendSV Handler */
    .long    SysTick_Handler       /* SysTick Handler */

    /* External interrupts */
    .long   WWDG_IRQHandler             /* Window Watchdog              */
    .long   PVD_IRQHandler              /* PVD through EXTI Line detect */
    .long   TAMPER_IRQHandler           /* Tamper                       */
    .long   RTC_IRQHandler              /* RTC                          */
    .long   FLASH_IRQHandler            /* Flash                        */
    .long   RCC_IRQHandler              /* RCC                          */
    .long   EXTI0_IRQHandler            /* EXTI Line 0                  */
    .long   EXTI1_IRQHandler            /* EXTI Line 1                  */
    .long   EXTI2_IRQHandler            /* EXTI Line 2                  */
    .long   EXTI3_IRQHandler            /* EXTI Line 3                  */
    .long   EXTI4_IRQHandler            /* EXTI Line 4                  */
    .long   DMAChannel1_IRQHandler      /* DMA Channel 1                */
    .long   DMAChannel2_IRQHandler      /* DMA Channel 2                */
    .long   DMAChannel3_IRQHandler      /* DMA Channel 3                */
    .long   DMAChannel4_IRQHandler      /* DMA Channel 4                */
    .long   DMAChannel5_IRQHandler      /* DMA Channel 5                */
    .long   DMAChannel6_IRQHandler      /* DMA Channel 6                */
    .long   DMAChannel7_IRQHandler      /* DMA Channel 7                */
    .long   ADC_IRQHandler              /* ADC                          */
    .long   USB_HP_CAN1_TX_IRQHandler   /* USB High Priority or CAN1 TX */
    .long   USB_LP_CAN1_RX0_IRQHandler  /* USB Low  Priority or CAN1 RX0*/ 
    .long   CAN1_RX1_IRQHandler         /* CAN1 RX1                     */
    .long   CAN1_SCE_IRQHandler         /* CAN1 SCE                     */
    .long   EXTI9_5_IRQHandler          /* EXTI Line 9..5               */
    .long   TIM1_BRK_IRQHandler         /* TIM1 Break                   */
    .long   TIM1_UP_IRQHandler          /* TIM1 Update                  */
    .long   TIM1_TRG_COM_IRQHandler     /* TIM1 Trigger and Commutation */
    .long   TIM1_CC_IRQHandler          /* TIM1 Capture Compare         */
    .long   TIM2_IRQHandler             /* TIM2                         */
    .long   TIM3_IRQHandler             /* TIM3                         */
    .long   TIM4_IRQHandler             /* TIM4                         */
    .long   I2C1_EV_IRQHandler          /* I2C1 Event                   */
    .long   I2C1_ER_IRQHandler          /* I2C1 Error                   */
    .long   I2C2_EV_IRQHandler          /* I2C2 Event                   */
    .long   I2C2_ER_IRQHandler          /* I2C2 Error                   */
    .long   SPI1_IRQHandler             /* SPI1                         */
    .long   SPI2_IRQHandler             /* SPI2                         */
    .long   USART1_IRQHandler           /* USART1                       */
    .long   USART2_IRQHandler           /* USART2                       */
    .long   USART3_IRQHandler           /* USART3                       */
    .long   EXTI15_10_IRQHandler        /* EXTI Line 15..10             */
    .long   RTCAlarm_IRQHandler         /* RTC Alarm through EXTI Line  */
    .long   USBWakeUp_IRQHandler        /* USB Wakeup from suspend      */
    
    .size    __isr_vector, . - __isr_vector

    .text
    .thumb
    .thumb_func
    .align 2
    .globl    Reset_Handler
    .type    Reset_Handler, %function
Reset_Handler:

/* 
 * Call SystemInit function (void SystemInit(void)) as soon as possible 
 */
	ldr     r0, =SystemInit
    blx     r0

/*     Loop to copy data from read only memory to RAM. The ranges
 *      of copy from/to are specified by following symbols evaluated in 
 *      linker script.
 *      __etext: End of code section, i.e., begin of data sections to copy from.
 *      __data_start__/__data_end__: RAM address range that data should be
 *      copied to. Both must be aligned to 4 bytes boundary.  */

    ldr     r1, =__etext
    ldr     r2, =__data_start__
    ldr     r3, =__data_end__

.flash_to_ram_loop:
    cmp     r2, r3
    ittt    lt
    ldrlt   r0, [r1], #4
    strlt   r0, [r2], #4
    blt     .flash_to_ram_loop

	eor     r0, r0
    ldr     r2, =__bss_start__
    ldr     r3, =__bss_end__

.flash_to_ram_bss_loop:
    cmp     r2, r3
    itt     lt
    strlt   r0, [r2], #4
    blt    .flash_to_ram_bss_loop

/*
 * Enable UsageFault, MemFault and Busfault interrupts
 */
	.equ    _SHCSR, 0xE000ED24		/* SHCSR is located at address 0xE000ED24 */
	ldr.w	r0, =_SHCSR				
	ldr 	r1, [r0]				/* Read CPACR */
	orr 	r1, r1, #(0x7 << 16)	/* Set bits 16,17,18 to enable usagefault, busfault, memfault interrupts */
	str 	r1, [r0]				/* Write back the modified value to the CPACR */
	dsb								/* Wait for store to complete */

/*
 * Set priority grouping (PRIGROUP) in AIRCR to 3 (16 levels for group priority and 0 for subpriority)
 */
    .equ	_AIRCR, 0xE000ED0C
	.equ    _AIRCR_VAL, 0x05FA0300
	ldr.w	r0, =_AIRCR
	ldr.w	r1, =_AIRCR_VAL
	str		r1,[r0]

/*
 * Finaly, jump to main function (void main (void))
 */
    ldr     r0, =main
    bx      r0
	
    .pool
    .size Reset_Handler, . - Reset_Handler

    .thumb
    .thumb_func
    .align 2
    .weak     SystemInit
    .globl    SystemInit
    .type    SystemInit, %function
SystemInit:

	bx 		lr
	.pool
    .size SystemInit, . - SystemInit

/* Exception Handlers */

    .weak   NMI_Handler
    .type   NMI_Handler, %function
	NMI_Handler:
    B       .
    .size   NMI_Handler, . - NMI_Handler

    .weak   HardFault_Handler
    .type   HardFault_Handler, %function
	HardFault_Handler:
    B       .
    .size   HardFault_Handler, . - HardFault_Handler

    .weak   MemManage_Handler
    .type   MemManage_Handler, %function
	MemManage_Handler:
    B       .
    .size   MemManage_Handler, . - MemManage_Handler

    .weak   BusFault_Handler
    .type   BusFault_Handler, %function
	BusFault_Handler:
    B       .
    .size   BusFault_Handler, . - BusFault_Handler

    .weak   UsageFault_Handler
    .type   UsageFault_Handler, %function
	UsageFault_Handler:
    B       .
    .size   UsageFault_Handler, . - UsageFault_Handler

    .weak   SVC_Handler
    .type   SVC_Handler, %function
	SVC_Handler:
    B       .
    .size   SVC_Handler, . - SVC_Handler

    .weak   DebugMon_Handler
    .type   DebugMon_Handler, %function
	DebugMon_Handler:
    B       .
    .size   DebugMon_Handler, . - DebugMon_Handler

    .weak   PendSV_Handler
    .type   PendSV_Handler, %function
	PendSV_Handler:
    B       .
    .size   PendSV_Handler, . - PendSV_Handler

    .weak   SysTick_Handler
    .type   SysTick_Handler, %function
	SysTick_Handler:
    B       .
    .size   SysTick_Handler, . - SysTick_Handler

    /* others IRQ Handlers are on the same loop Default_Handler */

    .type   Default_Handler, %function
	Default_Handler:
    B       .
    .size   Default_Handler, . - Default_Handler

	/* This is the macro to point an Handler on Default_Handler */
    .macro  IRQ handler
    	.weak   \handler
    	.type   \handler, %function
		.set    \handler, Default_Handler
    .endm

   	/* just use the macro to define all others*/
    IRQ     WWDG_IRQHandler
    IRQ     PVD_IRQHandler
    IRQ     TAMPER_IRQHandler
    IRQ     RTC_IRQHandler
    IRQ     FLASH_IRQHandler
    IRQ     RCC_IRQHandler
    IRQ     EXTI0_IRQHandler
    IRQ     EXTI1_IRQHandler
    IRQ     EXTI2_IRQHandler
    IRQ     EXTI3_IRQHandler
    IRQ     EXTI4_IRQHandler
    IRQ     DMAChannel1_IRQHandler
    IRQ     DMAChannel2_IRQHandler
    IRQ     DMAChannel3_IRQHandler
    IRQ     DMAChannel4_IRQHandler
    IRQ     DMAChannel5_IRQHandler
    IRQ     DMAChannel6_IRQHandler
    IRQ     DMAChannel7_IRQHandler
    IRQ     ADC_IRQHandler
    IRQ     USB_HP_CAN1_TX_IRQHandler
    IRQ     USB_LP_CAN1_RX0_IRQHandler
    IRQ     CAN1_RX1_IRQHandler
    IRQ     CAN1_SCE_IRQHandler
    IRQ     EXTI9_5_IRQHandler
    IRQ     TIM1_BRK_IRQHandler
    IRQ     TIM1_UP_IRQHandler
    IRQ     TIM1_TRG_COM_IRQHandler
    IRQ     TIM1_CC_IRQHandler
    IRQ     TIM2_IRQHandler
    IRQ     TIM3_IRQHandler
    IRQ     TIM4_IRQHandler
    IRQ     I2C1_EV_IRQHandler
    IRQ     I2C1_ER_IRQHandler
    IRQ     I2C2_EV_IRQHandler
    IRQ     I2C2_ER_IRQHandler
    IRQ     SPI1_IRQHandler
    IRQ     SPI2_IRQHandler
    IRQ     USART1_IRQHandler
    IRQ     USART2_IRQHandler
    IRQ     USART3_IRQHandler
    IRQ     EXTI15_10_IRQHandler
    IRQ     RTCAlarm_IRQHandler
    IRQ     USBWakeUp_IRQHandler
