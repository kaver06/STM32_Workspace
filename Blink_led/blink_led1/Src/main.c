#include "gpio.h"
int main(void)
{
    /* Loop forever */

	gpio_init();
	for(;;)
	{
		red_led_blink();
	}
}
