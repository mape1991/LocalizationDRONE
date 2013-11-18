/*-----------------------------------------------------------------------------------------
 * Name:    usart.c
 * Purpose: USART Drivers management on STM32.
 *
 * Version: V2.0
 * Authors: LASNIER Emeric (INSA Toulouse,France)
 *-----------------------------------------------------------------------------------------
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
 *----------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------
 * LIBRAIRIES
 *-----------------------------------------------------------------------------------------*/
#include "usart.h"
#include "clock.h"
#include "GPIO.h"

/*-----------------------------------------------------------------------------------------
 * FUNCTIONS
 *-----------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------
 * INIT_USART
 *-----------------------------------------------------------------------------------------
 * Configuration, activation et reglage de la liaison USART
 * Paramètres: - USART, pointeur de Type USART_TypeDef
 *						 - Vitesse de transmission
 *						 - Parite de la liaison
 *						 - Nombre de bit Stop
 *						 - Longueur de la donnee 
 * Retour: Erreur si les parametres de la liaison sont incoherents
 *-----------------------------------------------------------------------------------------*/
 
char Init_USART (USART_TypeDef* USART, unsigned int Baud_Rate, char Parity, char Bit_Stop, char data_length) {

		unsigned int CLK_USART=0x0, DIV_MANTISSA=0x0, DIV_FRACTION_I=0x0;
	  float USARTDIV=0x0, DIV_FRACTION_F=0x0;
		
		// Erreur si parametres de la liaison incoherents
	  if (((Parity==EVEN)||(Parity==ODD)||(Parity==DISABLED)) &&
				((Bit_Stop==STOP_BIT_MODE1)||(Bit_Stop==STOP_BIT_MODE2)||(Bit_Stop==STOP_BIT_MODE3)||(Bit_Stop==STOP_BIT_MODE4)) &&
				((data_length==WORD_LENGTH_8BITS)||(data_length==WORD_LENGTH_9BITS))) {
		
			// Activation de l'horloge pour les USARTs
			if (USART == USART1)
				RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
			if (USART == USART2)
				RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
		
      // Demarre USART
			USART->CR1 |= (1<<13);
		
			// Configuration de la taille de la donnee
			USART->CR1 |= (data_length<<12);
		
			// Configuration du bit STOP
			USART->CR2 |= (Bit_Stop<<12);
				
			// Configuration de la parite
			if (Parity == DISABLED) {
					USART->CR1 &= ~USART_CR1_PCE;
			} else {

					USART->CR1 |= USART_CR1_PCE;
			
					if (Parity == EVEN)
						USART->CR1 &= ~ USART_CR1_PS;
					else 
						USART->CR1 |= USART_CR1_PS;
			} 

			// Configuration de la vitesse de transmission 
			// USART_DIV = CLK_USART / 16*BAUD_Rate
			// Mantisse a partir du bit 4
			// Fraction a partir du bit 0 
			
			// Recuperation de l'Horloge 
			if (USART == USART1)
				CLK_USART = CLOCK_GetPCLK2();
			else
				CLK_USART = CLOCK_GetPCLK1();
			
			// Calcul de l'USARTDIV
			USARTDIV = (float) CLK_USART / (float) (16*Baud_Rate);
			
			// Calcul de la mantisse et de la fraction pour le registre BR
			DIV_MANTISSA = USARTDIV;
			DIV_FRACTION_F = USARTDIV - DIV_MANTISSA;
			DIV_FRACTION_I = 16*DIV_FRACTION_F;
			
			// Si Overflow sur la fraction +1 sur la Mantisse
			if (DIV_FRACTION_I > 15) {
					DIV_FRACTION_I = 0;
					DIV_MANTISSA = DIV_MANTISSA + 1;
			}
		
			// On regle la vitesse de transmission dans le registre BR
			USART->BRR = (DIV_MANTISSA<<4) | (DIV_FRACTION_I<<0);
			
			// Pas d'erreur
			return 0;
			
		} else {
			// Erreur 
			return 1;
		}
}


/*-----------------------------------------------------------------------------------------
 * SEND_DATA_USART
 *-----------------------------------------------------------------------------------------
 * Envoie d'une donnee sur la liaison USART
 * Paramètres: - USART, pointeur de Type USART_TypeDef
 *						 - Donnee a envoyer 
 * Retour: Code d'erreur si la donnee est superieur a 9 bits 
 *-----------------------------------------------------------------------------------------*/

char Send_Data_USART(USART_TypeDef* USART, unsigned int data) {
		
		// Test le bit 9 de data, s'il existe, la donne est superieur a 9 bits ... 
		if (data & (1<<9)) {
			// Erreur 
			return 1;
		} else {
	
			// Desactive la reception
			// USART->CR1 &= ~ USART_CR1_RE; 
			// Transmission activee
			USART->CR1 |= USART_CR1_TE;  
	
			// On attend si le registre DR est pret pour la transmission
			// bit (7) TXE du registre USART->SR
			do {
					// Attente ... 
			} while ((USART->SR & (1<<7)) == 0);
			
			// Registre contenant la data to send
			USART->DR = data ;
			
			// pas d'Erreur
			return 0;
	}
}


/*-----------------------------------------------------------------------------------------
 * SEND_STRING_DATA
 *-----------------------------------------------------------------------------------------
 * Configuration, activation et reglage de la frequence du Timer.
 * Paramètres: - USART, pointeur de Type USART_TypeDef
 *						 - Chaine de caracteres a envoyer
 * Retour: Code d'erreur si la chaine est nulle 
 *-----------------------------------------------------------------------------------------*/

