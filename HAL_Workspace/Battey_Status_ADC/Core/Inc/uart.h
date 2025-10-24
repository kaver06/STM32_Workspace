/*
 * uart.h
 *
 *  Created on: Oct 17, 2025
 *      Author: kaver
 */

#ifndef INC_UART_H_
#define INC_UART_H_

#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <string.h>

void UART2_Init(UART_HandleTypeDef* huart);
void UART2_SendString_IT(const char* str);

#endif /* INC_UART_H_ */
