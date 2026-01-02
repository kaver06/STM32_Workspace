#include "stm32f446xx.h"

#define GPIOCEN (1U<<2)
#define SYSCFGEN (1U<<14)

void pc13_exti_init(void)
{
	__disable_irq();

	// Enable GPIOC clock (for PC13 input)
	RCC->AHB1ENR |= GPIOCEN;

	// Set PC13 as input
	GPIOC->MODER &= ~(1U << 26);
	GPIOC->MODER &= ~(1U << 27);

	// Enable SYSCFG clock
	RCC->APB2ENR |= SYSCFGEN;

	// Connect EXTI13 to PC13 (EXTICR[3], bits 7:4)

	SYSCFG->EXTICR[3] |=  (1U << 5);  // Set to PORTC (value 0b0010)

	// Unmask EXTI13
	EXTI->IMR |= (1U << 13);

	// Falling edge trigger
	EXTI->FTSR |= (1U << 13);

	// Enable NVIC interrupt for EXTI13 (in EXTI15_10 range)
	NVIC_EnableIRQ(EXTI15_10_IRQn);

	__enable_irq();
}
