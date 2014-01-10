/******************************************************************************
* File Name          : GPIO.h
* Author             : T Rocacher
* Version            : revu 7/09/2011
* Date               : 01/03/2011
* Description        : driver pour les GPIO
*                     
*******************************************************************************/

#ifndef _GPIO_H__
#define _GPIO_H__


//=======================================================================================
// Rappel sur les ressources 
// Broche : de 0 à 15 pour GPIOA, GPIOB, GPIOC (STM103)
// Revue en Juin 2011. Préciser la clé de compil STM32F107
// pour avoir accès aux ports D et E du STM107
//=======================================================================================


//______________________________________________________________________________
	   
char GPIO_Configure(GPIO_TypeDef * Port, int Broche, int Sens, int Techno);
//______________________________________________________________________________
/*
La fonction initialise n'importe quelle broche de port (entrée, sortie, techno....)
Exemple :
Port_IO_Init(GPIOB, 8, OUTPUT, OUTPUT_PPULL);
Place le bit 8 du port B en sortie Push-pull
Renvoie 0 si tout est OK,  et 1 s'il y a un problème (plage d'entrée non repsectée)
*/

// Sens
#define INPUT 				'i'
#define OUTPUT 				'o'

// Techno pour pin en entrée (INPUT)
#define ANALOG 				0
#define INPUT_FLOATING 		1
#define INPUT_PULL_DOWN_UP 	2

// Techno pour pin en sortie (OUTPUT)
#define OUTPUT_PPULL 		0
#define OUTPUT_OPDRAIN 		1
#define ALT_PPULL 			2
#define ALT_OPDRAIN 		3

/* Doc tech : STM32F103RM.pdf
 Port : GPIOA à GPIOC (extension D et E pour STM107 , préciser la clé de compil STM32F107 !!!)
 Broche : de 0 à 15 

 */


//______________________________________________________________________________
// Macro
//______________________________________________________________________________
#define GPIO_Set(GPIO,Broche) GPIO->BSRR=(0x01<<Broche)
// Spécifier GPIO (GPIOA..GPIOC)
// Spécifier le numéro de broche (0 à 15)
// exemple : Port_IO_Set(GPIOB,8);

#define GPIO_Clear(GPIO,Broche) GPIO->BRR=(0x01<<Broche)
// Spécifier Port (GPIOA..GPIOC)
// Spécifier le numéro de broche (0 à 15)
// exemple : GPIO_Clear(GPIOB,8);


#define GPIO_Toggle(GPIO,Broche) GPIO->ODR=(GPIO->ODR)^(0x01<<Broche)
// Spécifier Port (GPIOA..GPIOC)
// Spécifier le numéro de broche (0 à 15)

#define GPIO_Write(GPIO,Broche,Val) GPIO->BRR=(0x01<<Broche); \
                                    GPIO->ODR|=(Val<<Broche)
// Spécifier Port (GPIOA..GPIOC)
// Spécifier le numéro de broche (0 à 15) 
// Cette macro est équivalente à GPIO_Clear et GPIO_Set, mais 
// dans ce cas, c'est la variable val, suivant
// qu'elle est à 1 ou à 0, qui provoquera le clear ou set. Cette
// macro prend tout sons sens lors d'une test bianire et d'une affectation
// en suivant sur une broche de port (pas besoin d'opérer un test pour ensuite
// faire explicitement un set ou un clear).

#define GPIO_Read(GPIO,Broche) ((GPIO->IDR)&(0x01<<Broche))
// Spécifier Port (GPIOA..GPIOC)
// Spécifier le numéro de broche (0 à 15) 
// Port_IO_Lire (GPIO,Broche) est un entier 16 bits	 qui vaut
// 0x0000 si la broche est à 0,  0x01<<Broche sinon
// Cette macro est utilisée lors d'un test de mise à 1 ou à 0
// d'une broche, évidemment configurée en entrée.

#define GPIO_Write_Multi_bits(GPIO,Masque32Bits) (GPIO->BSRR)=Masque32Bits
// Spécifier Port (GPIOA..GPIOC)
// Préciser le masque comme suit :
// Masque 32 bits
// | 16 bits Reset | 16 bits Set |
// Chacun des 16 bits est dans l'ordre des broches du GPIO : Br15 ... Br0.
//
// Exemple :
// on veut mettre à 1 le bit 15 et mettre le bit 6 à 0, la valeur du masque sera :
// Masque32Bits = b 0000 0000 0010 0000   1000 0000 0000 0000 = 0x00208000


//___________________________FIN ___________________________________________________
// suite à ignorer.

/* Retro-compatibilité avec les noms de fonction de Thierry */
#define Port_IO_Init	GPIO_Configure
#define Port_IO_Set 	GPIO_Set 
#define Port_IO_Clr 	GPIO_Clear
#define Port_IO_Toggle 	GPIO_Toggle
#define Port_IO_Lire 	GPIO_Read

#endif /* _GPIO_H__ */
