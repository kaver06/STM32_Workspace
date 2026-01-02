#include "stm32f446xx.h"
#include <stdint.h>
#include <stdio.h>
#include "uart.h"
#include "adc.h"
#include "Systick.h"
#include "tim.h"

#define GPIOBEN (1U<<1)
#define LED_PIN	(1U<<7)
void timer_callback(void);
int main(void)
{
	init_uart3_rxtx();
	init_tim2_1hz_interrupt();

	RCC->AHB1ENR|=GPIOBEN;

	GPIOB->MODER |= (1U<<14);
	GPIOB->MODER &= ~(1U<<15);

	for(;;)
	{

	}
}

void timer_callback(void)
{
	GPIOB->ODR ^= LED_PIN;
	printf("A second Passed !!\r");
}
void TIM2_IRQHandler(void)
{			//wait for UIF flag
	if((TIM2->SR & SR_UIF)!=0)
	{
		//clear UIF
		TIM2->SR &= ~SR_UIF;
		timer_callback();
	}
}


