#include "stm32f4xx.h"



#define GPIOCN	(1U<<2)
#define GPIOBN 	(1U<<1)
#define PIN7   	(1U<<7)
#define PIN13	(1U<<13)
#define LedPin 	PIN7
#define PushPin	PIN13

int main(void)
{
	RCC->AHB1ENR |= GPIOBN;
	RCC->AHB1ENR |= GPIOCN;


	GPIOB->MODER |= (1U<<14);
	GPIOB->MODER &= ~(1U<<15);

	GPIOC->MODER &= ~(1U<<26);
	GPIOC->MODER &= ~(1U<<27);

	while(1)
	{
		if(GPIOC->IDR & PushPin)
		{
			GPIOB->BSRR |= (LedPin);
		}
		else
		{
			GPIOB->BSRR |= (1U<<23);
		}
	}
}
