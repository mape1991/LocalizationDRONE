/*----------------------------------------------------------------------------
 * Name:    clock.c
 * Purpose: Pilote d'horloge générale.
 *
 * Version: V1.2
 * Auteurs: P. ACCO (INSA - Toulouse,France)
 *          S. DI MERCURIO (INSA - Toulouse,France)
 *----------------------------------------------------------------------------
 * Ce programme fait partie des pilotes pour STM32 de l'INSA de Toulouse, France.
 * Merci de consulter le fichier LICENCE.txt au sujet des conditions de redistribution,
 * modification et copyright concernant ce pilote. Le fichier LICENCE.txt doit faire partie
 * des fichiers fournis avec ce pilote.
 *
 * Copyright (c) 2012, INSA-GEI Toulouse, France.
 * Tout droits réservés.
 *---------------------------------------------------------------------------*/

#include "stm32f10x.h"

#include "clock_conf.h"
#include "clock.h"

static u16 CLOCK_HPRECompute(int hpre);
	
/*
 * Message d'alerte à l'attention de l'utilisateur
 */
#ifdef STM32F10X_CL
	#warning "La configuration d'horloge sur la famille Connectivity Line n'a jamais été testé en vrai: A VOS RISQUES ET PERILS !!!"
#endif
/*
 * Vérification de la configuration et des fréquences. Émet des erreurs en cas d'anomalie.
 *
 * Vérifie les bornes de __SYSCLK, __HCLK, __PCLK1, __PCLK2, __ADCCLK, __TIMXCLK, __TIMxCLK
 */
#if (__HSE<4000000UL)
	#error "__HSE crystal freq. too slow ! should be more than 4000000UL. Forgot a zero buddy ?"
#endif

#ifndef STM32F10X_CL
	#if (__HSE>16000000UL)
		#error "__HSE crystal freq. too high ! should be less than 16000000UL. Too much zeros buddy ?"
	#endif
#else
	#if (__HSE>25000000UL)
		#error "__HSE crystal freq. too high ! should be less than 25000000UL. Too much zeros buddy ?"
	#endif
#endif

#if ((_SYSCLK_SOURCE!=IS_HSI)&&(_SYSCLK_SOURCE!=IS_HSE)&&(_SYSCLK_SOURCE!=IS_PLL))
	#error "Invalid _SYSCLK_SOURCE should be IS_HSI, IS_HSE or IS_PLL"
#endif

#ifndef STM32F10X_CL
	#if ((_PLL_SOURCE!=IS_HSI)&&(_PLL_SOURCE!=IS_HSE))
		#error "Invalid _PLL_SOURCE should be IS_HSI or IS_HSE"
	#endif
#else
	#if ((_PLL_SOURCE!=IS_HSI)&&(_PLL_SOURCE!=IS_PREDIV_1))
		#error "Invalid _PLL_SOURCE should be IS_HSI or IS_PREDIV_1"
	#endif
#endif

#ifndef STM32F10X_CL
	#if ((_PLLXTPRE!=IS_HSE)&&(_PLLXTPRE!=IS_HSE_DIV_2))
		#error "Invalid _PLLXTPRE should be IS_HSE or IS_HSE_DIV_2"
	#endif

	#if (_PLLMUL > 16)
		#error "_PLLMUL value too high! Should be from 2 to 16"
	#elif (_PLLMUL < 2)
		#error "_PLLMUL value too low! Should be from 2 to 16"
	#endif
#else
	#if (_PLL_SOURCE == IS_PREDIV_1)
		#if (_PREDIV_1>16)
			#error "_PREDIV_1 should be less or equal to 16"
		#endif

		#if (_PREDIV_1<1)
			#error "_PREDIV_1 should be more or equal to 1"
		#endif

		#if ((_PREDIV_1_SOURCE!=IS_HSE) && (_PREDIV_1_SOURCE!=IS_PLL2))
			#error "_PREDIV_1_SOURCE should be set to either IS_HSE or IS_PLL2"
		#endif

		#if (_PREDIV_1_SOURCE==IS_PLL2)
			#if (_PLL2MUL>20)
				#error "_PLL2MUL should be less or equal to 20"
			#endif

			#if (_PLL2MUL<8)
				#error "_PLL2MUL should be more or equal to 8"
			#endif

			#if (_PREDIV_2>16)
				#error "_PREDIV_2 should be less or equal to 16"
			#endif

			#if (_PREDIV_2<1)
				#error "_PREDIV_2 should be more or equal to 1"
			#endif
		#endif
	#endif

	#if ((_PLLMUL != 4) && (_PLLMUL != 5) && (_PLLMUL != 6) && (_PLLMUL != 7) && (_PLLMUL != 8) && (_PLLMUL != 9) && (_PLLMUL != _PLLMUL_6_5))
		#error "_PLLMUL value invalid! Should be either 4,5,6,7,8,9 or _PLLMUL_6_5"
	#endif
