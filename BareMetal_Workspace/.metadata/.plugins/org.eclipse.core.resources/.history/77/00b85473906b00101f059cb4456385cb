#include "stm32f446xx.h"
#include <stdint.h>
#include <stdio.h>
#include "uart.h"
#include "adc.h"

uint32_t sensor_value;

int main(void)
{
	init_uart3_rxtx();
	init_adc_PA1();


	for(;;)
	{
		start_conversion();
		sensor_value = read_adc();
		printf("THE ADC VALUE is %d \r",(int)sensor_value);
	}
}

