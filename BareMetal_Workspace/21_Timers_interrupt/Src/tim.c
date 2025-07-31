#include "stm32f446xx.h"


#define TIM2_EN (1U<<0)
#define CR1_EN	(1U<<0)
#define DIER_UIE (1U<<0)
void init_tim2_1hz_interrupt(void);
void init_tim2_1hz(void)
{
	//Enable clock access to timer2
	RCC->APB1ENR |= TIM2_EN;
	// set prescalar value
	TIM2->PSC = 1600-1;
	//set auto-reload value
	TIM2->ARR = 10000-1;
	//clear counter
	TIM2->CNT = 0;
	//enable timer
	TIM2->CR1=CR1_EN;
}

void init_tim2_1hz_interrupt(void)
{
	//Enable clock access to timer2
	RCC->APB1ENR |= TIM2_EN;
	// set prescalar value
	TIM2->PSC = 1600-1;
	//set auto-reload value
	TIM2->ARR = 10000-1;
	//clear counter
	TIM2->CNT = 0;
	//enable timer
	TIM2->CR1=CR1_EN;
	//enable DMA/interrupt enable register
	TIM2->DIER |= DIER_UIE;
	//enable in NVIC
	NVIC_EnableIRQ(TIM2_IRQn);
}
