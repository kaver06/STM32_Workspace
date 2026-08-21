/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#define Temperature_Flag (1U<<0)
#define Battery_Flag	(1U<<1)
#define Pressure_Flag	(1U<<2)
#define ACQUISITION_ENABLE_FLAG  (1U << 0)
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef struct{
	uint32_t temperature;
	uint32_t pressure;
	uint32_t battery;
	uint32_t sample_number;
}Raw_Data;

typedef struct{
	uint32_t bat_threshold;
	uint32_t temp_threshold;
	uint32_t press_threshold;
}Sensor_Threshold;

typedef enum{
	CMD_RESET,
	CMD_SET_TEMP,
	CMD_SET_BAT,
	CMD_SET_PRE
}Command_Type;

typedef struct{
	Command_Type type;
	uint32_t value;
}Command;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart3;

/* Definitions for AcquisitionTask */
osThreadId_t AcquisitionTaskHandle;
const osThreadAttr_t AcquisitionTask_attributes = {
  .name = "AcquisitionTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for ProcessingTask */
osThreadId_t ProcessingTaskHandle;
const osThreadAttr_t ProcessingTask_attributes = {
  .name = "ProcessingTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};
/* Definitions for AlarmTask */
osThreadId_t AlarmTaskHandle;
const osThreadAttr_t AlarmTask_attributes = {
  .name = "AlarmTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for ConfigurationTa */
osThreadId_t ConfigurationTaHandle;
const osThreadAttr_t ConfigurationTa_attributes = {
  .name = "ConfigurationTa",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal,
};
/* Definitions for StatusTask */
osThreadId_t StatusTaskHandle;
const osThreadAttr_t StatusTask_attributes = {
  .name = "StatusTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal,
};
/* Definitions for RawData */
osMessageQueueId_t RawDataHandle;
const osMessageQueueAttr_t RawData_attributes = {
  .name = "RawData"
};
/* Definitions for CommandQueue */
osMessageQueueId_t CommandQueueHandle;
const osMessageQueueAttr_t CommandQueue_attributes = {
  .name = "CommandQueue"
};
/* Definitions for ConfigparamsMutex */
osMutexId_t ConfigparamsMutexHandle;
const osMutexAttr_t ConfigparamsMutex_attributes = {
  .name = "ConfigparamsMutex"
};
/* Definitions for AlarmSemaphore */
osSemaphoreId_t AlarmSemaphoreHandle;
const osSemaphoreAttr_t AlarmSemaphore_attributes = {
  .name = "AlarmSemaphore"
};
/* Definitions for ResetSemaphore */
osSemaphoreId_t ResetSemaphoreHandle;
const osSemaphoreAttr_t ResetSemaphore_attributes = {
  .name = "ResetSemaphore"
};
/* Definitions for abnormalflags */
osEventFlagsId_t abnormalflagsHandle;
const osEventFlagsAttr_t abnormalflags_attributes = {
  .name = "abnormalflags"
};
/* USER CODE BEGIN PV */
Sensor_Threshold thresholds = {
    .bat_threshold = 20,
    .temp_threshold = 95,
    .press_threshold = 90
};
volatile uint32_t current_alarm_state = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART3_UART_Init(void);
void StartAcquisitionTask(void *argument);
void StartProcessingTask(void *argument);
void StartAlarmTask(void *argument);
void StartConfigurationTask(void *argument);
void StartStatusTask(void *argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();
  /* Create the mutex(es) */
  /* creation of ConfigparamsMutex */
  ConfigparamsMutexHandle = osMutexNew(&ConfigparamsMutex_attributes);

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of AlarmSemaphore */
  AlarmSemaphoreHandle = osSemaphoreNew(1, 0, &AlarmSemaphore_attributes);

  /* creation of ResetSemaphore */
  ResetSemaphoreHandle = osSemaphoreNew(1, 0, &ResetSemaphore_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of RawData */
  RawDataHandle = osMessageQueueNew (4, sizeof(Raw_Data), &RawData_attributes);

  /* creation of CommandQueue */
  CommandQueueHandle = osMessageQueueNew (4, sizeof(Command), &CommandQueue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of AcquisitionTask */
  AcquisitionTaskHandle = osThreadNew(StartAcquisitionTask, NULL, &AcquisitionTask_attributes);

  /* creation of ProcessingTask */
  ProcessingTaskHandle = osThreadNew(StartProcessingTask, NULL, &ProcessingTask_attributes);

  /* creation of AlarmTask */
  AlarmTaskHandle = osThreadNew(StartAlarmTask, NULL, &AlarmTask_attributes);

  /* creation of ConfigurationTa */
  ConfigurationTaHandle = osThreadNew(StartConfigurationTask, NULL, &ConfigurationTa_attributes);

  /* creation of StatusTask */
  StatusTaskHandle = osThreadNew(StartStatusTask, NULL, &StatusTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* creation of abnormalflags */
  abnormalflagsHandle = osEventFlagsNew(&abnormalflags_attributes);

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  osEventFlagsSet(abnormalflagsHandle, ACQUISITION_ENABLE_FLAG);
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartAcquisitionTask */
/**
  * @brief  Function implementing the AcquisitionTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartAcquisitionTask */
void StartAcquisitionTask(void *argument)
{
  /* USER CODE BEGIN 5 */
	Raw_Data raw;
	uint32_t bat=95;
	uint32_t temp=25;
	uint32_t press=35;
	uint32_t sample=0;
	osStatus_t status;

  /* Infinite loop */
  for(;;)
  {
	osEventFlagsWait(abnormalflagsHandle, ACQUISITION_ENABLE_FLAG, osFlagsNoClear|osFlagsWaitAny, osWaitForever);
	status=osSemaphoreAcquire(ResetSemaphoreHandle, 0);
	if(status==osOK)
	{
		 bat=95;
		 temp=25;
		 press=35;
		 sample=0;
	}

	bat--;temp++;press++;sample++;
	raw.battery = bat;
	raw.temperature = temp;
	raw.pressure = press;
	raw.sample_number = sample;

	osMessageQueuePut(RawDataHandle, &raw, 0, osWaitForever);
	osDelay(500);
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartProcessingTask */
/**
* @brief Function implementing the ProcessingTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartProcessingTask */
void StartProcessingTask(void *argument)
{
  /* USER CODE BEGIN StartProcessingTask */
	Raw_Data raw;

	uint32_t l_bat_threshold;
	uint32_t l_temp_threshold;
	uint32_t l_press_threshold;
    uint32_t new_alarm_state;

  /* Infinite loop */
  for(;;)
  {
    osMessageQueueGet(RawDataHandle, &raw, 0, osWaitForever);
    osMutexAcquire(ConfigparamsMutexHandle, osWaitForever);
	l_bat_threshold =thresholds.bat_threshold;
	l_temp_threshold =thresholds.temp_threshold;
	l_press_threshold=thresholds.press_threshold;
    osMutexRelease(ConfigparamsMutexHandle);
    new_alarm_state = 0;
    if(l_bat_threshold>raw.battery)
    	new_alarm_state |= Battery_Flag;
    if(l_temp_threshold<raw.temperature)
    	 new_alarm_state |= Temperature_Flag;
    if(l_press_threshold<raw.pressure)
    	new_alarm_state |= Pressure_Flag;
    if(new_alarm_state != current_alarm_state)
   {
	   current_alarm_state = new_alarm_state;
	   osSemaphoreRelease(AlarmSemaphoreHandle);
   }
  }
  /* USER CODE END StartProcessingTask */
}

/* USER CODE BEGIN Header_StartAlarmTask */
/**
* @brief Function implementing the AlarmTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartAlarmTask */
void StartAlarmTask(void *argument)
{
  /* USER CODE BEGIN StartAlarmTask */

  /* Infinite loop */
  for(;;)
  {
	  osSemaphoreAcquire(AlarmSemaphoreHandle, osWaitForever);
	  switch(current_alarm_state)
	  {
	  	  case 0x00:
	  		osEventFlagsSet(abnormalflagsHandle, ACQUISITION_ENABLE_FLAG);
			  break;
	      case 0x01:
	    	  osEventFlagsSet(abnormalflagsHandle, ACQUISITION_ENABLE_FLAG);
	          break;
	      case 0x02:
	    	  osEventFlagsSet(abnormalflagsHandle, ACQUISITION_ENABLE_FLAG);
	          break;
	      case 0x03:
	    	  osEventFlagsSet(abnormalflagsHandle, ACQUISITION_ENABLE_FLAG);
	          break;
	      case 0x04:
	    	  osEventFlagsSet(abnormalflagsHandle, ACQUISITION_ENABLE_FLAG);
	          break;
	      case 0x05:
	    	  osEventFlagsClear(abnormalflagsHandle, ACQUISITION_ENABLE_FLAG);
	          break;
	      case 0x06:
	    	  osEventFlagsClear(abnormalflagsHandle, ACQUISITION_ENABLE_FLAG);
	          break;
	      case 0x07:
	    	  osEventFlagsClear(abnormalflagsHandle, ACQUISITION_ENABLE_FLAG);
	          break;
	  }
  }
  /* USER CODE END StartAlarmTask */
}

/* USER CODE BEGIN Header_StartConfigurationTask */
/**
* @brief Function implementing the ConfigurationTa thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartConfigurationTask */
void StartConfigurationTask(void *argument)
{
  /* USER CODE BEGIN StartConfigurationTask */
	Command cmd;
	osMutexAcquire(ConfigparamsMutexHandle, osWaitForever);
	uint32_t l_bat_threshold =thresholds.bat_threshold;
	uint32_t l_temp_threshold =thresholds.temp_threshold;
	uint32_t l_press_threshold=thresholds.press_threshold;
	osMutexRelease(ConfigparamsMutexHandle);
  /* Infinite loop */
  for(;;)
  {
	  osMessageQueueGet(CommandQueueHandle, &cmd, 0, osWaitForever);
	  switch(cmd.type)
	  {
	  	  case CMD_RESET:
	  		osSemaphoreRelease(ResetSemaphoreHandle);
	  		osEventFlagsSet(abnormalflagsHandle,ACQUISITION_ENABLE_FLAG);
	  		break;
	  	  case CMD_SET_TEMP:
	  		l_temp_threshold=cmd.value;
	  		break;
	  	  case CMD_SET_BAT:
	  		l_bat_threshold=cmd.value;
	  		break;
	  	  case CMD_SET_PRE:
	  		l_press_threshold=cmd.value;
	  		break;
	  }
	  osMutexAcquire(ConfigparamsMutexHandle, osWaitForever);
	  thresholds.bat_threshold=l_bat_threshold;
	  thresholds.press_threshold=l_press_threshold;
	  thresholds.temp_threshold=l_temp_threshold;
	  osMutexRelease(ConfigparamsMutexHandle);
  }
  /* USER CODE END StartConfigurationTask */
}

/* USER CODE BEGIN Header_StartStatusTask */
/**
* @brief Function implementing the StatusTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartStatusTask */
void StartStatusTask(void *argument)
{
  /* USER CODE BEGIN StartStatusTask */
    Command cmd;
    osDelay(3000);

    cmd.type = CMD_SET_TEMP;
	cmd.value = 40;
	osMessageQueuePut(CommandQueueHandle,&cmd,0,osWaitForever);
	osDelay(3000);
	cmd.type = CMD_SET_PRE;
	cmd.value = 50;
	osMessageQueuePut(CommandQueueHandle,&cmd,0,osWaitForever);
	osDelay(3000);
	cmd.type = CMD_RESET;
	cmd.value = 0;
	osMessageQueuePut(CommandQueueHandle,&cmd,0,osWaitForever);
	osDelay(3000);
  /* Infinite loop */
  for(;;)
  {
	  osDelay(osWaitForever);
  }
  /* USER CODE END StartStatusTask */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6)
  {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
