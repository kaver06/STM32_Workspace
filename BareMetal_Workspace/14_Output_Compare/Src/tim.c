#include "stm32f446xx.h"


#define TIM4_EN (1U<<2)
#define CR1_EN	(1U<<0)
#define OC_TOGGLE (1U<<13)|(1U<<12)
#define CC2E (1U<<4)
#define GPIOBEN (1U<<1)

void init_tim4_1hz(void)
{
	//Enable clock access to timer4
	RCC->APB1ENR |= TIM4_EN;
	// set prescalar value
	TIM4->PSC = 1600-1;
	//set auto-reload value
	TIM4->ARR = 10000-1;
	//clear counter
	TIM4->CNT = 0;
	//enable timer
	TIM4->CR1=CR1_EN;
}

void init_output_compare(void)
{
	//Enable clock access to GPIOB
	RCC->AHB1ENR |= GPIOBEN;
	//set PB7 to alternate function mode
	GPIOB->MODER |= (1U<<15);
	GPIOB->MODER &= ~(1U<<14);

	//set PB7 alternate function type(AF2)
	GPIOB->AFR[0] |= (1U<<29);
	GPIOB->AFR[0] &= ~(1U<<31);
	GPIOB->AFR[0] &= ~(1U<<30);
	GPIOB->AFR[0] &= ~(1U<<28);

	//Enable clock access to timer4
	RCC->APB1ENR |= TIM4_EN;
	// set prescalar value
	TIM4->PSC = 1600-1;
	//set auto-reload value
	TIM4->ARR = 10000-1;

	//set output compare toggle mode
	TIM4->CCMR1 = OC_TOGGLE;
	//enable tim4 channel 2 in compare mode
	TIM4->CCER = CC2E;


	//clear counter
	TIM4->CNT = 0;
	//enable timer
	TIM4->CR1=CR1_EN;
}
