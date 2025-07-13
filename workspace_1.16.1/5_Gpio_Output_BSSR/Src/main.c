#include "stm32f4xx.h"




#define GPIOBN (1U<<1)
#define PIN7   (1U<<7)
#define LedPin PIN7

int main(void)
{
	RCC->AHB1ENR |= GPIOBN;

	GPIOB->MODER |= (1U<<14);
	GPIOB->MODER &= ~(1U<<15);

	while(1)
	{
		GPIOB->BSRR |= (LedPin);
		for(int i=0;i<1200000;i++);
		GPIOB->BSRR |= (1U<<23);
		for(int i=0;i<1200000;i++);
	}
}
