#include "stm32f446xx.h"

#define SYSTICK_LOAD_VAL 	16000
#define SYSTICk_EN		 	(1U<<0)
#define SYSTICK_CLKSOURCE	(1U<<2)
#define SYSTICK_COUNTFLAG	(1U<<16)

void SystickDelay(int delay)
{
	//reload with number of clocks per milliseconds
	SysTick->LOAD = SYSTICK_LOAD_VAL;
	//clear systick current value register
	SysTick->VAL = 0;
	//Enable systick and select internal clk source
	SysTick->CTRL = SYSTICk_EN | SYSTICK_CLKSOURCE;

	for(int i=0;i<delay;i++)
	{
		//wait until the count flag is set
		while((SysTick->CTRL & SYSTICK_COUNTFLAG)==0);
	}
	//finally turn off the systick control
	SysTick->CTRL = 0;
}






