 //#include "stm_regs.h"
#include "stm32f10x.h" // seule modif 2012 (passage en 4.53)
/******************************************************************************
* File Name          : TIMER_1234.h
* Author             : T Rocacher
* Version            : revu 7/09/2011
* Date               : 01/03/2011
* Description        : Module qui configure les TIMER 1,2,3 et 4
*                     
*******************************************************************************/



#ifndef _TIMER_1234_H__
#define _TIMER_1234_H__


//=======================================================================================
// Rappel sur les ressources 
//
//
//  3 Timer "general Purpose", TIM2, TIM3 et TIM4 + TIM1
//  Chacun d'entre eux dispose de 4 voies de sorties numérotées de 1 à 4
//  Mapping des IO:
//  	TIM1_CH1 - PA08       TM2_CH1_ETR - PA0 TM3_CH1 - PA6		TIM4_CH1 - PB6	
//    	TIM1_CH1 - PA09       TM2_CH2 - PA1		TM3_CH2 - PA7		TIM4_CH2 - PB7
// 		TIM1_CH1 - PA10       TM2_CH3 - PA2		TM3_CH3 - PB0		TIM4_CH3 - PB8
// 		TIM1_CH4 - PA11		  TM2_CH4 - PA3		TM3_CH4 - PB1		TIM4_CH4 - PB9
//
//=======================================================================================



//=======================================================================================
// Fonctions basiques des timers
//=======================================================================================

//______________________________________________________________________________

float Timer_1234_Init(TIM_TypeDef *Timer, float Duree_us );
//______________________________________________________________________________

/*
Timer : de TIM1 à TIM4
Duree_us : c'est l'intervalle de temps exprimé en µs entre deux débordements successifs
Frequence_Ck_Timer_Khz : la fréquence qui attaque le prescaler du Timer
Renvoie la durée véritable, au plus proche de la valeur souhaitée
Le prescaler est calculé le plus petit possible pour une précision la plus 
fine possible). Le Timer est lancé en mode down.
NB: Lancer avant la fonction d'initialisétion du système d'hrologe. 
Elle permet de régler toutes les horloges par défaut.

Exemple :
Init_Clock_System(); // lance l'initialisétion du système d'horloge de la puce
Freq_Reel_Khz = 1000.0 / (Timer_1234_Init(TIM2, 10.0 ));   // Lance le timer avec
une périodicité de 10.0 uS. La fréquence correspondante est donc en principe 100 kHz.
Comme la fonction renvoie la valeur réelle en uS, l'inverse donne un résultat en MHz.
D'où la multiplication flottante par 1000.0


*/
//______________________________________________________________________________






//______________________________________________________________________________

void Active_IT_Debordement_Timer( TIM_TypeDef *Timer, char Prio, void (*IT_function) (void));
//______________________________________________________________________________

/*
La fonction initialise le périphérique et le NVIC de manière à générer une interruption 
à chaque débordement du timer précisé.
Le handler est écrit dans la lib (non accessible). Il opére un test sur les flags 
(sauf pour TIM1, vectorisation multiple) pour identifier la source d'IT (déb, capure...),
 puis lance la fonction précisée en paramètre (pointeur de fonction).
Avant de lancer la fonction, le handler rabaisse le flag d'IT pour ne pas 
rerentrer immediatement. Aucune gestion de ce type n'est à faire dans la 
fonction “IT_function”

Exemple :
Active_IT_Debordement_Timer( TIM2, 1, IT_Timer2); // La fonction active une interruption 
lors du débordement du Timer 2. La fonction appelée est IT_Timer2. La priorité associée est	1 
*/

//______________________________________________________________________________
//============== MODIF TR 18 Avril 2012==================
void Active_IT_Compare_Timer( TIM_TypeDef *Timer, char Voie, char Prio, void (*IT_function) (void));
/*
Même principe que la précédente. Permet de lancer une fonction d'IT sur comparaison
du compteur et du CCR (capture compare). Le CCR est donc à régler, grâce à la macro
déjà existante : PWM_Valeur(Timer,Voie) */
//============== FIN MODIF TR 18 Avril 2012==================

//=======================================================================================
// Timers en PWM
//=======================================================================================


