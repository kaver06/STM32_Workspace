/*
 * ir.h
 *
 *  Created on: Oct 3, 2025
 *      Author: kaver
 */

#ifndef INC_IR_H_
#define INC_IR_H_


#include "stm32f4xx_hal.h"

#define IR_COUNT	4

typedef struct{
	uint8_t state[IR_COUNT];
}IR_Array_T;

void IR_Array_Read(IR_Array_T *ir_data);
uint8_t IR_Array_GetPattern(IR_Array_T *ir_data);

#endif /* INC_IR_H_ */
