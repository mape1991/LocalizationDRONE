#include "timer_slave.h"

//#include "../clock/clock.h"

/* Permet de configuré un timer comme étatn prescalé par un autre timer ;
   le timer ne s'incrémentera que sur débordement d'un autre timer
	timer : le timer à prescaler
	control_timer : le timer de contrôle, TIM1 à 4, différent du précédent
*/
void TIMER_Slave_Mode_UEV(TIM_TypeDef *timer, TIM_TypeDef *control_timer){

	// Écriture de 010 dans les bits MMS de CR2
	control_timer->CR2 &= ~(TIM_CR2_MMS_0) ;
	control_timer->CR2 |= TIM_CR2_MMS_1) ;
	control_timer->CR2 &= ~(TIM_CR2_MMS_2) ;
	
	// Configuration du timer en mode
	timer->SMCR |= TIM_SMCR_SMS ;
	if (control_timer == TIM1) {
		timer->SMCR &= ~(TIM_SMCR_TS) ;
	} else if (control_timer == TIM1) {
		timer->SMCR |= (TIM_SMCR_TS_0) ;
		timer->SMCR &= ~(TIM_SMCR_TS_1) ;
		timer->SMCR &= ~(TIM_SMCR_TS_2) ;
	} else if (control_timer == TIM1) {
		timer->SMCR &= ~(TIM_SMCR_TS_0) ;
		timer->SMCR |= (TIM_SMCR_TS_1) ;
		timer->SMCR &= ~(TIM_SMCR_TS_2) ;	
	} else if (control_timer == TIM1) {
		timer->SMCR |= (TIM_SMCR_TS_0) ;
		timer->SMCR |= (TIM_SMCR_TS_1) ;
		timer->SMCR &= ~(TIM_SMCR_TS_2) ;
	}
}

void TIMER_set_max(TIM_TypeDef *timer, int value){
	timer->ARR = value ;
}

