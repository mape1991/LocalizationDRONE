
//=======================================================================================
// 	Auteur T.Rocacher
//  revu Mars 2011
//  Processeur : STM32F103RB
//  Logiciel dev : µVision 4
//  Module qui configure le NVIC et les IT externe
//
//=======================================================================================



#ifndef _NVIC_IT_EXT_H__
#define _NVIC_IT_EXT_H__

void NVIC_Enable_IT(char NumeroIT);
/*
Permet de valider une ligne d'interruption, hors coeur, de 0 à 63. Le numéro 
est passé en paramètre.
*/
void NVIC_Disable_IT(char NumeroIT);
/*
Permet d'invalider une ligne d'interruption, hors coeur, de 0 à 63. Le numéro 
est passé en paramètre.
*/
void NVIC_Prio_IT(char NumeroIT, char Prio);
/*
Fixe la priorité de la ligne d'interruption spécifiée.
*/



char NVIC_Ext_IT (GPIO_TypeDef * Port, char Broche, char Front, char Techno, char Prio, void (*IT_function) (void));
// Definitions :
// Front
#define FALLING_EGDE 0
#define RISING_EDGE 1
#define FALLING_RISING_EDGE 2

/*
place la broche du port indiquée en entree IT sur front up/down/les deux
La fonction est "clé en main" pour configurer l'IT sur une broche externe 
voie 0 à 4, port A, B ou C)
pour les autres voies, 5 à 15, c'est un peu plus complexe.
Retourne 1 si erreur, 0 sinon

Le user spécifie :
 - port
 - numéro de broche
 - front actif qui déclenche l'IT
 - Techno de la broche (flottant, pull down...)
 - Niveau de priorité
 - nom de la fonction à lancer lors de l'IT
 Tout est paramétré, jusqu'au NVIC.  Le flag d'IT est mis à 0 automatiquement

 Exemple de code :
NVIC_Ext_IT (GPIOA, 2, RISING_EDGE, INPUT_FLOATING,8,Cligno);
 La broche GPIOA.2 provoque au front montant une IT qui lance la fct Cligno(), avec
 une priorité de 8. L'entrée est flottante.


 Pour les broches, de 5 à 15, le STM32 n'utilise que 2 vecteurs d'IT.
 C'est pourquoi, contrairement aux broches 0 à 4, c'est au user de chercher
 le flag levé, et de le remettre à 0.
 Ce flag, c'est le bit de même rang que la broche, du registre EXTI->PR

 Exemple, remise à 0 du flag correspondant à la broche 10:
 EXTI->PR = (EXTI->PR)|(0x01<<10); // très surprenant, il faut mettre à 1 pour
 mettre à 0 le flag....c'est comme ça.
*/


#endif
