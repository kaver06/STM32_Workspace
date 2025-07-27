#include "stm32f446xx.h"
#include <stdint.h>
#include <stdio.h>
#include "uart.h"
#include "adc.h"
#include "Systick.h"
#include "tim.h"


int main(void)
{
	 init_output_compare();

	for(;;);
}

