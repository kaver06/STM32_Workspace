#include "gpio.h"
int main(void)
{
	gpio_init();
	while(1)
	{
		red_led_blink();
	}
}
