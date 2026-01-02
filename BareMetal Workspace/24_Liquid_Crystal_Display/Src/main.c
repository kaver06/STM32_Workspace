#include "stm32f446xx.h"
#include <stdint.h>
#include <stdio.h>
#include "uart.h"
#include "adc.h"
#include "Systick.h"
#include "tim.h"
#include "exti.h"
#include "lcd.h"


int main(void)
{
	LCD_Init();
	LCD_Data("K");
	LCD_Data("A");
	LCD_Data("V");
	LCD_Data("E");
	LCD_Data("R");
	Delay(200);
	GPIOB->MODER |= (1U<<14);
	GPIOB->MODER &= ~(1U<<15);
	while(1)
	{

		GPIOB->ODR ^= (1U<<7);
		Delay(500);
	}
}
