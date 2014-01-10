/**
 * \file
 * \brief Fichier de configuration des horloges
 * \author Pascal ACCO (INSA - Toulouse,France)
 * \author Sébastien DI MERCURIO (INSA - Toulouse,France)
 * \version 1.0
 * \date 17/02/2012
 *
 * Ce fichier fait partie des pilotes pour STM32 de l'INSA de Toulouse, France.
 * Merci de consulter le fichier LICENCE.txt au sujet des conditions de redistribution,
 * modification et copyright concernant ce pilote. Le fichier LICENCE.txt doit faire partie
 * des fichiers fournis avec ce pilote.
 *
 * Copyright (c) 2012, INSA-GEI Toulouse, France.
 * Tout droits réservés.
 */

#ifndef __CLOCK_CONF_H__
#define __CLOCK_CONF_H__

#if !defined(STM32F10X_MD) && !defined (STM32F10X_CL)
	#error "Aucune cible definie, selectionnez soit STM32F10X_CL, soit STM32F10X_MD "
#endif

#define RCC_PLLSRC							(1<<16)	     // PLL entry clock source
#define RCC_PLLSRC_IS_HSI 			(0<<16)
#define RCC_PLLSRC_IS_HSE 			(1<<16)
#define RCC_PLLSRC_IS_PREDIV_1 	(1<<16)

#define RCC_PLLMUL_SHIFT				(18)
#define RCC_PLLMUL_VAL(mult) (((mult-2) & 0xF )<<RCC_PLLMUL_SHIFT)

#define RCC_ADCPRE_SHIFT				(14)
#define RCC_ADCPRE_VAL(pres) ((((pres/2)-1) & 0x3 )<<RCC_ADCPRE_SHIFT)

#define RCC_PPRE1_SHIFT					(8)
#define RCC_PPRE1_VAL(pre) ((pre==1) ? 0 : ((pre/2)-(pre/8)-2*(pre/16)+3)<<RCC_PPRE1_SHIFT)

#define RCC_PPRE2_SHIFT					(11)
#define RCC_PPRE2_VAL(pre) ((pre==1) ? 0 : ((pre/2)-(pre/8)-2*(pre/16)+3)<<RCC_PPRE2_SHIFT)

#define RCC_HPRE_SHIFT					(4)
#define RCC_HPRE_VAL(pre) ( (pre==1) ? 0 : (7+(pre>256)+(pre>128)+(pre>64)+(pre>16)+(pre>8)+(pre>4)+(pre>2)+(pre>1))<<RCC_HPRE_SHIFT )

	/** @addtogroup conf_CLOCK Configuration du pilote CLOCK
	 * @{
	 */

#define	 IS_HSI 1 /**< Indique l'utilisation de l'horloge HSI */
#define	 IS_HSE 2 /**< Indique l'utilisation de l'horloge HSE */
#define	 IS_PREDIV_1 7 /**< Indique l'utilisation du prediviseur 1 */
#define	 IS_PLL 3  /**< Indique l'utilisation de la PLL 1 */
#define	 IS_PLL2 4 /**< Indique l'utilisation du la PLL 2 (uniquement sur STM32F107) */
#define	 IS_HSE_DIV_2 5 /**< Indique l'utilisation de l'entrée HSE divisée par 2 (uniquement sur STM32F103 pour _PLLXTPRE) */
#define  _PLLMUL_6_5 (65/10)
#define  _USB_PRE_1_5 (15/10)

/**
 * \brief Constante fixant la vitesse de l'horloge interne
 */
#define __HSI 8000000UL	  // High Speed Internal RC clock (8MHz) on STM32F103xx

/**
 * \def __HSE
 * \brief Constante fixant la vitesse du quartz externe
 *
 * \note Normalement, le quartz externe (__HSE) vaut:
 * - 8000000L (8Mhz) pour STM32F10X_MD
 * - 25000000L (25Mhz) pour STM32F10X_CL
 */
#ifdef STM32F10X_MD
	#define __HSE 8000000UL
#endif
#ifdef STM32F10X_CL
	#define __HSE 25000000UL
#endif

/**
 * \def _SYSCLK_SOURCE
 * \brief Configure SYSCLK
 *
 * Configure la source d'horloge à utilise pour SYSCLK.
 * Les valeurs possibles que peut prendre _SYSCLK_SOURCE sont:
 * - IS_HSI: Utilisation de l'horloge interne (High Speed)
 * - IS_HSE: Utilisation de l'horloge externe (High Speed)
 * - IS_PLL: Utilisation de la PLL pour augmenter la frequence d'horloge
 */
 
#define _SYSCLK_SOURCE IS_PLL

/**
 * \def _PLL_SOURCE
 * \brief Configure la PLL
 *
 * Configure la source d'horloge à utilise en entrée de la PLL.
 * Les valeurs possibles que peut prendre _PLL_SOURCE sont:
 * - IS_HSI: Utilisation de l'horloge interne (High Speed)
 * - IS_HSE: Utilisation de l'horloge externe (High Speed) (si STM32F10X_MD est défini)
 * - IS_PREDIV_1: Utilisation de la sortie du prediviseur 1 (si STM32F10X_CL est défini)
 *
 * \note Si l'USB doit être utilisé, il faut obligatoirement sélectionner comme source IS_HSE,
 * la HSI n'etant pas assez stable pour pouvoir utiliser l'USB.
 */
