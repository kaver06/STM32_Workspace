#include <stdio.h>
#include <stdint.h>
#include "stm32f446xx.h"
#include "uart.h"




int main(void)
{


	init_uart3_tx();

	while(1)
	{
		printf("Hello from STM32F4........\n\r");
	}

}





