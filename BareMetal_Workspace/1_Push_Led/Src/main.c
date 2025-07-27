#include "gpio.h"

int main(void)
{
	gpio_init();

	while(1)
	{
		check_state();
	}
}