#endif

#if ((_HPRE!=1)&&(_HPRE!=2)&&(_HPRE != 4)&&(_HPRE!=8)&&(_HPRE!=16)&&(_HPRE!=64)&&(_HPRE!=128)&&(_HPRE!=256)&&(_HPRE!=512) )
	#error "_HPRE value should be 1,2,4,8,16,64,128,256 or 512 (32 is not on the list!)"
#endif

#if ((_PPRE1!=1)&&(_PPRE1!=2)&&(_PPRE1!=4)&&(_PPRE1!=8)&&(_PPRE1!=16) )
	#error "_PPRE1 value should be 1,2,4,8 or 16"
#endif

#if ((_PPRE2!=1)&&(_PPRE2!=2)&&(_PPRE2!=4)&&(_PPRE2!=8)&&(_PPRE2!=16) )
	#error "_PPRE2 value should be 1,2,4,8 or 16"
#endif

#if ((_ADCPRE!=2)&&(_ADCPRE!=4)&&(_ADCPRE!=6)&&(_ADCPRE!=8) )
	#error "_ADCPRE value should be 2,4,6 or 8"
#endif

#ifdef STM32F10X_CL
	#if ((_USB_PRE!=2) && (_USB_PRE!=3))
		#error "_USB_PRE should be equal to either 2 or 3 on STM32F107"
	#endif
#else
#ifdef STM32F10X_MD
	#if ((_USB_PRE!=1) && (_USB_PRE!=_USB_PRE_1_5))
		#error "_USB_PRE should be equal to either 1 or _USB_PRE_1_5 on STM32F103"
	#endif
#endif
#endif
/*
 * Calcul de __SYSCLK (max 72 Mz sur STM32F10x)
 */
#if   (_SYSCLK_SOURCE==IS_HSI)
	#define __SYSCLK (__HSI)
#elif (_SYSCLK_SOURCE==IS_HSE)
	#define __SYSCLK (__HSE)
#elif (_SYSCLK_SOURCE==IS_PLL)
	#ifdef STM32F10X_CL
		#if (_PLL_SOURCE==IS_HSI)
			#define INPLL_CLK (__HSI/2)
		#elif(_PLL_SOURCE==IS_PREDIV_1)
			#if (_PREDIV_1_SOURCE==IS_PLL2)
				#define INPLL_CLK ((__HSE*_PLL2MUL)/(_PREDIV_2*_PREDIV_1))
			#else
				#define INPLL_CLK (__HSE/_PREDIV_1)
			#endif
		#endif

		#if (_PLLMUL!=_PLLMUL_6_5)
			#define __PLLCLK ((_PLLMUL)*(INPLL_CLK))
		#else
			#define __PLLCLK ((_PLLMUL)*(INPLL_CLK))
		#endif
	#else
		#if (_PLL_SOURCE==IS_HSI)
			#define INPLL_CLK (__HSI/2)
		#else
			#if (_PLLXTPRE==IS_HSE)
				#define INPLL_CLK (__HSE)
			#else
				#define INPLL_CLK (__HSE/2)
			#endif
		#endif

		#define __PLLCLK ((_PLLMUL)*(INPLL_CLK))
	#endif

	#define __SYSCLK (__PLLCLK)
#else
	#define __SYSCLK (0)
#endif

#if ((__SYSCLK)>72000000UL)
#ifdef STM32F10X_CL
	#error "PLL output frequency should not exceed 72 MHz ! Reduce __PLMULL, _PLL2MUL, increase _PREDIV_1 or _PREDIV_2 or set _PREDIV_1_SOURCE to another value"
#else
	#error "PLL output frequency should not exceed 72 MHz ! Reduce __PLMULL or set _PLLXTPRE to IS_DIVIDED_BY_2"
#endif
#endif

/*
 * Calcul de USBCLK
 */
