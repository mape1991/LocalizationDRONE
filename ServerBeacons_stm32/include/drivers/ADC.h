#include "stm_regs.h" 
/******************************************************************************
* File Name          : ADC.h
* Author             : T Rocacher
* Version            : revu 7/09/2011
* Date               : 01/03/2011
* Description        : Module qui configure les ADC 12 bits
*                     
*******************************************************************************/


#ifndef _ADC_H__
#define _ADC_H__

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



// alignement des data, quelque soit le mode
#define ALIGN_DROITE_16_0(ADC) ADC->CR2=ADC->CR2&~(ADC_ALIGN)
#define ALIGN_GAUCHE_1_15(ADC) ADC->CR2=ADC->CR2|(ADC_ALIGN)


//=======================================================================================
// ADC en mode basique, single conversion
//=======================================================================================

//_______________________________________________________________________________

float Init_ADC_Single_Conv(ADC_TypeDef * ADC, float Duree_Ech_us, char Voie_ADC);
//_______________________________________________________________________________
/*
Initialise l'ADC en mode single conversion, 
une seule voie. Le temps passé en paramètre est la durée d'échantillonnage. 
La fonction renvoie un float qui est la durée exacte, immédiatement  
supérieure à celle spécifiée (dépend du choix des prescaler).
L'ADC est on, prêt à fonctionner. Tous les canaux sont paramétrés 
avec la même durée d'échantillonnage.

ADC_TypeDef *ADC : ADC1 ou ADC2
Float Duree_Ech_us: Durée d'échantillonnage exprimée en us.
Char Voie_ADC: Précise la voie de l'ADC	 

Exemple :
Init_ADC_Single_Conv(ADC1, 0.0,3);
//L'ADC1 est réglé avec un temps d'acquisition nul, et orienté sur la voie 3.
*/
//_______________________________________________________________________________



//_______________________________________________________________________________

void Lance_ADC(ADC_TypeDef * ADC, char Voie_ADC);
//_______________________________________________________________________________
/* 
Lance l'ADC tout simplement sur la voie spécifiée
Exemple :
Lance_ADC(ADC1, 3);	Lance une conversion de l'ADC1 sur la voie 3. Le résultat
est stockée dans un registre interne
*/
//_______________________________________________________________________________





//_______________________________________________________________________________

vu16 Scrute_Fin_Conv_ADC(ADC_TypeDef * ADC);
//_______________________________________________________________________________
/*
Attend que la conversion se finisse en scrutation (EOC) et renvoie la donnée
Exemple :
Lance_ADC(ADC1, 3);
Resultat = Scrute_Fin_Conv_ADC(ADC1);
// La variable Resultat contiendra la valeur de l'ADC à la fin de la conversion

*/
//_______________________________________________________________________________




//_______________________________________________________________________________

vu16 Acquiert_ADC_Single_Conv(ADC_TypeDef * ADC, char Voie_ADC);
//_______________________________________________________________________________
/*
Lance l'ADC, pour une conversion. Attend que la conversion se finisse 
en scrutation. Associe en fait les deux fonctions précédentes
Exemple :
Resultat = Acquiert_ADC_Single_Conv(ADC1, 3);
// La variable Resultat contiendra la valeur de l'ADC à la fin de la conversion	du
canal 3
*/
//_______________________________________________________________________________






//_______________________________________________________________________________

#define Res_ADC(ADC)  ADC->DR;
/*
permet de lire le registre de donnée
*/
//_______________________________________________________________________________







//=======================================================================================
// ADC en interruption
//=======================================================================================

//_______________________________________________________________________________

void Configure_IT_ADC_EOC(ADC_TypeDef * ADC, char Prio, void (*IT_function) (void));
//_______________________________________________________________________________


/* Configure l'ADC de manière à rentrer en interruption lors de 
la fin de conversion. Le nom de la fonction est passé en paramètre 
ainsi que la priorité d'interruption associée.
Voir fonction  Init_ADC_Trig_Timer pour les options de compilation de cette fonction.

Exemple :
Configure_IT_ADC_EOC(ADC1, 1, IT_ADC);
// Provoque une interruption de priorité 1, dès que l'ADC a terminé sa conversion.
La fonction IT_ADC est alors lancée.
*/
//_______________________________________________________________________________


