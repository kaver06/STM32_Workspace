/*
 * ir.c
 *
 *  Created on: Oct 3, 2025
 *      Author: kaver
 */

#include "ir.h"



/*
 * left ir PA0
 * mid left PA1
 * mid right PA4
 * right ir PB0
 */

// Assign pins and ports for 4 sensors
const uint16_t ir_pins[IR_COUNT]  = {GPIO_PIN_0, GPIO_PIN_4, GPIO_PIN_2, GPIO_PIN_0};
GPIO_TypeDef* ir_ports[IR_COUNT]  = {GPIOB, GPIOA, GPIOG, GPIOC};
void IR_Array_Read(IR_Array_T *ir_data)
{
    for (int i = 0; i < IR_COUNT; i++)
    {
        ir_data->state[i] = HAL_GPIO_ReadPin(ir_ports[i], ir_pins[i]);
    }
}

// Convert 4 bits into one number
uint8_t IR_Array_GetPattern(IR_Array_T *ir_data)
{
    uint8_t pattern = 0;
    for (int i = 0; i < IR_COUNT; i++)
    {
        pattern <<= 1;
        pattern |= (ir_data->state[i] & 0x01);
    }

    return pattern;
}



