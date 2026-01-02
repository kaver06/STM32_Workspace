#include "stm32f446xx.h"
#include <stdint.h>
#include<stdio.h>
#include "uart.h"

static void usart3_interrupt_callback(void);

#define GPIOBEN (1<<1)
#define LedPin	(1<<7)
char key;
int main(void)
{
	init_uart3_interrupt_rxtx();
	RCC->AHB1ENR |= GPIOBEN;

	GPIOB->MODER |= (1<<14);
	GPIOB->MODER &= ~(1<<15);

	for(;;)
	{


	}
}
static void usart3_interrupt_callback(void)
{
	key = USART3->DR;
	if(key=='1')
		GPIOB->ODR |= LedPin;
	else
		GPIOB->ODR &= ~LedPin;
}
void USART3_IRQHandler(void)
{
	if(USART3->SR & RXNE)
		usart3_interrupt_callback();
}



