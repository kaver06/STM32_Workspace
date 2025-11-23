/*
 * uart.c
 *
 *  Created on: Oct 17, 2025
 *      Author: kaver
 */

#include "uart.h"

static UART_HandleTypeDef* uart2_handle;

void UART2_Init(UART_HandleTypeDef* huart)
{
    uart2_handle = huart;
}

void UART2_SendString_IT(const char* str)
{
    if(uart2_handle != NULL)
    {
        HAL_UART_Transmit_IT(uart2_handle, (uint8_t*)str, strlen(str));
    }
}
