/*
 * gpio.c
 *
 *  Created on: Feb 4, 2025
 *      Author: Admin
 */
#include "gpio.h"


#define GPIOBEN (1U<<1)

#define GPIOB14	(1U<<14)	//RED LED
#define GPIOB7 (1U<<7)          //BLUE LED

#define RED_LED GPIOB14



void gpio_init(void)
{
	//enable clock access to GPIO ports B
	//enable clock to GPIO B

	RCC_AHB1ENR |= GPIOBEN;

	//set mode of the pins

	//BLUE LED, GPIO7 OUTPUT
	//GPIOB_MODER |= (1U<<14);
	//GPIOB_MODER &= ~(1U<<15);

	//red led,GPIO14 output
	GPIOB_MODER |= (1U<<28);
	GPIOB_MODER &= ~(1U<<29);
}

void red_led_blink(void)
{
	GPIOB_ODR |= RED_LED;
	for(int x=0;x<1800000;x++);
	GPIOB_ODR &= ~(RED_LED);
}

