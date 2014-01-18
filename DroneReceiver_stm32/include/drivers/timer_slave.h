#ifndef __TIMER_SLAVE_H
#define __TIMER__SLAVE_H

#include "stm32f10x.h"


/* Permet de configur� un timer comme �tatn prescal� par un autre timer ;
   le timer ne s'incr�mentera que sur d�bordement d'un autre timer
	timer : le timer � prescaler
	control_timer : le timer de contr�le, TIM1 � 4, diff�rent du pr�c�dent
*/
void TIMER_Slave_Mode_UEV(TIM_TypeDef *timer, TIM_TypeDef *control_timer);

/*Permet d'indiquer la valeur max que peut prendre le timer*/
void TIMER_set_max(TIM_TypeDef *timer, int value);

/*Permet de passet un timer en mode onepulse*/
void TIMER_onepulse(TIM_TypeDef *timer);

#endif 