#ifdef STM32F10X_MD
	#define __USBCLK (__PLLCLK/_USB_PRE)
#else
	#define __USBCLK ((__PLLCLK*2)/_USB_PRE)
#endif
/*
 * Calcul de __HCLK
 */
#define __HCLK	((__SYSCLK)/(_HPRE))

/*
 * Calcul de __PCLK1 (max 36 Mhz)
 */
#define __PCLK1 ((__HCLK)/(_PPRE1))
#if ((__PCLK1)>36000000UL)
	#error "APB1 output frequency should not exceed 36 MHz ! Increase _PPRE1 or _HPRE prescaler ratio"
#endif
#if (_PPRE1==1)
	#define __TIMXCLK (__PCLK1)
#else
	#define __TIMXCLK (__PCLK1*2UL)
#endif

/*
 * Calcul de __PCLK2 (max 72 Mhz)
 */
#define __PCLK2 ((__HCLK)/(_PPRE2))
#if (_PPRE2==1)
	#define __TIMxCLK (__PCLK2)
#else
	#define __TIMxCLK (__PCLK2*2UL)
#endif

/*
 * Calcul de __ADCCLK (max 14 Mhz)
 */
#define __ADCCLK ((__PCLK2)/(_ADCPRE))
#if ((__ADCCLK)>14000000UL)
	#error "ADC prescaler output frequency should not exceed 14 MHz ! Increase _ADCPRE or _HPRE prescaler ratio"
#endif

#define __RCC_CR_RESET    	0x00000083
#define __RCC_CFGR_RESET	0x00000000
#define __RCC_CIR_RESET		0x00000000
#define __RCC_CFGR2_RESET	0x00000000

#ifdef STM32F10X_MD
	#if (_PLLXTPRE == IS_HSE)
		#define _PLLXTPRE_MASK (0<<17)
	#else
		#define _PLLXTPRE_MASK (1<<17)
	#endif
#endif
/*
 * Configure la chaîne d'horloge pour STM32F103
 */
#ifdef STM32F10X_MD
	#if ( (_SYSCLK_SOURCE  == IS_PLL) && (_PLL_SOURCE  == IS_HSE) )
		#define __RCC_CFGR_VALUE	(RCC_PLLMUL_VAL(_PLLMUL)| 	\
									_PLLXTPRE_MASK| 			\
									RCC_PLLSRC_IS_HSE| 			\
									RCC_ADCPRE_VAL(_ADCPRE)| 	\
									RCC_CFGR_SW_HSI| 			\
									RCC_PPRE1_VAL(_PPRE1)| 		\
									RCC_PPRE2_VAL(_PPRE2)) 
		#define HSE_IS_USED
		#define PLL_IS_USED
	#elif ( (_SYSCLK_SOURCE  == IS_PLL) && (_PLL_SOURCE  == IS_HSI) )
		#define __RCC_CFGR_VALUE	(RCC_PLLMUL_VAL(_PLLMUL)| 	\
									_PLLXTPRE_MASK| 			\
									RCC_PLLSRC_IS_HSI| 			\
									RCC_ADCPRE_VAL(_ADCPRE)| 	\
									RCC_CFGR_SW_HSI| 			\
									RCC_PPRE1_VAL(_PPRE1)| 		\
									RCC_PPRE2_VAL(_PPRE2)) 
		#define HSI_IS_USED
		#define PLL_IS_USED
	#elif (_SYSCLK_SOURCE  == IS_HSI)
		#define __RCC_CFGR_VALUE	(RCC_ADCPRE_VAL(_ADCPRE)| 	\
									RCC_CFGR_SW_HSI| 			\
									RCC_PPRE1_VAL(_PPRE1)| 		\
									RCC_PPRE2_VAL(_PPRE2)) 
		#define HSI_IS_USED
	#elif (_SYSCLK_SOURCE  == IS_HSE)
		#define __RCC_CFGR_VALUE	(RCC_ADCPRE_VAL(_ADCPRE)| 	\
									RCC_CFGR_SW_HSI| 			\
									RCC_PPRE1_VAL(_PPRE1)| 		\
									RCC_PPRE2_VAL(_PPRE2)) 
		#define HSE_IS_USED
	#endif