//=======================================================================================
// ADC en démarrage sur débordement timer (lancement automatique, matérielle)
// Nécessite dont un Timer associé pour trigger l'ADC.
//=======================================================================================


//_______________________________________________________________________________

#define  TIMER_ADC_EN_MODE_TOGGLE
/*
Option de compilation
- si défini, alors le trig de l'ADC se fera en mode toggle (un cycle sur deux
du timer déclenche l'IT via OC1ref)
- sinon, le trig se fait à chaque cycle timer, mais alors dans l'IT ADC, OC1ref
est remis à 0 (géré par soft ds le handler ADC, le user n'a pas à s'en soucier)
*/
//_______________________________________________________________________________


//_______________________________________________________________________________

char Init_ADC_Trig_Timer(ADC_TypeDef * ADC, float Duree_Ech_us,char Source, char Voie_ADC);
//_______________________________________________________________________________

/*
Configure l'ADC de manière à ce qu'il soit déclenché par un timer directement. 
Les sources sont multiples et limitées à celles proposées dans la liste (ci-dessous).
Cette fonction appelle  Init_ADC_Single_Conv.

Deux options possibles  (option de compilation):
- le timer active sa sortie Ocref en toggle : chaque débordement inverse 
la sortie et donc l'ADC est lancé un débordement sur deux. Pour cela définir 
TIMER_ADC_EN_MODE_TOGGLE 
- si cette definition est mise en commentaire le timer fait passer Ocref à 1, 
et c'est dans l'interruption ADC que Ocref est mis à 0. Cela prend un peu de 
tps dans l'IT ADC.
NB: peut sans doute être amélioré en plaçant Ocref en PWM....
a voir pour une prochaine mise à jour.

Liste des sources
  - Sources de trig possible :
      -  0  TIM1_CC1 event
      -  1  TIM1_CC2 event
      -  2  TIM1_CC3 event
      -  3  TIM2_CC2 event
      -  5  TIM4_CC4 event
   NB, d'autres sources sont prévu par le constructeur, mais non
   utilisés dans cette routine
*/

// Les sources de déclenchement :
#define TIM1_CC1 0
#define TIM1_CC2 1
#define TIM1_CC3 2
#define TIM2_CC2 3
#define TIM4_CC4 5

//_______________________________________________________________________________





//=======================================================================================
// ADC en DMA
//=======================================================================================

float Init_ADC1_DMA(char Nb_Canaux, char Seq_Canaux[], float Duree_Ech_us);
/*
- Nb_Canaux : de 1 à 16, c'est le nombre de canaux scannés
- Seq_Canaux[] Table de séquence qui doit avoir la taille Nb_Canaux. Rlle contient
l'ordre d'échantillonnage des diverses voies
- Seq_Canaux[0] = 0 à 15 c'est le rang du scan pour la voie 0
- Seq_Canaux[n] = 0 à 15 c'est le rang du scan pour la voie n
- T_samp_us : Temps de fermeture de l'IT (echantillonnage de la capa ADC) en µs
- Freq_ADC_kHz : fréquence d'attaque de l'ADC en MHz, maxi 14 MHz
- La fonction renvoie la valeur recalculée de la période d'échantillonnage Te en us
*/

vu16 ADC_Lire_DMA(int Voie);

/*
- Voie de 0 à 15 numéro de la voie ADC à lire
- Attention Voie n'est pas la voie ADC, c'est le rang de la table DMA.
Quelque soit les canaux ADC, si par exemple on en choisit 6, si on veut lire la première valeur samplée, on indiquera Voie = 0, pour la dernière Voie = 5.

Exemple d'appli user :
Seq_Canaux[3]={1,6,4}; // 1-6-4-1-6-4-1-6-4-....
Init_ADC1_DMA( 3,Seq_Canaux,2.0);
val = ADC_Lire_DMA[1] ; (canal 6 de l'ADC1)
*/


#endif
