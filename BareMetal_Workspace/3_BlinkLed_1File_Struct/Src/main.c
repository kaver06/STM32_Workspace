
#include<stdint.h>

#define PERIPH_BASE 			(0x40000000UL)

#define AHB1PERIPH_OFFSET		(0x20000UL)
#define AHB1PERIPH_BASE			(PERIPH_BASE+AHB1PERIPH_OFFSET)

#define GPIOB_OFFSET			(0x400UL)
#define GPIOB_BASE				(AHB1PERIPH_BASE+GPIOB_OFFSET)

#define RCC_OFFSET				(0x3800UL)
#define RCC_BASE				(AHB1PERIPH_BASE+RCC_OFFSET)

#define GPIOBEN	(1U<<1)
#define PIN14	(1U<<14)
#define LED_PIN  PIN14

typedef struct{
	volatile uint32_t MODER;
	volatile uint32_t DUMMY[4];
	volatile uint32_t ODR;
}GPIO_TypeDef;

typedef struct
{
	volatile uint32_t DUMMY[12];
	volatile uint32_t AHB1ENR;
}RCC_TypeDef;

#define RCC ((RCC_TypeDef*)RCC_BASE)
#define GPIOB ((GPIO_TypeDef*)GPIOB_BASE)

int main(void)
{
	RCC->AHB1ENR |= GPIOBEN;

	GPIOB->MODER |= (1U<<28);
	GPIOB->MODER &= ~(1U<<29);

	while(1)
	{
		GPIOB->ODR^=LED_PIN;
		for(int i=0;i<1200000;i++);
	}
}



//RED_LED PB14 MODE_R 1U<<28 | ~(1U<<29)
