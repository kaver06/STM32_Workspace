#include "stm32f446xx.h"
#include <stdint.h>
#include<stdio.h>
#include "uart.h"

#define GPIOBEN (1<<1)
#define LedPin	(1<<7)
char key;
int main(void)
{
	init_uart3_rxtx();
	RCC->AHB1ENR |= GPIOBEN;

	GPIOB->MODER |= (1<<14);
	GPIOB->MODER &= ~(1<<15);

	for(;;)
	{
		key = uart3_read();
		if(key=='1')
			GPIOB->ODR |= LedPin;
		else
			GPIOB->ODR &= ~LedPin;
	}
}


