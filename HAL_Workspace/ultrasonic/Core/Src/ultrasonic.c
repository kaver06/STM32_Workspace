#include "ultrasonic.h"
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;


#define BUFFER_SIZE 1
volatile uint32_t rising1 = 0, falling1 = 0;
volatile uint32_t rising2 = 0, falling2 = 0;
volatile uint32_t rising3 = 0, falling3 = 0;

volatile uint32_t distance1_cm = 0;
volatile uint32_t distance2_cm = 0;
volatile uint32_t distance3_cm = 0;
volatile uint8_t object1_near_flag = 0;
volatile uint8_t object2_near_flag = 0;
volatile uint8_t object3_near_flag = 0;

void Ultrasonic_Init(void)
{
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1); // Sensor 1 trigger
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3); // Sensor 2 trigger
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);// Sensor 3 trigger

    HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_2); // Sensor 1 echo
    HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_4); // Sensor 2 echo
    HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_2); // Sensor 3 echo
}

// Distance getters
uint32_t Ultrasonic1_GetDistance(void) { return distance1_cm; }
uint32_t Ultrasonic2_GetDistance(void) { return distance2_cm; }
uint32_t Ultrasonic3_GetDistance(void) { return distance3_cm; }

// Process function: just for flag update (optional)
void Ultrasonic_Process(void)
{
    HAL_GPIO_WritePin(LED1_PORT, LED1_PIN, object1_near_flag);
    HAL_GPIO_WritePin(LED2_PORT, LED2_PIN, object2_near_flag);
   // HAL_GPIO_WritePin(LED3_PORT, LED3_PIN, object3_near_flag);
}

// ---- Callback function ----
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM2)
	{
	    if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2) // Sensor 1
	    {
			if(htim->Instance->CCER & TIM_CCER_CC2P) // falling edge
			{
				falling1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
				distance1_cm = (falling1 - rising1) * 0.0343 / 2;
				object1_near_flag = (distance1_cm < DISTANCE_THRESHOLD);
				__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_2, TIM_INPUTCHANNELPOLARITY_RISING);
			}
			else // rising edge
			{
				rising1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
				__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_2, TIM_INPUTCHANNELPOLARITY_FALLING);
			}
	    }
	    else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4) // Sensor 2
	    {
			if(htim->Instance->CCER & TIM_CCER_CC4P) // falling edge
			{
				falling2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4);
				distance2_cm = (falling2 - rising2) * 0.0343 / 2;
				object2_near_flag = (distance2_cm < DISTANCE_THRESHOLD);
				__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_4, TIM_INPUTCHANNELPOLARITY_RISING);
			}
			else // rising edge
			{
				rising2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4);
				__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_4, TIM_INPUTCHANNELPOLARITY_FALLING);
			}
	    }
	}
	else if(htim->Instance == TIM3 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
	{
	    uint32_t captured = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
	    if(!rising3)
	    {
	        rising3 = captured;
	        __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_2, TIM_INPUTCHANNELPOLARITY_FALLING);
	    }
	    else
	    {
	        falling3 = captured;
	        distance3_cm = (falling3 - rising3) * 0.0343 / 2;
	        object3_near_flag = (distance3_cm < DISTANCE_THRESHOLD);
	        rising3 = 0;
	        __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_2, TIM_INPUTCHANNELPOLARITY_RISING);
	    }
	}

}