#ifdef STM32F10X_CL
	#define _PLL_SOURCE IS_PREDIV_1
#else
	#define _PLL_SOURCE IS_HSE
#endif

#ifndef STM32F10X_CL
/**
 * \def _PLLXTPRE
 * \brief Configure le prescaler en entrée de la PLL
 *
 * Configure le prescaler en entrée de la PLL.
 * Les valeurs possibles que peut prendre _PLLXTPRE sont:
 * - IS_HSE: L'horloge HSE est directement envoyé en entrée de la PLL
 * - IS_HSE_DIV_2: L'horloge HSE est divisé par 2 avant d'etre fournie à la PLL
 * \note Uniquement disponible si la cible est STM32F10X_MD
 */
#define _PLLXTPRE IS_HSE
#else
/**
 * \def _PREDIV_1
 * \brief Configure le prescaler en entrée de la PLL 1
 *
 * Configure le prescaler en entrée de la PLL 1.
 * Les valeurs possibles que peut prendre _PREDIV_1 vont de 1 à 16.
 *
 * \note Uniquement disponible si la cible est STM32F10X_CL
 */
#define _PREDIV_1 5

/**
 * \def _PREDIV_1_SOURCE
 * \brief Configure la source du prescaler en entrée de la PLL1
 *
 * Configure la source du prescaler en entrée de la PLL1.
 * Les valeurs possibles que peut prendre _PREDIV_1_SOURCE sont:
 * - IS_PLL2 : La source du prescaler de la PLL1 est la sortie de la PLL2
 * - IS_HSE : La source du prescaler de la PLL1 est le quartz externe HSE
 *
 * \note Uniquement disponible si la cible est STM32F10X_CL
 */
#define _PREDIV_1_SOURCE IS_PLL2

/**
 * \def _PLL2MUL
 * \brief Configure la multiplication de la PLL2
 *
 * La PLL2 sert à fournir une horloge en entrée du prescaler de la PLL1. On peut régler ici le
 * facteur multiplicateur entre la source de la PLL2 (PREDIV2) et le prescaler de la PLL1 (PREDIV1).
 * Les valeurs possibles que peut prendre _PLL2MUL vont de 8 à 20.
 *
 * \note Uniquement disponible si la cible est STM32F10X_CL
 */
#define _PLL2MUL 8

/**
 * \def _PREDIV_2
 * \brief Configure le prescaler en entrée de la PLL2
 *
 * Configure le prescaler en entrée de la PLL2.
 * Les valeurs possibles que peut prendre _PREDIV_2 vont de 1 à 16.
 *
 * \note Uniquement disponible si la cible est STM32F10X_CL
 */
#define _PREDIV_2 5
#endif /* STM32F10X_CL */

/**
 * \def _USB_PRE
 * \brief Configure le prescaler de l'USB.
 *
 * Configure le prescaler en entrée l'horloge USB.
 * Les valeurs possibles que peut prendre _USB_PRE sont:
 * - 1 ou _USB_PRE_1_5 sur STM32F103
 * - 2 ou 3 sur STM32F107.
 */
#ifdef STM32F10X_CL
	#define _USB_PRE 3
#else
	#define _USB_PRE 1
#endif

/**
 * \def _PLLMUL
 * \brief Configure la multiplication de la PLL
 *
 * La PLL sert à fournir l'horloge SYSCLK. On peut régler ici le facteur multiplicateur
 * entre la source de la PLL (HSE ou HSI) et SYSCLK fourni par la PLL.
 * Les valeurs possibles que peut prendre _PLLMUL vont:
 * - De 2 à 16 pour STM32F10X_MD
 * - De 4 à 9, ainsi que _PLLMUL_6_5 pour STM32F10X_CL
 *
 * \attention SYSCLK ne peut pas dépasser 72 Mhz.
 */
#define _PLLMUL 9

/**
 * \def _HPRE
 * \brief Configure le prescaler pour AHB
 *
 * Configure le prescaler entre SYSCLK et HCLK.
 * Les valeurs possibles que peut prendre _HPRE sont {1,2,4,8,16,64,128,256,512}.
 * \attention Une division de 32 n'est pas possible.
 */
#define _HPRE	1

/**
 * \def _PPRE1
 * \brief Configure le prescaler pour APB1
 *
 * Configure le prescaler entre HCLK et PCLK1 (bus APB1, low speed).
 * Les valeurs possibles que peut prendre _PPRE1 sont {1,2,4,8,16}.
 */
#define _PPRE1	2 

/**
 * \def _PPRE2
 * \brief Configure le prescaler pour APB2
 *
 * Configure le prescaler entre HCLK et PCLK2 (bus APB2, high speed).
 * Les valeurs possibles que peut prendre _PPRE2 sont {1,2,4,8,16}.
 */
#define _PPRE2	1 

/**
 * \def _ADCPRE
 * \brief Configure le prescaler pour l'horloge ADC
 *
 * Configure le prescaler entre HCLK et ADCCLK (bus ADC).
 * Les valeurs possibles que peut prendre _PPRE2 sont {2,4,6,8}.
 *
 *\attention ADCCLK ne peut pas dépasser 14 Mhz.
 */
#define _ADCPRE	6

	/**
	 * @}
	 */

#endif /* __CLOCK_CONF_H__ */
