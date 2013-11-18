/**
 * \file usart.h
 * \brief USART Drivers management on STM32.
 * \author LASNIER Emeric (INSA Toulouse, FRANCE)
 * \version 2.0
 * \date 07/10/2012
 *
 * This driver allow us to manage USARTs transmission for STM32 Chip especially :
 *      . USART communication configuration
 *      . Sending data :
 *          - Sending one Byte
 *          - Sending String
 *      . Receiving data
 *      . Enable USART Interrupt on data received :
 *          - Data available for reading (Version 2.0)
 *
 * Copyright (c) 2012, All rights reserved.
 */

#ifndef _V_USART
#define _V_USART

#include "stm32f10x.h"

#define EVEN 0x0            		/*!< Parite Paire */
#define ODD 0x1									/*!< Parite Impaire */
#define DISABLED 0x2						/*!< Parite Desactivee */

#define STOP_BIT_MODE1 0x0  		/*!< 1   Bit Stop */ 
#define STOP_BIT_MODE2 0x1  		/*!< 0.5 Bit Stop */
#define STOP_BIT_MODE3 0x2  		/*!< 2   Bit Stop */
#define STOP_BIT_MODE4 0x3  		/*!< 1.5 Bit Stop */

#define WORD_LENGTH_8BITS 0x0   /*!< Taille de la donnee : 8 bits */
#define WORD_LENGTH_9BITS 0x1   /*!< Taille de la donnee : 9 bits */


struct _PTR_Func_USART1 {
	
	// Reception ok
	void (*PTR_Func_USART1_Received_Data) (void);

} typedef _PTR_Function_USART1;

struct _PTR_Func_USART2 {
	
	// Reception ok
	void (*PTR_Func_USART2_Received_Data) (void);

} typedef _PTR_Function_USART2;

struct _PTR_Func_USART3 {
	
	// Reception ok
	void (*PTR_Func_USART3_Received_Data) (void);

} typedef _PTR_Function_USART3;

/**
 * \fn void Init_USART (USART_TypeDef *USART, unsigned int Baud_Rate, char Parity, char Bit_Stop, char data_length)
 * \brief Fonction qui configure la liaison USART
 *
 * Configure la liaison série à travers différent paramètres :
 *  . Vitesse de transmission souhaitée. Calcul effectué et configuration du registre BRR.
 *  . La parité de la liaison peut être :
 *      - Desactivée (Voir constante associée DISABLED).
 *      - Paire (Voir constante associée EVEN).
 *      - Impaire (Voir constante associée ODD).
 *  . Le nombre de bit Stop : 0,5, 1, 1.5 et 2 (Voir constantes associées STOP_BIT_MODEx).
 *  . La taille de la donnée à émettre : 8 ou 9 bits (Voir constantes asscociées WORD_LENGTH_xBits)
 * Configure la broche PA9 en Output : Tx et active l'émetteur.
 * Configure la broche PA10 en Input : Rx et active le récepteur.
 * Retourne un code d'erreur si :
 *	. La parite ne fait pas partie des trois choix.
 *	. Le nombre de bit stop n'existe pas.
 *	. La taille de la donnee a emettre n'est pas valable.
 * 1 si Erreur, 0 Sinon.
 *
 * \param USART pointeur de type USART, ne peut être NULL.
 * \param Baud_Rate Vitesse de transmisson de la liaison (Bit/s).
 * \param Parity Parité de la liaison.
 * \param Bit_Stop Nombre de Bit Stop de la liaison.
 * \param data_length Taille du message à envoyer.
 * \return Code d'erreur.
 */

char Init_USART (USART_TypeDef *USART, unsigned int Baud_Rate, char Parity, char Bit_Stop, char data_length);


/**
 * \fn char Send_Data_USART(USART_TypeDef *USART, char data)
 * \brief Fonction qui permet d'envoyer une donnée
 *
 * Attends que le registre UDT soit disponible avant de pouvoir écrire dedans.
 * Retourne une Erreur si la donnee a envoyer depasse 9 bits.
 * 1 si Erreur, 0 Sinon.
 *
 * \param USART pointeur de type USART, ne peut être NULL.
 * \param data Donnée à émettre.
 * \return Code d'erreur.
 */

char Send_Data_USART(USART_TypeDef *USART, unsigned int data);


/**
 * \fn unsigned int Receive_Data_USART(USART_TypeDef *USART)
 * \brief Fonction qui permet de lire une donnée
 *
 * Attends que le registre UDR soit disponible avant de venir lire la donnée.
 *
 * \param USART pointeur de type USART, ne peut être NULL.
 * \return Donnée lue.
 */

unsigned int Receive_Data_USART(USART_TypeDef *USART);

/**
 * \fn Data_USART(USART_TypeDef *USART) 
 * \brief Fonction qui permet d'envoyer le charactere recu
 *
 * Lecture d'une donnee sur la liaison USART
 * 
 * \param USART pointeur de type USART, ne peut être NULL.
 * \param string Chaine de caractère à afficher.
 * \return char charactere recu.
 */
 
char Data_USART(USART_TypeDef *USART); 

/**
 * \fn Send_String_USART (USART_TypeDef* USART, char string[])
 * \brief Fonction qui permet d'envoyer une chaine de caractère
 *
 * Attends que le registre UDR soit disponible avant de pouvoir écrire la donnée.
 * Utilisation en boucle de la Send_Data_USART
 * Code d'erreur si la String est NULL
 *
 * \param USART pointeur de type USART, ne peut être NULL.
 * \param string Chaine de caractère à afficher.
 * \return Code d'erreur.
 */

char Send_String_USART (USART_TypeDef* USART, char string[]);


void Active_Interrupt_Received_Data_available (USART_TypeDef *USART, char priority, void (*IT_Function) (void)) ;


#endif