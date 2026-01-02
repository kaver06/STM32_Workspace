#include "stm32f446xx.h"
#include <stdint.h>
#include <stdio.h>
#include "uart.h"
#include "adc.h"
#include "Systick.h"


#define GPIOBEN (1U<<1)
#define LED_PIN	(1U<<7)
int main(void)
{
	init_uart3_rxtx();
	Systick_1hz_interrupt();
	RCC->AHB1ENR|=GPIOBEN;

	GPIOB->MODER |= (1U<<14);
	GPIOB->MODER &= ~(1U<<15);

	for(;;)
	{

	}
}

void systick_callback(void)
{
	GPIOB->ODR ^= LED_PIN;
	printf("A second Passed !!\r");
}
void SysTick_Handler(void)
{
		systick_callback();

}