#else
	#ifdef STM32F10X_CL
	#if ( (_SYSCLK_SOURCE  == IS_PLL) && (_PLL_SOURCE  == IS_PREDIV_1) )
		#define __RCC_CFGR_VALUE	(RCC_PLLMUL_VAL(_PLLMUL)| 	\
									RCC_PLLSRC_IS_HSE| 			\
									RCC_ADCPRE_VAL(_ADCPRE)| 	\
									RCC_CFGR_SW_HSI| 			\
									RCC_PPRE1_VAL(_PPRE1)| 		\
									RCC_PPRE2_VAL(_PPRE2)) 
		
		// Modif Thierry : decalages -1 sur _PREDIV_1 et 2 et - 2 sur _PLL2MUL
		#define __RCC_CFGR2_VALUE_TMP   ((_PREDIV_1 -1) | 			\
				                     ((_PREDIV_2-1)<<4)| 			\
				                     ((_PLL2MUL - 2)<<8))

		#if (_PREDIV_1_SOURCE == IS_HSE)
			#define __RCC_CFGR2_VALUE __RCC_CFGR2_VALUE_TMP

			#define HSE_IS_USED
			#define PLL_IS_USED
		#else
			#define __RCC_CFGR2_VALUE (__RCC_CFGR2_VALUE_TMP|0x00010000)

			#define HSE_IS_USED
			#define PLL2_IS_USED
			#define PLL_IS_USED
		#endif
	#elif ( (_SYSCLK_SOURCE  == IS_PLL) && (_PLL_SOURCE  == IS_HSI) )
		#define __RCC_CFGR_VALUE	(RCC_PLLMUL_VAL(_PLLMUL)| 	\
									RCC_PLLSRC_IS_HSI| 			\
									RCC_ADCPRE_VAL(_ADCPRE)| 	\
									RCC_CFGR_SW_HSI| 			\
									RCC_PPRE1_VAL(_PPRE1)| 		\
									RCC_PPRE2_VAL(_PPRE2)) 

		#define __RCC_CFGR2_VALUE_TMP	0

		#define HSI_IS_USED
		#define PLL_IS_USED
	#elif (_SYSCLK_SOURCE  == IS_HSI)
		#define __RCC_CFGR_VALUE	(RCC_ADCPRE_VAL(_ADCPRE)| 	\
									RCC_CFGR_SW_HSI| 			\
									RCC_PPRE1_VAL(_PPRE1)| 		\
									RCC_PPRE2_VAL(_PPRE2)) 

		#define __RCC_CFGR2_VALUE_TMP	0

		#define HSI_IS_USED
	#elif (_SYSCLK_SOURCE  == IS_HSE)
		#define __RCC_CFGR_VALUE	(RCC_ADCPRE_VAL(_ADCPRE)| 	\
									RCC_CFGR_SW_HSI| 			\
									RCC_PPRE1_VAL(_PPRE1)| 		\
									RCC_PPRE2_VAL(_PPRE2)) 

		#define __RCC_CFGR2_VALUE_TMP	0

		#define HSE_IS_USED
	#endif
#endif /* _defined STM32F10X_CL */
#endif /* _defined STM32F10X_MD */


static u16 CLOCK_HPRECompute(int hpre)
{
	u16 val;
	
	val = RCC_HPRE_VAL(hpre);
	if (val==(7<<RCC_HPRE_SHIFT)) val =0;
	
	return val;
}

/*
 * Configuration de l'horloge
 * Paramètres: Aucun
 * Retour: Aucun
 */
