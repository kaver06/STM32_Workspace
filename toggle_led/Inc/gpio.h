/*
 * gpio.h
 *
 *  Created on: Feb 6, 2025
 *      Author: Admin
 */

#ifndef GPIO_H_
#define GPIO_H_

#include "base.h"

#define GPIOB_MODER_OFFSET		(0x00)
#define GPIOB_MODER				(*(volatile unsigned int *)(GPIOB_PERIPHERAL + GPIOB_MODER_OFFSET ))

#define GPIOB_ODR_OFFSET		(0x14)
#define GPIOB_ODR				(*(volatile unsigned int *)(GPIOB_PERIPHERAL + GPIOB_ODR_OFFSET ))

#define GPIOC_MODER_OFFSET		(0x00)
#define GPIOC_MODER				(*(volatile unsigned int *)(GPIOC_PERIPHERAL + GPIOC_MODER_OFFSET ))

#define GPIOC_IDR_OFFSET		(0x10)
#define GPIOC_IDR				(*(volatile unsigned int *)(GPIOC_PERIPHERAL + GPIOC_IDR_OFFSET ))

#define RCC_AHB1ENR_OFFSET		(0x30)
#define RCC_AHB1ENR				(*(volatile unsigned int *)(RCC_PERIPHERAL + RCC_AHB1ENR_OFFSET ))

void gpio_init(void);
void led_toggle(void);
void check_state(void);
void delay(void);

#endif /* GPIO_H_ */
