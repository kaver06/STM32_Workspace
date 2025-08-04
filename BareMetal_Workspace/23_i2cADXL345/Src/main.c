#include "stm32f446xx.h"
#include <stdint.h>
#include<stdio.h>
#include "uart.h"

static void dma1_callback(void);

#define GPIOBEN (1U<<1)
#define LedPin	(1U<<7)
#define DMA1_TCIF3 (1U<<27)
#define LIFCR_CTCIF (1U<<27)
int main(void)
{
	char message[25]= "Hello from STM32F446ZE\r";
	RCC->AHB1ENR |=	GPIOBEN;
	GPIOB->MODER |= (1<<14);
	GPIOB->MODER &= ~(1<<15);

	init_uart3_tx();
	i2c_init();
	dma1_stream3_init((uint32_t)message,(uint32_t)&USART3->DR,31);

	for(;;)
	{


	}
}

static void dma1_callback(void)
{
	GPIOB->ODR |= LedPin;
}
void DMA1_Stream3_IRQHandler(void)
{
	if((DMA1->LISR & DMA1_TCIF3))
	{
		//clear the flag
		DMA1->LIFCR |= LIFCR_CTCIF;

		dma1_callback();
	}
}


