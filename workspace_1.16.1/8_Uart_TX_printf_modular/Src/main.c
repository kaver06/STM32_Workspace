#include "stm32f446xx.h"
#include <stdint.h>
#include<stdio.h>
#include "uart.h"



int main(void)
{
	init_uart3_tx();
	while(1)
	{
		printf("Here is a Hello from STM32F446ZE.....\r");
	}
}


