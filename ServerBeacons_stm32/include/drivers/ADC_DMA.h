//#include "stm_regs.h" 
#include "stm32f10x.h" // seule modif 2012 (passage en 4.53)
/******************************************************************************
* File Name          : ADC_DMA.h
* Author             : T Rocacher
* Version            : 0
* Date               : 4 /11 / 2011
* Description        : Module qui configure les ADC 12 bits avec la DMA
*                     
*******************************************************************************/

//NB: A chaque demande (ADon), 
//on a une ou plusiers conversion(s) qui se déclenche(nt) en rafale (ça dépend du nb de canaux
// configurés en mode régular). 
// A chaque échantillon, EOC se déclenche.
// Par exemple, si l'ADC est configuré pour 3 voies à traiter, alors
// une mise à 1 de ADon provoquera 3 conversions successives au plus rapide
// que peu faire l'ADC, et donc 3 passage à 1 de EOC vont se faire

#ifndef _ADC_DMA_H__
#define _ADC_DMA_H__

//=======================================================================================
// Rappel sur les ressources 
//
//  - 2ADC 16 canaux, 12 bits
//  - Les broches :
//		- ADC_In0  PA0
//		- ADC_In1	PA1
//		- ADC_In2	PA2
//		- ADC_In3	PA3
//		- ADC_In4	PA4
//		- ADC_In5 	PA5
//		- ADC_In6	PA6
//		- ADC_In7	PA7
//		- ADC_In8	PB0
//		- ADC_In9	PB1
//		- ADC_In10	PC0
//		- ADC_In11	PC1
//		- ADC_In12	PC2
//		- ADC_In13	PC3
//		- ADC_In14	PC4
//		- ADC_In15	PC5
//
// 	-  ADCck Maxi = 14MHz
//	
//=======================================================================================

//=======================================================================================
// Fonction de configuration incontournable
//=======================================================================================

float Init_TimingADC_ActiveADC( ADC_TypeDef * ADC, float Duree_Ech_us );
// Renvoie la durée réelle de la durée de conversion totale (Tacq+Tconv)
// ADC1 ou ADC2

//=======================================================================================
// Fonctions de configuration Single / Multiple Conversion Regular
//=======================================================================================
void Single_Channel_ADC(ADC_TypeDef * ADC, char Voie_ADC);
void Init_MultiChan_Regular( ADC_TypeDef * ADC, char Nb_Canaux, char Seq_Canaux[]);
// Seq_Canaux[] est une table de longueur Nb_Canaux. Elle contient la séquence
// d'échantillonnage voulue.
// Exemple Seq_Canaux[4]={1,0,4,4};
// provoquera les conversions successives des channel 1,0,4,4.

//=======================================================================================
// Fonctions de configuration Manuel / periodique
//=======================================================================================
void Init_Conversion_On_Trig_Timer(ADC_TypeDef * ADC , char Source, float Freq_KHz);
// l'ADC est lancé matériellement par un timer
// Le timer est lancé (en PWM 50% interne au uC)
// Les sources de déclenchement :
#define TIM1_CC1 0
#define TIM1_CC2 1
#define TIM1_CC3 2
#define TIM2_CC2 3
#define TIM4_CC4 5


//=======================================================================================
// Fonctions de configuration en interruption
//=======================================================================================
void Init_IT_ADC_EOC(ADC_TypeDef * ADC, char Prio, void (*IT_function) (void));
//Configure l'ADC de manière à rentrer en interruption lors de la fin de conversion. 
//Le nom de la fonction est passé en paramètre ainsi que la priorité d'interruption associée.
//Exemple :
//Init_IT_ADC_EOC(ADC1, 1, IT_ADC);
// Provoque une interruption de priorité 1, dès que l'ADC a terminé sa conversion.
//La fonction IT_ADC est alors lancée.

void Init_IT_End_Of_DMA1(char Prio, void (*IT_function) (void));
//Configure la DMA channel 1 de manière à rentrer en interruption lors de la fin de transfert DMA
//Le nom de la fonction est passé en paramètre ainsi que la priorité d'interruption associée.
//Exemple :
//Init_IT_End_Of_DMA1(1, IT_DMA);
// Provoque une interruption de priorité 1, dès que la DMA est terminée.
//La fonction IT_DMA est alors lancée.


//=======================================================================================
// Fonctions de configuration en DMA
//=======================================================================================
void Init_ADC1_DMA1(char Circ, vu16 *Ptr_Table_DMA);
// Prépare l'ADC pour rentrer en DMA. La canal est DMA1. Il est associé à ADC1.
// La DMA transferre des mots 16 bits
//Circ = 1 reconfigure la DMA. Une nouvelle bufferisation reprend.







//=======================================================================================
// Fonctions et Macros d'utilisation lancement manuel ADC , DMA
//=======================================================================================
#define Start_ADC_Conversion(ADC) ADC->CR2=	(ADC->CR2)|0x01;
void Start_DMA1(u16 NbEchDMA);
// Lance une DMA sur le nombre de points spécifié. La zone de RAM écrite est indiquée 
// lors de l'appel de la fonction  Init_ADC1_DMA1
#define  Stop_DMA1 DMA1_Channel1->CCR =(DMA1_Channel1->CCR) &~0x1;
// Bloque le périph DMA. Normalement peu utile. 

//=======================================================================================
// Fonctions d'utilisation scrutation
//=======================================================================================
void Wait_On_EOC_ADC ( ADC_TypeDef * ADC);
// Fonction bloquante  qui scrute  l'indicateur EOC de l'ADC
void Wait_On_End_Of_DMA1(void);
// Fonction bloquante  qui scrute  l'indicateur TCIF1 de la DMA

//=======================================================================================
// Fonctions et Macros d'utilisation lecture de donnée convertie
//=======================================================================================
#define Read_ADC(ADC) (ADC->DR)


//=======================================================================================
// Divers Define
//=======================================================================================

// alignement des data, quelque soit le mode
#define ALIGN_DROITE_16_0(ADC) ADC->CR2=ADC->CR2&~(ADC_ALIGN)
#define ALIGN_GAUCHE_1_15(ADC) ADC->CR2=ADC->CR2|(ADC_ALIGN)

/* Test réel effectué, Trigger sur Timer, et DMA
Duree_Reelle=Init_TimingADC_ActiveADC(ADC1, 1.0 );  	config ADC de base
Single_Channel_ADC(ADC1, 1);					 		single chan, voie 1 (potentiometre)
Init_IT_ADC_EOC(ADC1, 1, IT_ADC);						ADC en IT, inutile mais bon
Init_Conversion_On_Trig_Timer(ADC1 , TIM2_CC2, 0.001);	Conversion en trig sur Timer, 1Hz

Init_ADC1_DMA1(20, 0, TableDMA);					   	DMA activée pour 20 eoc, non circulaire
Start_DMA1(20);											Demarrage DMA
Wait_On_End_Of_DMA1();									Polling fin DMA
pipo=Read_ADC(ADC1);
*/


/*
TEST DE DMA EN SIMULATION
Les lancement ADC sont faits sur IT T3.

void IT_T3 (void)
{
Start_ADC_Conversion(ADC1);
}

main
{
Duree_Reelle=Init_TimingADC_ActiveADC(ADC1, 1.0 );
Single_Channel_ADC(ADC1, Voie);
Timer_1234_Init(TIM3, 10.0);
Active_IT_Debordement_Timer(TIM3, 1, IT_T3);
Init_ADC1_DMA1(0, Ptr_Tab_DMA);
}
*/

#endif
