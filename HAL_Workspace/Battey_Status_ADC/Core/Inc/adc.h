#ifndef INC_ADC_H_
#define INC_ADC_H_

#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#define MAX_BAT_V 13.2f
void BatteryMonitor_Init(void);
float BatteryMonitor_GetVoltage(void);
void Start_Battery_ADC_Read(void);

extern volatile float battery_voltage;
extern volatile int battery_percentage;

#endif /* INC_ADC_H_ */
