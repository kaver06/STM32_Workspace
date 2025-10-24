#include "adc.h"
#include "main.h"
#include "uart.h"

extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim7;

// Define (not extern) here:
volatile float battery_voltage = 0.0f;
volatile int battery_percentage = 0;

static volatile uint32_t battery_tick_counter = 0;
static volatile bool battery_read_in_progress = false;

void Start_Battery_ADC_Read(void)
{
    if (!battery_read_in_progress)
    {
        battery_read_in_progress = true;
        HAL_ADC_Start_IT(&hadc1);
    }
}

float BatteryMonitor_GetVoltage(void)
{
    float voltage;
    __disable_irq();
    voltage = battery_voltage;
    __enable_irq();
    return voltage;
}

void BatteryMonitor_Init(void)
{
    // Start timer interrupt
    HAL_TIM_Base_Start_IT(&htim7);

    // Kickstart first read immediately
    Start_Battery_ADC_Read();
}

// TIM6 interrupt handler (every 1 sec)
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM7)
    {
        battery_tick_counter++;
        if (battery_tick_counter >= 5)  // 5 sec interval
        {
            battery_tick_counter = 0;
            Start_Battery_ADC_Read();
        }
    }
}

// ADC conversion complete interrupt handler
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    if (hadc->Instance == ADC1)
    {
        uint32_t raw = HAL_ADC_GetValue(hadc);
        const float vref = 3.1f;
        const float R1 = 102.0f, R2 = 33.0f;
        const float CAL_V = 1.4f;

        float v_adc = ((float)raw / 4095.0f) * vref;
        battery_voltage = (v_adc * ((R1 + R2) / R2)) * CAL_V;
        battery_percentage = (int)((battery_voltage / MAX_BAT_V) * 100.0f);

        if (battery_percentage>95) battery_percentage=99;
        if ((battery_percentage<=95)&&(battery_percentage>90)) battery_percentage=95;
        if ((battery_percentage<=90)&&(battery_percentage>85)) battery_percentage=90;
        if ((battery_percentage<=85)&&(battery_percentage>80)) battery_percentage=85;

        battery_read_in_progress = false;
    }
}
