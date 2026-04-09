/**
  ******************************************************************************
  * @file    pid.h
  * @brief   PID控制器模块头文件 - 智能垃圾桶运动控制
  *          实现位置式PID和增量式PID，用于电机速度闭环控制
  *
  * @note    本文件位于Drivers/BSP目录下，不会被CubeMX重新生成覆盖
  *
  * @author  OUYQ
  * @date    2026-03-25
  ******************************************************************************
  */

#ifndef __PID_H
#define __PID_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Exported types ------------------------------------------------------------*/

/**
  * @brief  PID控制器结构体（位置式PID）
  */
typedef struct {
    float Kp;           /*!< 比例系数 */
    float Ki;           /*!< 积分系数 */
    float Kd;           /*!< 微分系数 */
    
    float Target;       /*!< 目标值 */
    float Actual;       /*!< 实际值 */
    float Error;        /*!< 当前误差 */
    float LastError;    /*!< 上一次误差 */
    float PrevError;    /*!< 上上次误差 */
    float Integral;     /*!< 积分累积 */
    float Output;       /*!< PID输出 */
    
    float MaxOutput;    /*!< 输出上限 */
    float MinOutput;    /*!< 输出下限 */
    float MaxIntegral;  /*!< 积分上限 */
    float MinIntegral;  /*!< 积分下限 */
    
    uint8_t FirstRun;   /*!< 首次运行标志 */
} PID_Position_t;

/**
  * @brief  PID控制器结构体（增量式PID）
  */
typedef struct {
    float Kp;           /*!< 比例系数 */
    float Ki;           /*!< 积分系数 */
    float Kd;           /*!< 微分系数 */
    
    float Target;       /*!< 目标值 */
    float Actual;       /*!< 实际值 */
    float Error;        /*!< 当前误差 */
    float LastError;    /*!< 上一次误差 */
    float PrevError;    /*!< 上上次误差 */
    float DeltaOutput;  /*!< 增量输出 */
    float Output;       /*!< 累计输出 */
    
    float MaxOutput;    /*!< 输出上限 */
    float MinOutput;    /*!< 输出下限 */
    
    uint8_t FirstRun;   /*!< 首次运行标志 */
} PID_Increment_t;

/* Exported constants --------------------------------------------------------*/

/* 默认PID参数 */
#define PID_DEFAULT_KP          1.0f
#define PID_DEFAULT_KI          0.0f
#define PID_DEFAULT_KD          0.0f

/* 默认输出限幅 */
#define PID_DEFAULT_MAX_OUTPUT  1000.0f
#define PID_DEFAULT_MIN_OUTPUT  (-1000.0f)
#define PID_DEFAULT_MAX_INT     500.0f
#define PID_DEFAULT_MIN_INT     (-500.0f)

/* 电机速度环PID参数（需要根据实际电机调试） */
#define MOTOR_PID_KP            5.0f
#define MOTOR_PID_KI            0.5f
#define MOTOR_PID_KD            1.0f

/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/

/* 4个电机的速度环PID控制器 */
extern PID_Position_t g_PidMotor1;
extern PID_Position_t g_PidMotor2;
extern PID_Position_t g_PidMotor3;
extern PID_Position_t g_PidMotor4;

/* Exported functions prototypes ---------------------------------------------*/

/* 位置式PID函数 */
void PID_Position_Init(PID_Position_t *pid, float kp, float ki, float kd);
void PID_Position_SetTarget(PID_Position_t *pid, float target);
void PID_Position_SetLimits(PID_Position_t *pid, float max_out, float min_out);
void PID_Position_SetIntegralLimits(PID_Position_t *pid, float max_int, float min_int);
float PID_Position_Calculate(PID_Position_t *pid, float actual);
void PID_Position_Reset(PID_Position_t *pid);

/* 增量式PID函数 */
void PID_Increment_Init(PID_Increment_t *pid, float kp, float ki, float kd);
void PID_Increment_SetTarget(PID_Increment_t *pid, float target);
void PID_Increment_SetLimits(PID_Increment_t *pid, float max_out, float min_out);
float PID_Increment_Calculate(PID_Increment_t *pid, float actual);
void PID_Increment_Reset(PID_Increment_t *pid);

/* 电机速度环控制函数 */
void PID_MotorInit(void);
int16_t PID_MotorSpeedControl(uint8_t motor_id, int16_t target_speed, int16_t actual_speed);

/* 参数调整函数 */
void PID_SetParams(PID_Position_t *pid, float kp, float ki, float kd);
void PID_GetParams(PID_Position_t *pid, float *kp, float *ki, float *kd);

#ifdef __cplusplus
}
#endif

#endif /* __PID_H */
