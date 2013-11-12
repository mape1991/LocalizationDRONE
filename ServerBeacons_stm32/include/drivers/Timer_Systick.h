
//=======================================================================================

// 	Auteur T.Rocacher
//  revu Mars 2011
//  Processeur : STM32F103RB
//  Logiciel dev : µVision 4
//  Module qui configure le timer du coeur, le Systick
//
//=======================================================================================



#ifndef _TIMER_SYSTICK_H__
#define _TIMER_SYSTICK_H__




//========================================================================================
// Gestion du Timer SysTick	 Pas de remise à 0 de flag à gérer ds l'IT
//=======================================================================================
// handler flag auto
#define  SysTick_On ((SysTick->CTRL)=(SysTick->CTRL)|1<<0)
#define  SysTick_Off ((SysTick->CTRL)=(SysTick->CTRL)& ~(1<<0))
#define  SysTick_Enable_IT ((SysTick->CTRL)=(SysTick->CTRL)|1<<1)
#define  SysTick_Disable_IT ((SysTick->CTRL)=(SysTick->CTRL)& ~(1<<1))


void Systick_Prio_IT(char Prio,void (*Systick_function) (void));
/*
Fixer la priorité de l'IT coeur Systick, plus elle est basse, plus la prio est importante
Le second paramètre est le nom de la fonction à lancer lors de l'interruption
*/



float Systick_Period(float Duree_us);
/*
 Fixer la periode du Systick et renvoie de la valeur effective
 La fonction fixe le prescaler Systick (ds le clock tree) à 1/8
 L'init des horloges doit donc être lancée AVANT cette fonction.

 Ex: si FreqSys = 40MHz, la durée maximum est 3,355s
 Dans le cas contraire, le fonction renvoie 0.0
*/


// Exemple de code :
// Gestion du Systick, ici, configuration en IT périodique 
// de 0,5s
/*
duree=Systick_Period(500000.0);
Systick_Prio_IT(2,cligno); 
SysTick_On;
SysTick_Enable_IT;
*/

#endif
