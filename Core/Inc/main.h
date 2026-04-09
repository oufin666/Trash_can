/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define ADC1_IN11_Pin GPIO_PIN_1
#define ADC1_IN11_GPIO_Port GPIOC
#define ENCODER_A2_Pin GPIO_PIN_0
#define ENCODER_A2_GPIO_Port GPIOA
#define ENCODER_B2_Pin GPIO_PIN_1
#define ENCODER_B2_GPIO_Port GPIOA
#define ENCODER_A3_Pin GPIO_PIN_6
#define ENCODER_A3_GPIO_Port GPIOA
#define ENCODER_B3_Pin GPIO_PIN_7
#define ENCODER_B3_GPIO_Port GPIOA
#define ENCODER_A1_Pin GPIO_PIN_9
#define ENCODER_A1_GPIO_Port GPIOE
#define ENCODER_B1_Pin GPIO_PIN_11
#define ENCODER_B1_GPIO_Port GPIOE
#define USART3_RX_Pin GPIO_PIN_11
#define USART3_RX_GPIO_Port GPIOB
#define USART3_TX_Pin GPIO_PIN_8
#define USART3_TX_GPIO_Port GPIOD
#define M4_BIN1_Pin GPIO_PIN_5
#define M4_BIN1_GPIO_Port GPIOG
#define M4_BIN2_Pin GPIO_PIN_6
#define M4_BIN2_GPIO_Port GPIOG
#define M1_PWM_Pin GPIO_PIN_6
#define M1_PWM_GPIO_Port GPIOC
#define M2_PWM_Pin GPIO_PIN_7
#define M2_PWM_GPIO_Port GPIOC
#define M3_PWM_Pin GPIO_PIN_8
#define M3_PWM_GPIO_Port GPIOC
#define M4_PWM_Pin GPIO_PIN_9
#define M4_PWM_GPIO_Port GPIOC
#define M3_AIN2_Pin GPIO_PIN_8
#define M3_AIN2_GPIO_Port GPIOA
#define M3_AIN1_Pin GPIO_PIN_10
#define M3_AIN1_GPIO_Port GPIOC
#define M2_BIN1_Pin GPIO_PIN_2
#define M2_BIN1_GPIO_Port GPIOD
#define M2_BIN2_Pin GPIO_PIN_3
#define M2_BIN2_GPIO_Port GPIOD
#define USART2_TX_Pin GPIO_PIN_5
#define USART2_TX_GPIO_Port GPIOD
#define USART2_RX_Pin GPIO_PIN_6
#define USART2_RX_GPIO_Port GPIOD
#define M1_AIN1_Pin GPIO_PIN_11
#define M1_AIN1_GPIO_Port GPIOG
#define M1_AIN2_Pin GPIO_PIN_15
#define M1_AIN2_GPIO_Port GPIOG
#define ENCODER_A4_Pin GPIO_PIN_6
#define ENCODER_A4_GPIO_Port GPIOB
#define ENCODER_B4_Pin GPIO_PIN_7
#define ENCODER_B4_GPIO_Port GPIOB
#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
