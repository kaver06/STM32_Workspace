/*
 * gpio.c
 *
 *  Created on: Feb 6, 2025
 *      Author: Admin
 */


#include "gpio.h"

#define GPIOBEN (1U<<1)
#define GPIOCEN (1U<<2)

#define GPIOB14 (1U<<14)
#define GPIOC13 (1U<<13)

void gpio_init(void)
{
	//enable clock access to GPIO ports B
		//enable clock to GPIO B

		RCC_AHB1ENR |= GPIOBEN;
		RCC_AHB1ENR |= GPIOCEN;


		GPIOB_MODER |= (1U<<28);
		GPIOB_MODER &= ~(1U<<29);

		GPIOC_MODER &= ~(1U<<26);
		GPIOC_MODER &= ~(1U<<27);
}

void led_toggle(void)
{
	GPIOB_ODR ^= GPIOB14;
	delay();
}

void delay(void)
{
	for(int x=0;x<500000;x++);
}
void check_state(void)
{
	if(GPIOC_IDR && GPIOC13)
	{
		led_toggle();
	}
//	else
//	{
//		led_toggle();
//	}
}

