#include "gpio.h"



int main(void)
{
	gpio_init();
    /* Loop forever */
	for(;;)
	{
		check_state();
	}
}