void CLOCK_Configure(void)
{ 
	RCC->CR		=__RCC_CR_RESET;
	RCC->CFGR	=__RCC_CFGR_RESET;
	RCC->CIR	=__RCC_CIR_RESET;

#ifdef STM32F10X_CL
	RCC->CFGR2  =__RCC_CFGR2_RESET;
#endif

	RCC->CFGR 	=__RCC_CFGR_VALUE;
	RCC->CFGR 	|= CLOCK_HPRECompute(_HPRE);
	
#ifdef STM32F10X_CL
	RCC->CFGR2  = __RCC_CFGR2_VALUE;
#endif

	/* Réglage de la flash (waitstates) */
	FLASH->ACR = 0x0; /* RAZ du contrôleur de flash -> 0 WS, pas de buffer de prefetch  */

	#if (__SYSCLK <=24000000UL)
		FLASH->ACR |= FLASH_ACR_LATENCY_0;
	#elif (__SYSCLK <=48000000UL) 
		FLASH->ACR |= FLASH_ACR_LATENCY_1;
	#else
		FLASH->ACR |= FLASH_ACR_LATENCY_2;
	#endif 	 
	 
	#ifdef HSE_IS_USED
		//activate HSE
		RCC->CR |= RCC_CR_HSEON;
		//wait for HSE to be ready
	  	while((RCC->CR & RCC_CR_HSERDY) == 0);
	
		#ifndef PLL_IS_USED
		// switch clock to HSE
		RCC->CFGR &= ~(RCC_CFGR_SW);
		RCC->CFGR |= RCC_CFGR_SW_HSE;
		#endif
	#endif  

	#ifdef HSI_IS_USED
		//activate HSI
		RCC->CR |= RCC_CR_HSION;
		//wait for HSI to be ready
	  	while((RCC->CR & RCC_CR_HSIRDY) == 0);

		#ifndef PLL_IS_USED
		// switch clock to HSI
		RCC->CFGR &= ~(RCC_CFGR_SW);
		RCC->CFGR |= RCC_CFGR_SW_HSI;
		#endif
	#endif  

	#ifdef PLL2_IS_USED /* Activation de la PLL2 avant la PLL1 */
		// activate PLL2
		RCC->CR |= RCC_CR_PLL2ON;
		//wait for PLL2 to be ready
		while((RCC->CR & RCC_CR_PLL2RDY) == 0);
	#endif

	#ifdef PLL_IS_USED
		//activate PLL
		RCC->CR |= RCC_CR_PLLON;
		//wait for PLL to be ready
		while((RCC->CR & RCC_CR_PLLRDY) == 0);
		
		// switch clock to PLL
		RCC->CFGR &= ~(RCC_CFGR_SW);
		RCC->CFGR |= RCC_CFGR_SW_PLL;
	#endif

#ifdef STM32F10X_CL
	#if (_USB_PRE == 3)
		RCC->CFGR |= RCC_CFGR_OTGFSPRE;
	#endif
#else
	#if (_USB_PRE == _USB_PRE_1_5)
		RCC->CFGR |= RCC_CFGR_USBPRE;
	#endif
#endif

	FLASH->ACR |= FLASH_ACR_PRFTBE; /* Buffer de prefetch activ� */
	
	#if ((_HPRE == 1) && (__SYSCLK <8000000UL))
		FLASH->ACR |= FLASH_ACR_HLFCYA; /* Half cycle flash access (possible seulement si sysclk < 8Mhz
		                               et HCLK == SYSCLK*/	
	#endif	 	 
}

/*
 * Retourne la fréquence, en Hz, du timer indiqué en paramètre
 * Paramètres:    timer: Pointeur sur la structure de registre du timer
 * Retour:       Fréquence, en Hz de PCLK2
 */
u32 CLOCK_GetTIMCLK (void *TimPtr)
{
#ifdef TIM8
	if ((TimPtr == TIM1) || (TimPtr == TIM8))
#else
	if (TimPtr == TIM1)
#endif
	{
		return (u32)__TIMxCLK;
	}
	else
	{
		return (u32)__TIMXCLK;
	}
}

/*
 *   Retourne la fréquence, en Hz, de l'horloge HCLK, sortie du diviseur AHB
 *   Parameter:    none
 *   Return:       Fréquence, en Hz de HCLK
 */
u32 CLOCK_GetHCLK(void)
{
	return (u32)__HCLK;
}

/*
 *   Retourne la fréquence, en Hz, de l'horloge PCLK1, sortie du diviseur APB1 (low speed)
 *   Parameter:    none
 *   Return:       Fréquence, en Hz de PCLK1
 */
u32 CLOCK_GetPCLK1(void)
{
	return (u32)__PCLK1;
}

/*
 *   Retourne la fréquence, en Hz, de l'horloge PCLK2, sortie du diviseur APB2 (high speed)
 *   Parameter:    none
 *   Return:       Fréquence, en Hz de PCLK2
 */
u32 CLOCK_GetPCLK2(void)
{
	return (u32)__PCLK2;
}

/*
 *   Retourne la fréquence, en Hz, de l'horloge ADC, après le diviseur ADC
 *   Parameter:    none
 *   Return:       Fréquence, en Hz de l'ADC
 */
u32 CLOCK_GetADCCLK (void)
{
	return (u32)__ADCCLK;
}


