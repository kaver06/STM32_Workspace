#ifndef ENCODER_H
#define ENCODER_H

#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdbool.h>
#define ENCODER_CPR 1024

/**
 * Initialize both encoders.
 * Call after MX_TIM4_Init() and MX_TIM5_Init() (the CubeMX generated timer init functions).
 * This function will start the encoder interfaces and enable update interrupts.
 */
void encoder_init(void);

/**
 * Get 32-bit signed position (counts).
 * The returned value is the extended count (includes overflows).
 * Left = TIM4, Right = TIM5.
 */
int32_t encoder_get_left_position(void);
int32_t encoder_get_right_position(void);

/**
 * Reset encoder positions to zero (atomic).
 */
void encoder_reset_positions(void);

/**
 * These functions should be called from the IRQ handlers in stm32f4xx_it.c
 * Example:
 *   void TIM4_IRQHandler(void) {
 *     HAL_TIM_IRQHandler(&htim4);
 *   }
 * HAL will call HAL_TIM_PeriodElapsedCallback -> implemented in encoder.c
 */
void encoder_tim_update_callback(TIM_HandleTypeDef *htim);
float encoder_get_left_degree(int32_t left_count);
float encoder_get_right_degree(int32_t right_count);
#endif // ENCODER_H
