/**
 * \file
 * \brief Pilote d'horloge pour STM32
 * \author Pascal ACCO (INSA - Toulouse,France)
 * \author Sebastien DI MERCURIO (INSA - Toulouse,France)
 * \version 1.2
 * \date 17/02/2012
 *
 * Ce pilote fait partie des pilotes pour STM32 de l'INSA de Toulouse, France.
 * Merci de consulter le fichier LICENCE.txt au sujet des conditions de redistribution,
 * modification et copyright concernant ce pilote. Le fichier LICENCE.txt doit faire partie
 * des fichiers fournis avec ce pilote.
 *
 * Copyright (c) 2012, INSA-GEI Toulouse, France.
 * Tout droits réservés.
 */

#ifndef _CLOCK_H__
#define _CLOCK_H__

/**
 * \page clock CLOCK
 * \section CLOCK_intro Présentation
 *
 * Le pilote CLOCK fournit le support pour la configuration et le contrôle des horloges du MCU.
 *
 * \section CLOCK_dep Dépendances
 * Aucune dépendance à un autre pilote
 *
 * \section CLOCK_compinfo Clefs de compilation
 * Les clefs de compilation suivantes peuvent être utilisées:
 *  - STM32F10X_CL : Configure les horloges pour un micro STM32F107
 *  - STM32F10X_MD : Configure les horloges pour un micro STM32F103
 *
 * \section CLOCK_reference Liste des API relatif à ce pilote
 *  - \ref CLOCK_driver
 *
 * \section CLOCK_exemples Exemples d'utilisation
 *  Exemple 1: Initialise les horloges du micro tel que configuré dans le fichier clock_conf.h
 *
 *  \code
 *  #include "clock.h"
 *
 *  void main (void)
 *  {
 *  	CLOCK_Configure();
 *  }
 *
 *  \endcode
 */

/**
 * @addtogroup CLOCK_driver CLOCK
 * @{
 */

	/** @addtogroup CLOCK_constantes Constantes
	 * @{
	 */

	/**
	 * @}
	 */

	/** @addtogroup CLOCK_defines Defines
	 * @{
	 */

	/**
	 * @}
	 */

	/** @addtogroup CLOCK_fonctions Fonctions
	 * @{
	 */

/**
 * \brief Initialise les horloges
 *
 * Règle le système d'horloge RCC du micro, conformément aux réglages configurés dans le fichier drivers_conf.h
 */
void CLOCK_Configure(void);

/**
 * \brief Fourni l'horloge HCLK
 *
 * Retourne la fréquence, en Hz, de l'horloge HCLK, en sortie du diviseur AHB (aussi fréquence CPU)
 * \return Fréquence en Hz de l'horloge HCLK
 */
u32 CLOCK_GetHCLK(void);

/**
 * \brief Fourni l'horloge PCLK1
 *
 * Retourne la fréquence, en Hz, de l'horloge PCLK1, en sortie du diviseur APB1 (périphériques lents)
 * \return Fréquence en Hz de l'horloge PCLK1
 */
u32 CLOCK_GetPCLK1(void);

/**
 * \brief Fourni l'horloge PCLK2
 *
 * Retourne la fréquence, en Hz, de l'horloge PCLK2, en sortie du diviseur APB2 (périphériques rapides)
 * \return Fréquence en Hz de l'horloge PCLK2
 */
u32 CLOCK_GetPCLK2(void);

/**
 * \brief Fourni l'horloge Timer
 *
 * Retourne la fréquence, en Hz, du timer indiqué en paramètre
 * \param[in] timer Pointeur sur la structure de registre du timer
 * \return Fréquence en Hz du timer indiqué en paramètre.
 */
u32 CLOCK_GetTIMCLK (void *timer);

/**
 * \brief Fourni l'horloge ADC
 *
 * Retourne la fréquence, en Hz, de l'horloge ADC, après le diviseur ADC
 * \return Fréquence en Hz de l'ADC
 */
u32 CLOCK_GetADCCLK (void);

	/**
	 * @}
	 */

	/** @addtogroup CLOCK_macros Macro-fonctions
	 * @{
	 */

	/**
	 * @}
	 */

/**
 * @}
 */

#endif /* _CLOCK_H__ */
