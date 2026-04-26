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
#include "pid.h"
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
int16_t g_motor_speed[4] = {0, 0, 0, 0};  /* 四个电机独立目标速度（脉冲/秒）*/

/* 串口指令缓冲区 */
uint8_t uart_rx_buf[3] = {0};  /* 存储接收到的指令：[0]=包头, [1]=指令, [2]=包尾 */
uint8_t uart_rx_index = 0;      /* 接收索引 */
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
  
  /* 初始化PID控制器 */
  PID_MotorInit();
  
  /* 实际速度（脉冲/秒）和PID输出 */
  int16_t actual_speed1, actual_speed2, actual_speed3, actual_speed4;
  int16_t pid_output1, pid_output2, pid_output3, pid_output4;
  int16_t target_speed1, target_speed2, target_speed3, target_speed4;
  
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
    
    /* 计算编码器速度（脉冲/秒）*/
    Encoder_CalculateSpeed(1, PID_CONTROL_PERIOD);
    Encoder_CalculateSpeed(2, PID_CONTROL_PERIOD);
    Encoder_CalculateSpeed(3, PID_CONTROL_PERIOD);
    Encoder_CalculateSpeed(4, PID_CONTROL_PERIOD);
    
    /* 获取实际速度（脉冲/秒）*/
    actual_speed1 = Encoder_GetSpeed(1);
    actual_speed2 = Encoder_GetSpeed(2);
    actual_speed3 = Encoder_GetSpeed(3);
    actual_speed4 = Encoder_GetSpeed(4);
    
    /* 获取各电机目标速度并限幅 */
    target_speed1 = g_motor_speed[0];
    target_speed2 = g_motor_speed[1];
    target_speed3 = g_motor_speed[2];
    target_speed4 = g_motor_speed[3];
    
    /* 限幅保护 */
    if (target_speed1 > MOTOR_MAX_SPEED_PPS) target_speed1 = MOTOR_MAX_SPEED_PPS;
    if (target_speed1 < -MOTOR_MAX_SPEED_PPS) target_speed1 = -MOTOR_MAX_SPEED_PPS;
    if (target_speed2 > MOTOR_MAX_SPEED_PPS) target_speed2 = MOTOR_MAX_SPEED_PPS;
    if (target_speed2 < -MOTOR_MAX_SPEED_PPS) target_speed2 = -MOTOR_MAX_SPEED_PPS;
    if (target_speed3 > MOTOR_MAX_SPEED_PPS) target_speed3 = MOTOR_MAX_SPEED_PPS;
    if (target_speed3 < -MOTOR_MAX_SPEED_PPS) target_speed3 = -MOTOR_MAX_SPEED_PPS;
    if (target_speed4 > MOTOR_MAX_SPEED_PPS) target_speed4 = MOTOR_MAX_SPEED_PPS;
    if (target_speed4 < -MOTOR_MAX_SPEED_PPS) target_speed4 = -MOTOR_MAX_SPEED_PPS;
    
    /* 执行速度PID闭环控制 */
    pid_output1 = PID_MotorSpeedControl(MOTOR_1, target_speed1, actual_speed1);
    pid_output2 = PID_MotorSpeedControl(MOTOR_2, target_speed2, actual_speed2);
    pid_output3 = PID_MotorSpeedControl(MOTOR_3, target_speed3, actual_speed3);
    pid_output4 = PID_MotorSpeedControl(MOTOR_4, target_speed4, actual_speed4);
    
    /* 将PID输出应用到电机 */
    Motor_SetSpeed(MOTOR_1, pid_output1);
    Motor_SetSpeed(MOTOR_2, pid_output2);
    Motor_SetSpeed(MOTOR_3, pid_output3);
    Motor_SetSpeed(MOTOR_4, pid_output4);
    
    osDelay(PID_CONTROL_PERIOD);  /* PID控制周期 */
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
/* 串口指令解析函数 */
void ParseUARTCommand(uint8_t cmd)
{
  int16_t speed = DEFAULT_SPEED;
  
  switch(cmd)
  {
    /* 运动控制指令 */
    case CMD_MOVE_FORWARD:
      /* 前进：电机1正转，电机2反转，电机3反转，电机4正转 */
      g_motor_speed[0] = speed;   /* 电机1正转 */
      g_motor_speed[1] = -speed;  /* 电机2反转 */
      g_motor_speed[2] = -speed;  /* 电机3反转 */
      g_motor_speed[3] = speed;   /* 电机4正转 */
      break;
      
    case CMD_MOVE_BACKWARD:
      /* 后退：电机1反转，电机2正转，电机3正转，电机4反转 */
      g_motor_speed[0] = -speed;  /* 电机1反转 */
      g_motor_speed[1] = speed;   /* 电机2正转 */
      g_motor_speed[2] = speed;   /* 电机3正转 */
      g_motor_speed[3] = -speed;  /* 电机4反转 */
      break;
      
    case CMD_MOVE_LEFT:
      /* 向左：电机1反转，电机2反转，电机3正转，电机4正转 */
      g_motor_speed[0] = -speed;  /* 电机1反转 */
      g_motor_speed[1] = -speed;  /* 电机2反转 */
      g_motor_speed[2] = speed;   /* 电机3正转 */
      g_motor_speed[3] = speed;   /* 电机4正转 */
      break;
      
    case CMD_MOVE_RIGHT:
      /* 向右：电机1正转，电机2正转，电机3反转，电机4反转 */
      g_motor_speed[0] = speed;   /* 电机1正转 */
      g_motor_speed[1] = speed;   /* 电机2正转 */
      g_motor_speed[2] = -speed;  /* 电机3反转 */
      g_motor_speed[3] = -speed;  /* 电机4反转 */
      break;
      
    case CMD_MOVE_STOP:
      /* 停止：所有电机速度设为0 */
      g_motor_speed[0] = 0;
      g_motor_speed[1] = 0;
      g_motor_speed[2] = 0;
      g_motor_speed[3] = 0;
      break;
      
    /* PID调参指令 - 直接修改PID参数 */
    case CMD_KP_INC:
      g_PidMotor1.Kp += 1.0f;
      g_PidMotor2.Kp += 1.0f;
      g_PidMotor3.Kp += 1.0f;
      g_PidMotor4.Kp += 1.0f;
      break;
      
    case CMD_KP_DEC:
      g_PidMotor1.Kp -= 1.0f;
      g_PidMotor2.Kp -= 1.0f;
      g_PidMotor3.Kp -= 1.0f;
      g_PidMotor4.Kp -= 1.0f;
      break;
      
    case CMD_KI_INC:
      g_PidMotor1.Ki += 0.1f;
      g_PidMotor2.Ki += 0.1f;
      g_PidMotor3.Ki += 0.1f;
      g_PidMotor4.Ki += 0.1f;
      break;
      
    case CMD_KI_DEC:
      g_PidMotor1.Ki -= 0.1f;
      g_PidMotor2.Ki -= 0.1f;
      g_PidMotor3.Ki -= 0.1f;
      g_PidMotor4.Ki -= 0.1f;
      break;
      
    case CMD_KD_INC:
      g_PidMotor1.Kd += 1.0f;
      g_PidMotor2.Kd += 1.0f;
      g_PidMotor3.Kd += 1.0f;
      g_PidMotor4.Kd += 1.0f;
      break;
      
    case CMD_KD_DEC:
      g_PidMotor1.Kd -= 1.0f;
      g_PidMotor2.Kd -= 1.0f;
      g_PidMotor3.Kd -= 1.0f;
      g_PidMotor4.Kd -= 1.0f;
      break;
      
    default:
      /* 未知指令，忽略 */
      break;
  }
}

