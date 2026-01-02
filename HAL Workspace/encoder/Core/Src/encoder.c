#include "encoder.h"
#include "main.h"   // for extern TIM_HandleTypeDef htim4/htim5 (CubeMX generated)
#include "stm32f4xx_hal.h"
#include <string.h>

extern TIM_HandleTypeDef htim4; // left
extern TIM_HandleTypeDef htim5; // right

/* internal 32-bit extended positions */
static volatile int32_t left_pos = 0;
static volatile int32_t right_pos = 0;

/* store ARR for convenience */
static uint32_t tim4_arr_val = 0xFFFF;
static uint32_t tim5_arr_val = 0xFFFF;

/* store last raw counter snapshots (optional) */
static volatile uint16_t last_cnt4 = 0;
static volatile uint16_t last_cnt5 = 0;

void encoder_init(void)
{
    /* Ensure ARR values match CubeMX config (if different change accordingly) */
    tim4_arr_val = __HAL_TIM_GET_AUTORELOAD(&htim4);
    tim5_arr_val = __HAL_TIM_GET_AUTORELOAD(&htim5);

    /* Reset internal counters */
    left_pos = 0;
    right_pos = 0;
    last_cnt4 = (uint16_t)__HAL_TIM_GET_COUNTER(&htim4);
    last_cnt5 = (uint16_t)__HAL_TIM_GET_COUNTER(&htim5);

    /* Start encoder interface with interrupts enabled for update (overflow/underflow) */
    HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(&htim5, TIM_CHANNEL_ALL);

    /* Enable update interrupt for both timers to track over/underflow */
    __HAL_TIM_ENABLE_IT(&htim4, TIM_IT_UPDATE);
    __HAL_TIM_ENABLE_IT(&htim5, TIM_IT_UPDATE);
}

/* Called by HAL in the update interrupt context.
   You need to call this from HAL's PeriodElapsedCallback or directly from your IRQ handler */
void encoder_tim_update_callback(TIM_HandleTypeDef *htim)
{
    /* TIM4 update -> left encoder overflow/underflow */
    if (htim->Instance == htim4.Instance) {
        /* Determine direction: DIR bit = 0 => counting up, DIR = 1 => counting down */
        uint32_t dir = (htim4.Instance->CR1 & TIM_CR1_DIR) ? 1U : 0U;
        if (dir == 0) {
            /* counter wrapped upward (from 0xFFFF -> 0) => we should add (ARR+1) */
            left_pos += (int32_t)(tim4_arr_val + 1U);
        } else {
            /* wrapped downward (from 0 -> 0xFFFF) => subtract */
            left_pos -= (int32_t)(tim4_arr_val + 1U);
        }
        /* update last snapshot */
        last_cnt4 = (uint16_t)__HAL_TIM_GET_COUNTER(&htim4);
    }
    /* TIM5 update -> right encoder */
    else if (htim->Instance == htim5.Instance) {
        uint32_t dir = (htim5.Instance->CR1 & TIM_CR1_DIR) ? 1U : 0U;
        if (dir == 0) {
            right_pos += (int32_t)(tim5_arr_val + 1U);
        } else {
            right_pos -= (int32_t)(tim5_arr_val + 1U);
        }
        last_cnt5 = (uint16_t)__HAL_TIM_GET_COUNTER(&htim5);
    }
}

/* HAL callback bridge: implement HAL_TIM_PeriodElapsedCallback in this file so HAL calls encoder_tim_update_callback */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    /* This callback will be invoked for update events. Delegate */
    encoder_tim_update_callback(htim);
}

/* Public getters (atomic read) */
int32_t encoder_get_left_position(void)
{
    int32_t pos;
    uint32_t prim = __get_PRIMASK();
    /* disable interrupts to read safely (short critical section) */
    __disable_irq();
    uint16_t cnt = (uint16_t)__HAL_TIM_GET_COUNTER(&htim4);
    pos = left_pos + (int32_t)cnt;
    if (!prim) { __enable_irq(); }
    return pos;
}

int32_t encoder_get_right_position(void)
{
    int32_t pos;
    uint32_t prim = __get_PRIMASK();
    __disable_irq();
    uint16_t cnt = (uint16_t)__HAL_TIM_GET_COUNTER(&htim5);
    pos = right_pos + (int32_t)cnt;
    if (!prim) { __enable_irq(); }
    return pos;
}

float encoder_get_left_degree(int32_t left_count)
{
    float left_revs  = (float)left_count  / (ENCODER_CPR * 4.0f);
    float left_deg  = left_revs  * 360.0f;
    return left_deg;
}

float encoder_get_right_degree(int32_t right_count)
{
	 float right_revs = (float)right_count / (ENCODER_CPR * 4.0f);
	 float right_deg = right_revs * 360.0f;
	 return right_deg;
}

void encoder_reset_positions(void)
{
    uint32_t prim = __get_PRIMASK();
    __disable_irq();
    left_pos = 0;
    right_pos = 0;
    __HAL_TIM_SET_COUNTER(&htim4, 0);
    __HAL_TIM_SET_COUNTER(&htim5, 0);
    last_cnt4 = 0;
    last_cnt5 = 0;
    if (!prim) { __enable_irq(); }
}
