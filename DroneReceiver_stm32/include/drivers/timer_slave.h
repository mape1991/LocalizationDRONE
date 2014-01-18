#ifndef __TIMER_SLAVE_H
#define __TIMER__SLAVE_H

#include "stm32f10x.h"


/* Permet de configuré un timer comme étatn prescalé par un autre timer ;
   le timer ne s'incrémentera que sur débordement d'un autre timer
	timer : le timer à prescaler
	control_timer : le timer de contrôle, TIM1 à 4, différent du précédent
*/
void TIMER_Slave_Mode_UEV(TIM_TypeDef *timer, TIM_TypeDef *control_timer);

/*Permet d'indiquer la valeur max que peut prendre le timer*/
void TIMER_set_max(TIM_TypeDef *timer, int value);

/*Permet de passet un timer en mode onepulse*/
void TIMER_onepulse(TIM_TypeDef *timer);

#endif 