void StartDebugTask(void *argument)
{
  /* USER CODE BEGIN StartDebugTask */
  char debug_buf[100];
  char adc_buf[50];
  int16_t act_speed1, act_speed2, act_speed3, act_speed4;
  int32_t encoder_count1, encoder_count2, encoder_count3, encoder_count4;
  static uint8_t rx_buf2[100] = {0};
  static uint8_t rx_buf3[100] = {0};
  static uint32_t rx_len2 = 0;
  static uint32_t rx_len3 = 0;
  uint32_t adc_value = 0;
  uint8_t i;
  
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
    
    /* 处理USART3接收数据（解析指令）*/
    if (usart3_rx_flag) {
      rx_len3 = HAL_UART_Receive(&huart3, rx_buf3, 100, 100);
      if (rx_len3 > 0) {
        /* 解析指令格式：FF（包头）+ 指令（1字节）+ FE（包尾）*/
        for (i = 0; i < rx_len3; i++) {
          if (uart_rx_index == 0 && rx_buf3[i] == CMD_HEAD) {
            /* 收到包头 */
            uart_rx_buf[0] = rx_buf3[i];
            uart_rx_index = 1;
          } else if (uart_rx_index == 1) {
            /* 收到指令 */
            uart_rx_buf[1] = rx_buf3[i];
            uart_rx_index = 2;
          } else if (uart_rx_index == 2 && rx_buf3[i] == CMD_TAIL) {
            /* 收到包尾，完成一条指令 */
            uart_rx_buf[2] = rx_buf3[i];
            ParseUARTCommand(uart_rx_buf[1]);  /* 解析指令 */
            uart_rx_index = 0;  /* 重置索引 */
          } else {
            /* 格式错误，重置 */
            uart_rx_index = 0;
          }
        }
      }
      usart3_rx_flag = 0;
    }
    
    /* 根据调试模式输出不同内容 */
    #if DEBUG_MODE == 1
      /* DEBUG_MODE=1: 速度PID模式，输出目标速度和实际速度（脉冲/秒）*/
      act_speed1 = Encoder_GetSpeed(1);
      act_speed2 = Encoder_GetSpeed(2);
      act_speed3 = Encoder_GetSpeed(3);
      act_speed4 = Encoder_GetSpeed(4);
      
      sprintf(debug_buf, "T:1:%4d 2:%4d 3:%4d 4:%4d | A:1:%4d 2:%4d 3:%4d 4:%4d pps\r\n", 
              g_motor_speed[0], g_motor_speed[1], g_motor_speed[2], g_motor_speed[3],
              act_speed1, act_speed2, act_speed3, act_speed4);
    #elif DEBUG_MODE == 0
      /* DEBUG_MODE=0: 编码器模式，输出累计脉冲数 */
      encoder_count1 = Encoder_GetCount(1);
      encoder_count2 = Encoder_GetCount(2);
      encoder_count3 = Encoder_GetCount(3);
      encoder_count4 = Encoder_GetCount(4);
      
      sprintf(debug_buf, "ENC: 1:%8ld 2:%8ld 3:%8ld 4:%8ld\r\n", 
              encoder_count1, encoder_count2, encoder_count3, encoder_count4);
    #endif
    
    /* 发送到串口3 */
    HAL_UART_Transmit(&huart3, (uint8_t*)debug_buf, strlen(debug_buf), HAL_MAX_DELAY);
    
    /* ADC采样 */
    HAL_ADC_Start(&hadc1);
    if (HAL_ADC_PollForConversion(&hadc1, 100) == HAL_OK) {
      adc_value = HAL_ADC_GetValue(&hadc1);
      sprintf(adc_buf, "ADC:%lu\r\n", adc_value);
      HAL_UART_Transmit(&huart3, (uint8_t*)adc_buf, strlen(adc_buf), HAL_MAX_DELAY);
    }
    
    osDelay(1000);  /* 每秒输出一次 */
  }
  /* USER CODE END StartDebugTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

