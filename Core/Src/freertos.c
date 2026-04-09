/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "motor.h"
#include "stdio.h"
#include "adc.h"
#include "usart.h"
#include <string.h>
/* 外部全局标志位声明 */
extern volatile uint8_t usart2_rx_flag;
extern volatile uint8_t usart3_rx_flag;
extern volatile uint8_t encoder1_flag;
extern volatile uint8_t encoder2_flag;
extern volatile uint8_t encoder3_flag;
extern volatile uint8_t encoder4_flag;
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for VisionTask */
osThreadId_t VisionTaskHandle;
const osThreadAttr_t VisionTask_attributes = {
  .name = "VisionTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for MotionTask */
osThreadId_t MotionTaskHandle;
const osThreadAttr_t MotionTask_attributes = {
  .name = "MotionTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};
/* Definitions for StateTask */
osThreadId_t StateTaskHandle;
const osThreadAttr_t StateTask_attributes = {
  .name = "StateTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for DebugTask */
osThreadId_t DebugTaskHandle;
const osThreadAttr_t DebugTask_attributes = {
  .name = "DebugTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal,
};
/* Definitions for VisionQueue */
osMessageQueueId_t VisionQueueHandle;
const osMessageQueueAttr_t VisionQueue_attributes = {
  .name = "VisionQueue"
};
/* Definitions for CommandQueue */
osMessageQueueId_t CommandQueueHandle;
const osMessageQueueAttr_t CommandQueue_attributes = {
  .name = "CommandQueue"
};
/* Definitions for VisionReadySem */
osSemaphoreId_t VisionReadySemHandle;
const osSemaphoreAttr_t VisionReadySem_attributes = {
  .name = "VisionReadySem"
};
/* Definitions for myBinarySem02 */
osSemaphoreId_t myBinarySem02Handle;
const osSemaphoreAttr_t myBinarySem02_attributes = {
  .name = "myBinarySem02"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartVisionTask(void *argument);
void StartMotionTask(void *argument);
void StartStateTask(void *argument);
void StartDebugTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of VisionReadySem */
  VisionReadySemHandle = osSemaphoreNew(1, 1, &VisionReadySem_attributes);

  /* creation of myBinarySem02 */
  myBinarySem02Handle = osSemaphoreNew(1, 1, &myBinarySem02_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of VisionQueue */
  VisionQueueHandle = osMessageQueueNew (5, 8, &VisionQueue_attributes);

  /* creation of CommandQueue */
  CommandQueueHandle = osMessageQueueNew (3, 8, &CommandQueue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of VisionTask */
  VisionTaskHandle = osThreadNew(StartVisionTask, NULL, &VisionTask_attributes);

  /* creation of MotionTask */
  MotionTaskHandle = osThreadNew(StartMotionTask, NULL, &MotionTask_attributes);

  /* creation of StateTask */
  StateTaskHandle = osThreadNew(StartStateTask, NULL, &StateTask_attributes);

  /* creation of DebugTask */
  DebugTaskHandle = osThreadNew(StartDebugTask, NULL, &DebugTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartVisionTask */
/**
  * @brief  Function implementing the VisionTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartVisionTask */
void StartVisionTask(void *argument)
{
  /* USER CODE BEGIN StartVisionTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartVisionTask */
}

/* USER CODE BEGIN Header_StartMotionTask */
/**
* @brief Function implementing the MotionTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartMotionTask */
void StartMotionTask(void *argument)
{
  /* USER CODE BEGIN StartMotionTask */
  /* 初始化电机 */
  Motor_Init();
  
  /* 设置四个电机以50%占空比正转 */
  Motor_SetSpeed(MOTOR_1, 500);  /* 500/1000 = 50%占空比 */
  Motor_SetSpeed(MOTOR_2, 500);
  Motor_SetSpeed(MOTOR_3, 500);
  Motor_SetSpeed(MOTOR_4, 500);
  
  /* Infinite loop */
  for(;;)
  {
    /* 处理编码器中断标志位 */
    if (encoder1_flag) {
      Encoder_Update(1);
      encoder1_flag = 0;
    }
    if (encoder2_flag) {
      Encoder_Update(2);
      encoder2_flag = 0;
    }
    if (encoder3_flag) {
      Encoder_Update(3);
      encoder3_flag = 0;
    }
    if (encoder4_flag) {
      Encoder_Update(4);
      encoder4_flag = 0;
    }
    
    /* 每100ms计算一次编码器速度 */
    Encoder_CalculateSpeed(1, 100);
    Encoder_CalculateSpeed(2, 100);
    Encoder_CalculateSpeed(3, 100);
    Encoder_CalculateSpeed(4, 100);
    
    osDelay(100);
  }
  /* USER CODE END StartMotionTask */
}

/* USER CODE BEGIN Header_StartStateTask */
/**
* @brief Function implementing the StateTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartStateTask */
void StartStateTask(void *argument)
{
  /* USER CODE BEGIN StartStateTask */
  /* Infinite loop */
  for(;;)
  {
    HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
    osDelay(1000);
  }
  /* USER CODE END StartStateTask */
}

/* USER CODE BEGIN Header_StartDebugTask */
/**
* @brief Function implementing the DebugTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartDebugTask */
void StartDebugTask(void *argument)
{
  /* USER CODE BEGIN StartDebugTask */
  char speed_buf[100];
  char adc_buf[50];
  float rpm1 = 0.0f, rpm2 = 0.0f, rpm3 = 0.0f, rpm4 = 0.0f;
  const float ENCODER_RES = 13.0;  /* 编码器线数 */
  static uint8_t rx_buf2[100] = {0};
  static uint8_t rx_buf3[100] = {0};
  static uint32_t rx_len2 = 0;
  static uint32_t rx_len3 = 0;
  uint32_t adc_value = 0;
  
  /* 发送测试信息 */
  HAL_UART_Transmit(&huart2, (uint8_t*)"USART2 Test Message\r\n", 20, HAL_MAX_DELAY);
  HAL_UART_Transmit(&huart3, (uint8_t*)"USART3 Test Message\r\n", 20, HAL_MAX_DELAY);
  
  /* 启动串口接收中断 */
  HAL_UART_Receive_IT(&huart2, rx_buf2, 1);
  HAL_UART_Receive_IT(&huart3, rx_buf3, 1);
  
  /* 启动ADC */
  HAL_ADC_Start(&hadc1);
  
  /* Infinite loop */
  for(;;)
  {
    /* 处理USART2接收数据 */
    if (usart2_rx_flag) {
      rx_len2 = HAL_UART_Receive(&huart2, rx_buf2, 100, 100);
      if (rx_len2 > 0) {
        HAL_UART_Transmit(&huart2, rx_buf2, rx_len2, HAL_MAX_DELAY);
      }
      usart2_rx_flag = 0;
    }
    
    /* 处理USART3接收数据 */
    if (usart3_rx_flag) {
      rx_len3 = HAL_UART_Receive(&huart3, rx_buf3, 100, 100);
      if (rx_len3 > 0) {
        HAL_UART_Transmit(&huart3, rx_buf3, rx_len3, HAL_MAX_DELAY);
      }
      usart3_rx_flag = 0;
    }
    
    /* 计算所有编码器转速 */
    Encoder_GetAllRPM(&rpm1, &rpm2, &rpm3, &rpm4, ENCODER_RES);
    
    /* 格式化输出 - 输出RPM的整数部分 */
    sprintf(speed_buf, "1:%d  2:%d  3:%d  4:%d\r\n", 
            (int)rpm1, (int)rpm2, (int)rpm3, (int)rpm4);
    
    /* 发送到串口3 */
    HAL_UART_Transmit(&huart3, (uint8_t*)speed_buf, strlen(speed_buf), HAL_MAX_DELAY);
    
    /* ADC采样 */
    HAL_ADC_Start(&hadc1);
    if (HAL_ADC_PollForConversion(&hadc1, 100) == HAL_OK) {
      adc_value = HAL_ADC_GetValue(&hadc1);
      sprintf(adc_buf, "ADC：%lu\r\n", adc_value);
      HAL_UART_Transmit(&huart3, (uint8_t*)adc_buf, strlen(adc_buf), HAL_MAX_DELAY);
    }
    
    osDelay(1000);  /* 每秒输出一次转速和ADC值 */
  }
  /* USER CODE END StartDebugTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