vu16 PWM_Init(TIM_TypeDef *Timer, char Voie, float Frequence_PWM_Khz);
/*
Cette fonction initialise la voie spécifiée du timer spécifié en PWM.
La fréquence souhaitée est passée en paramètre.
La fonction renvoie un entier qui correspond à la résolution de la PWM 
pour pouvoir ensuite régler les rapports cycliques (ARR+1)
3 Timer "general Purpose", TIM2, TIM3 et TIM4 + TIM1
Chacun d'entre eux dispose de 4 voies de sorties numérotées de 1 à 4
Mapping des IO, voir début du .h.

!!C'est au user de configurer la sortie correctement, Altenate ppull !
*/

#define PWM_Valeur(Timer,Voie) Timer->CCR##Voie
/*
Permet de fixer  la durée à l'état haut de la PWM, dont indirectement son rapport 
cyclique. La grandeur doit être comprise entre 0 et ARR. Ex:
Reso = PWM_Init (TIM3,2,25.0);
PWM_Valeur(TIM3,2) = Reso /4; // arrondi à gerer
*/

#define CNT(Timer) Timer->CNT
/*
Permet un accès direct au compteur du Timer spécifié sans avoir à connaître 
les registres du STM32
*/
#define ARR(Timer) Timer->ARR
/*
Permet un accès direct à l'autoreload du Timer (modulo du timer) spécifié sans 
avoir à connaître les registres du STM32
*/
#define Clear_Flag_Overflow(Timer) 	Timer->SR = ((Timer->SR)&~0x01)

//============== MODIF TR 18 Avril 2012==================
#define CptUp(Timer) Timer->CR1=Timer->CR1&~(1<<4)
#define CptDown(Timer) Timer->CR1=Timer->CR1|(1<<4)
//============== FIN MODIF TR 18 Avril 2012==================


//=======================================================================================
// Timers en mode incrémental
//=======================================================================================

//  3 Timer "general Purpose", TIM2, TIM3 et TIM4
//  Chacun d'entre eux dispose de 4 voies de sorties numérotées de 1 à 4
//  Mapping des IO:
//  	TM2_CH1_ETR - PA0 	TM3_CH1 - PA6		TIM4_CH1 - PB6	
//    	TM2_CH2 - PA1		TM3_CH2 - PA7		TIM4_CH2 - PB7
//
//   Dans ce mode, le timerx utilise les 2 entrées 1 et 2
//
char Timer_Inc_Init(TIM_TypeDef *Timer, char Resolution);

#define Reso_Demie_Per_Ch1 2
#define Reso_Demie_Per_Ch2 1
#define Reso_Quart_Per 3

/*
Fonction de configuration du timer en question en mode codeur incrémental
Compte les fronts montant et descendant. Selon la configuration désirée, 
la résolution est de ½ période du channel 1, ou  2, ou encore d' ¼ de période 
en comptant sur les deux voies.

!!C'est au user de configurer des canaux  en entrée !!
*/
#define Reset_Timer(Timer) Timer->CNT=0
// remet à 0 le compteur 

#define Bloque_Timer(Timer) Timer->CR1=(Timer->CR1)&~(1<<0)
// bloque le timer (CEN=0)


#define Run_Timer(Timer) Timer->CR1=(Timer->CR1)|(1<<0)
// Lance timer (CEN=1)

//=======================================================================================
// Timers en mode capture
//=======================================================================================


vu16 Capture_Init(TIM_TypeDef *Timer, char Voie, float Largeur_Pulse_Max_us, vu16 Resolution, char Prio);
/*
Cette fonction détermine le prescaler (le pas du compteur) nécessaire au compteur 
considéré  pour mesurer une durée d'impulsion. On précise le maximum de la durée
et la résolution précisée 	correspond à cette durée. La fonction renvoie 
la résolution effective, pour la durée max.
La voie est également passée en paramètre. La mesure repose sur une interruption 
déclenchée sur front montant et sur front descendant. Le sens du front est inversé à 
chaque interruption. La fonction de lecture, ci-après permet de renvoyer la durée.

!!C'est au user de configurer l'IO correspondant  à la voie du timer,  en entrée !!
*/


vu16 Lire_Duree_Pulse(TIM_TypeDef *Timer,int Voie);
/*
Le rôle de la fonction est de retourner le comptage opéré pendant l'impulsion. 
C'est un nombre sans dimension dont la conversion en seconde peut se faire à 
partir de la résolution, et de la largeur d'impulsion maxi passés en paramètres 
de Capture_Init.
*/





#endif
