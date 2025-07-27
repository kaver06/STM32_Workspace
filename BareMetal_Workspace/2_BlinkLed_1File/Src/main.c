

#define PERIPH_BASE 			(0x40000000UL)

#define AHB1PERIPH_OFFSET		(0x20000UL)
#define AHB1PERIPH_BASE			(PERIPH_BASE+AHB1PERIPH_OFFSET)

#define GPIOB_OFFSET			(0x400UL)
#define GPIOB_BASE				(AHB1PERIPH_BASE+GPIOB_OFFSET)

#define RCC_OFFSET				(0x3800UL)
#define RCC_BASE				(AHB1PERIPH_BASE+RCC_OFFSET)

#define AHB1EN_R_OFFSET			(0x30UL)
#define RCC_AHB1ENR_R			(*(volatile unsigned int*)(RCC_BASE+AHB1EN_R_OFFSET))

#define MODE_R_OFFSET			(0x00UL)
#define GPIOB_MODER_R			(*(volatile unsigned int*)(GPIOB_BASE+MODE_R_OFFSET))

#define OD_R_OFFSET				(0x14UL)
#define GPIOB_ODR_R				(*(volatile unsigned int*)(GPIOB_BASE+OD_R_OFFSET))



#define GPIOBEN	(1U<<1)
#define PIN14	(1U<<14)
#define LED_PIN  PIN14

int main(void)
{
	RCC_AHB1ENR_R |= GPIOBEN;

	GPIOB_MODER_R |= (1U<<28);
	GPIOB_MODER_R &= ~(1U<<29);

	while(1)
	{
		GPIOB_ODR_R^=LED_PIN;
		for(int i=0;i<1200000;i++);
	}


}



//RED_LED PB14 MODE_R 1U<<28 | ~(1U<<29)