char Send_String_USART (USART_TypeDef* USART, char string[]) {

	int i=0;
	
	// Erreur si chaine NULL
	if (string != 0) {
		// Parcours de la chaine tant que pas finie ... 
		while (string[i] != '\0') {
			// Affichage du caractere 
			Send_Data_USART(USART, string[i]);
			i++;
		} 
		// Pas d'erreur 
		return 0;
		
	} else {
	// Erreur
		return 1 ;
	}
}


/*-----------------------------------------------------------------------------------------
 * RECEIVE_DATA_USART
 *-----------------------------------------------------------------------------------------
 * Lecture d'une donnee sur la liaison USART
 * Il faut pas l'utiliser apres une interruption USART
 * Paramètres: - USART, pointeur de Type USART_TypeDef
 * Retour: Renvoie de la donnee lue 
 *-----------------------------------------------------------------------------------------*/

unsigned int Receive_Data_USART(USART_TypeDef *USART) { 
	
		// Reception activee
	  USART->CR1 |= USART_CR1_RE; 

		// On attend si le registre DR est pret pour la reception
    // bit (5) RXNE du registre USART->SR
    do {
				// Attente ... 
    } while ((USART->SR & (1<<5)) == 0) ;
		 
  return USART->DR;
}


/*-----------------------------------------------------------------------------------------
 * DATA_USART
 *-----------------------------------------------------------------------------------------
 * Lecture d'une donnee sur la liaison USART
 * Paramètres: - USART, pointeur de Type USART_TypeDef
 * Retour: Renvoie de la donnee lue 
 *-----------------------------------------------------------------------------------------*/

char Data_USART(USART_TypeDef *USART) 
{ 		 
  return USART->DR;
}


struct _PTR_Func_USART1 PTR_Function_USART1;
struct _PTR_Func_USART2 PTR_Function_USART2;
struct _PTR_Func_USART3 PTR_Function_USART3;

void USART1_IRQHandler (void) {
		
	if (USART1->SR & (1<<5)) 
	{
		// Remise a zero du Flag
		USART1->SR &= ~(1<<5);
		
		// Detection de bruit 
		if ((USART1->SR & (1<<2)) == 0) 
		{
			if (PTR_Function_USART1.PTR_Func_USART1_Received_Data != 0)
				(*PTR_Function_USART1.PTR_Func_USART1_Received_Data) ();
		} 
		else 
		{
			// Lecture du registre DR pour remettre l'interruption a zero 
			USART1->DR = USART1->DR;
		}
	}
}

void USART2_IRQHandler (void) {
		
	if (USART2->SR & (1<<5)) {
		// Remise a zero du Flag
	//	USART2->SR &= ~(1<<5);
		
		// Detection de bruit 
		if ((USART2->SR & (1<<2)) == 0)
					if (PTR_Function_USART2.PTR_Func_USART2_Received_Data != 0)
							(*PTR_Function_USART2.PTR_Func_USART2_Received_Data) ();

		// Lecture du registre DR pour remise a zero 
		USART2->DR = USART2->DR;
	}
}

void USART3_IRQHandler (void) {
		
	if (USART3->SR & (1<<5)) {
		// Remise a zero du Flag
		USART3->SR &= ~(1<<5);
		
		// Detection de bruit 
		if ((USART3->SR & (1<<2)) == 0) {
					if (PTR_Function_USART3.PTR_Func_USART3_Received_Data != 0)
							(*PTR_Function_USART3.PTR_Func_USART3_Received_Data) ();
		} else {
				// Lecture du registre DR pour remise a zero 
				USART3->DR = USART3->DR;
		}
	}
}

/*-----------------------------------------------------------------------------------------
 * ACTIVE_Interrupt_Received_Data_available
 *-----------------------------------------------------------------------------------------
 * Lecture d'une donnee sur la liaison USART par interruption 
 * Paramètres: - USART, pointeur de Type USART_TypeDef
 * Retour: Renvoie de la donnee lue 
 *-----------------------------------------------------------------------------------------*/

void Active_Interrupt_Received_Data_available (USART_TypeDef *USART, char priority, void (*IT_Function) (void)) { 

		// Reception activee
	  USART->CR1 |= USART_CR1_RE; 
	  // Autorisation de l'interruption au niveau USART 
		USART->CR1 |= USART_CR1_RXNEIE;
			
		if (USART == USART1) {
			// Activation au niveau NVIC : Position 37 
			NVIC->ISER[1] |= (1<<5);
			NVIC->IP[37] |= (priority<<4);
			// On pointe vers la fonction a executer en cas d'IT;
			PTR_Function_USART1.PTR_Func_USART1_Received_Data = IT_Function;
		}
		if (USART == USART2) {
			// Activation au niveau NVIC : Position 38
			NVIC->ISER[1] |= (1<<6);
			NVIC->IP[38] |= (priority<<4);
			// On pointe vers la fonction a executer en cas d'IT;
			PTR_Function_USART2.PTR_Func_USART2_Received_Data = IT_Function;
		}
		if (USART == USART3) {
			// Activation au niveau NVIC : Position 39 
			NVIC->ISER[1] |= (1<<7);
			NVIC->IP[39] |= (priority<<4);
			// On pointe vers la fonction a executer en cas d'IT;
			PTR_Function_USART3.PTR_Func_USART3_Received_Data = IT_Function;
		}
}