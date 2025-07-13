#include "gpio.h"

#define GPIOBEN (1U<<1)
#define GPIOCEN (1U<<2)

#define GPIOB7 (1U<<7)
#define GPIOC13 (1U<<13)

void gpio_init(void)
{
	//enable clock access to GPIO ports B and c


		RCC_AHB1ENR |= GPIOBEN;
		RCC_AHB1ENR |= GPIOCEN;


		GPIOB_MODER |= (1U<<14);
		GPIOB_MODER &= ~(1U<<15);

		GPIOC_MODER &= ~(1U<<26);
		GPIOC_MODER &= ~(1U<<27);



}



void delay(void)
{
	for(int x=0;x<500000;x++);
}
void check_state(void)
{
	if(GPIOC_IDR && GPIOC13)
	{
		GPIOB_ODR ^=(GPIOB7);
		delay();
	}
}
