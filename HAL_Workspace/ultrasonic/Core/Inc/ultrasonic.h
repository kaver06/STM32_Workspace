/*
 * ultrasonic.h
 *
 *  Created on: Oct 20, 2025
 *      Author: kaver
 */

#ifndef INC_ULTRASONIC_H_
#define INC_ULTRASONIC_H_

#include <stdio.h>
#include "main.h"

#define LED1_PIN          GPIO_PIN_7
#define LED1_PORT         GPIOB
#define LED2_PIN          GPIO_PIN_14
#define LED2_PORT         GPIOB
//#define LED3_PIN		  GPIO_PIN_0
//#define LED3_PORT		  GPIOB
#define DISTANCE_THRESHOLD 30  // cm

// Initialize ultrasonic sensor (TIM2 PWM + Input Capture with DMA)
void Ultrasonic_Init(void);

// Process captured values and update distances/flags
void Ultrasonic_Process(void);

// Get last measured distance
uint32_t Ultrasonic1_GetDistance(void);
uint32_t Ultrasonic2_GetDistance(void);
uint32_t Ultrasonic3_GetDistance(void);

// Flags if object is closer than threshold
extern volatile uint8_t object1_near_flag;
extern volatile uint8_t object2_near_flag;
extern volatile uint8_t object3_near_flag;

#endif /* INC_ULTRASONIC_H_ */
