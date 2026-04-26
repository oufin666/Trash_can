/**
  ******************************************************************************
  * @file    pid.c
  * @brief   PID控制器模块实现 - 智能垃圾桶运动控制
  *          实现位置式PID和增量式PID算法
  *
  * @note    本文件位于Drivers/BSP目录下，不会被CubeMX重新生成覆盖
  *
  * @author  OUYQ
  * @date    2026-03-25
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "pid.h"
#include <string.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* 电机编号定义（与motor.h保持一致） */
#define MOTOR_1             0
#define MOTOR_2             1
#define MOTOR_3             2
#define MOTOR_4             3

/* Private macro -------------------------------------------------------------*/

/* 限幅宏 */
#define LIMIT(x, max, min)  (((x) > (max)) ? (max) : (((x) < (min)) ? (min) : (x)))

/* Private variables ---------------------------------------------------------*/

/* 4个电机的速度环PID控制器实例 */
PID_Position_t g_PidMotor1;
PID_Position_t g_PidMotor2;
PID_Position_t g_PidMotor3;
PID_Position_t g_PidMotor4;

/* Private function prototypes -----------------------------------------------*/
/* Private user code ---------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/**
  * @brief  位置式PID初始化
  * @param  pid: PID结构体指针
  * @param  kp: 比例系数
  * @param  ki: 积分系数
  * @param  kd: 微分系数
  * @retval None
  */
void PID_Position_Init(PID_Position_t *pid, float kp, float ki, float kd)
{
    if (pid == NULL) return;
    
    /* 清零结构体 */
    memset(pid, 0, sizeof(PID_Position_t));
    
    /* 设置PID参数 */
    pid->Kp = kp;
    pid->Ki = ki;
    pid->Kd = kd;
    
    /* 设置默认限幅 */
    pid->MaxOutput = PID_DEFAULT_MAX_OUTPUT;
    pid->MinOutput = PID_DEFAULT_MIN_OUTPUT;
    pid->MaxIntegral = PID_DEFAULT_MAX_INT;
    pid->MinIntegral = PID_DEFAULT_MIN_INT;
    
    /* 设置首次运行标志 */
    pid->FirstRun = 1;
}

/**
  * @brief  设置位置式PID目标值
  * @param  pid: PID结构体指针
  * @param  target: 目标值
  * @retval None
  */
void PID_Position_SetTarget(PID_Position_t *pid, float target)
{
    if (pid == NULL) return;
    pid->Target = target;
}

/**
  * @brief  设置位置式PID输出限幅
  * @param  pid: PID结构体指针
  * @param  max_out: 输出上限
  * @param  min_out: 输出下限
  * @retval None
  */
void PID_Position_SetLimits(PID_Position_t *pid, float max_out, float min_out)
{
    if (pid == NULL) return;
    pid->MaxOutput = max_out;
    pid->MinOutput = min_out;
}

/**
  * @brief  设置位置式PID积分限幅
  * @param  pid: PID结构体指针
  * @param  max_int: 积分上限
  * @param  min_int: 积分下限
  * @retval None
  */
void PID_Position_SetIntegralLimits(PID_Position_t *pid, float max_int, float min_int)
{
    if (pid == NULL) return;
    pid->MaxIntegral = max_int;
    pid->MinIntegral = min_int;
}

/**
  * @brief  位置式PID计算
  * @param  pid: PID结构体指针
  * @param  actual: 实际值
  * @retval PID计算输出值
  * 
  * @note   位置式PID公式：
  *         Error = Target - Actual
  *         Integral += Error
  *         Output = Kp*Error + Ki*Integral + Kd*(Error - LastError)
  */
float PID_Position_Calculate(PID_Position_t *pid, float actual)
{
    float p_term, i_term, d_term;
    
    if (pid == NULL) return 0.0f;
    
    /* 保存实际值 */
    pid->Actual = actual;
    
    /* 计算误差 */
    pid->Error = pid->Target - pid->Actual;
    
    /* 首次运行处理 */
    if (pid->FirstRun) {
        pid->LastError = pid->Error;
        pid->FirstRun = 0;
    }
    
    /* 计算积分项（带限幅） */
    pid->Integral += pid->Error;
    pid->Integral = LIMIT(pid->Integral, pid->MaxIntegral, pid->MinIntegral);
    
    /* 计算各项 */
    p_term = pid->Kp * pid->Error;
    i_term = pid->Ki * pid->Integral;
    d_term = pid->Kd * (pid->Error - pid->LastError);
    
    /* 计算输出 */
    pid->Output = p_term + i_term + d_term;
    
    /* 输出限幅 */
    pid->Output = LIMIT(pid->Output, pid->MaxOutput, pid->MinOutput);
    
    /* 保存当前误差 */
    pid->LastError = pid->Error;
    
    return pid->Output;
}

/**
  * @brief  位置式PID复位
  * @param  pid: PID结构体指针
  * @retval None
  */
void PID_Position_Reset(PID_Position_t *pid)
{
    if (pid == NULL) return;
    
    pid->Error = 0.0f;
    pid->LastError = 0.0f;
    pid->PrevError = 0.0f;
    pid->Integral = 0.0f;
    pid->Output = 0.0f;
    pid->FirstRun = 1;
}

/**
  * @brief  增量式PID初始化
  * @param  pid: PID结构体指针
  * @param  kp: 比例系数
  * @param  ki: 积分系数
  * @param  kd: 微分系数
  * @retval None
  */
void PID_Increment_Init(PID_Increment_t *pid, float kp, float ki, float kd)
{
    if (pid == NULL) return;
    
    /* 清零结构体 */
    memset(pid, 0, sizeof(PID_Increment_t));
    
    /* 设置PID参数 */
    pid->Kp = kp;
    pid->Ki = ki;
    pid->Kd = kd;
    
    /* 设置默认限幅 */
    pid->MaxOutput = PID_DEFAULT_MAX_OUTPUT;
    pid->MinOutput = PID_DEFAULT_MIN_OUTPUT;
    
    /* 设置首次运行标志 */
    pid->FirstRun = 1;
}

/**
  * @brief  设置增量式PID目标值
  * @param  pid: PID结构体指针
  * @param  target: 目标值
  * @retval None
  */
void PID_Increment_SetTarget(PID_Increment_t *pid, float target)
{
    if (pid == NULL) return;
    pid->Target = target;
}

/**
  * @brief  设置增量式PID输出限幅
  * @param  pid: PID结构体指针
  * @param  max_out: 输出上限
  * @param  min_out: 输出下限
  * @retval None
  */
void PID_Increment_SetLimits(PID_Increment_t *pid, float max_out, float min_out)
{
    if (pid == NULL) return;
    pid->MaxOutput = max_out;
    pid->MinOutput = min_out;
}

/**
  * @brief  增量式PID计算
  * @param  pid: PID结构体指针
  * @param  actual: 实际值
  * @retval PID计算输出值
  * 
  * @note   增量式PID公式：
  *         Error = Target - Actual
  *         DeltaOutput = Kp*(Error - LastError) + Ki*Error + Kd*(Error - 2*LastError + PrevError)
  *         Output += DeltaOutput
  */
float PID_Increment_Calculate(PID_Increment_t *pid, float actual)
{
    float p_term, i_term, d_term;
    
    if (pid == NULL) return 0.0f;
    
    /* 保存实际值 */
    pid->Actual = actual;
    
    /* 计算误差 */
    pid->Error = pid->Target - pid->Actual;
    
    /* 首次运行处理 */
    if (pid->FirstRun) {
        pid->LastError = pid->Error;
        pid->PrevError = pid->Error;
        pid->FirstRun = 0;
    }
    
    /* 计算各项 */
    p_term = pid->Kp * (pid->Error - pid->LastError);
    i_term = pid->Ki * pid->Error;
    d_term = pid->Kd * (pid->Error - 2.0f * pid->LastError + pid->PrevError);
    
    /* 计算增量输出 */
    pid->DeltaOutput = p_term + i_term + d_term;
    
    /* 累加输出 */
    pid->Output += pid->DeltaOutput;
    
    /* 输出限幅 */
    pid->Output = LIMIT(pid->Output, pid->MaxOutput, pid->MinOutput);
    
    /* 保存误差历史 */
    pid->PrevError = pid->LastError;
    pid->LastError = pid->Error;
    
    return pid->Output;
}

/**
  * @brief  增量式PID复位
  * @param  pid: PID结构体指针
  * @retval None
  */
void PID_Increment_Reset(PID_Increment_t *pid)
{
    if (pid == NULL) return;
    
    pid->Error = 0.0f;
    pid->LastError = 0.0f;
    pid->PrevError = 0.0f;
    pid->DeltaOutput = 0.0f;
    pid->Output = 0.0f;
    pid->FirstRun = 1;
}

/**
  * @brief  电机PID控制器初始化
  * @retval None
  * @note   初始化4个电机的速度环PID参数
  */
void PID_MotorInit(void)
{
    /* 初始化电机1速度环PID */
    PID_Position_Init(&g_PidMotor1, MOTOR_PID_KP, MOTOR_PID_KI, MOTOR_PID_KD);
    PID_Position_SetLimits(&g_PidMotor1, 1000.0f, -1000.0f);
    PID_Position_SetIntegralLimits(&g_PidMotor1, 300.0f, -300.0f);
    
    /* 初始化电机2速度环PID */
    PID_Position_Init(&g_PidMotor2, MOTOR_PID_KP, MOTOR_PID_KI, MOTOR_PID_KD);
    PID_Position_SetLimits(&g_PidMotor2, 1000.0f, -1000.0f);
    PID_Position_SetIntegralLimits(&g_PidMotor2, 300.0f, -300.0f);
    
    /* 初始化电机3速度环PID */
    PID_Position_Init(&g_PidMotor3, MOTOR_PID_KP, MOTOR_PID_KI, MOTOR_PID_KD);
    PID_Position_SetLimits(&g_PidMotor3, 1000.0f, -1000.0f);
    PID_Position_SetIntegralLimits(&g_PidMotor3, 300.0f, -300.0f);
    
    /* 初始化电机4速度环PID */
    PID_Position_Init(&g_PidMotor4, MOTOR_PID_KP, MOTOR_PID_KI, MOTOR_PID_KD);
    PID_Position_SetLimits(&g_PidMotor4, 1000.0f, -1000.0f);
    PID_Position_SetIntegralLimits(&g_PidMotor4, 300.0f, -300.0f);
}

/**
  * @brief  电机速度闭环控制（带前馈）
  * @param  motor_id: 电机编号 (0~3)
  * @param  target_speed: 目标速度（脉冲/秒）
  * @param  actual_speed: 实际速度（脉冲/秒）
  * @retval 电机控制量 (-1000 ~ 1000)
  * 
  * @note   需要在定时器中断中周期性调用，建议控制周期为5~10ms
  * @note   输出 = 前馈项 + PID校正项
  *         前馈项 = 目标速度 / 最大速度 * 最大输出
  *         PID校正项 = 误差的校正（消除偏差）
  */
int16_t PID_MotorSpeedControl(uint8_t motor_id, int16_t target_speed, int16_t actual_speed)
{
    float pid_output;
    float feedforward;
    float total_output;
    PID_Position_t *pid = NULL;
    
    /* 选择对应的PID控制器 */
    switch (motor_id) {
        case MOTOR_1:
            pid = &g_PidMotor1;
            break;
        case MOTOR_2:
            pid = &g_PidMotor2;
            break;
        case MOTOR_3:
            pid = &g_PidMotor3;
            break;
        case MOTOR_4:
            pid = &g_PidMotor4;
            break;
        default:
            return 0;
    }
    
    /* 计算前馈项：基础PWM = 目标速度 / 最大速度 * 最大输出 */
    if (MOTOR_MAX_SPEED_PPS != 0) {
        feedforward = ((float)target_speed / (float)MOTOR_MAX_SPEED_PPS) * 1000.0f;
    } else {
        feedforward = 0.0f;
    }
    
    /* 设置PID目标为0（PID只计算误差校正）*/
    PID_Position_SetTarget(pid, 0.0f);
    
    /* 计算误差：误差 = 目标速度 - 实际速度 */
    float error = (float)(target_speed - actual_speed);
    
    /* 执行PID计算（计算校正项）*/
    pid_output = PID_Position_Calculate(pid, -error);
    
    /* 总输出 = 前馈项 + PID校正项 */
    total_output = feedforward + pid_output;
    
    /* 输出限幅 */
    if (total_output > 1000.0f) total_output = 1000.0f;
    if (total_output < -1000.0f) total_output = -1000.0f;
    
    /* 返回整型控制量 */
    return (int16_t)total_output;
}

/**
  * @brief  设置PID参数
  * @param  pid: PID结构体指针
  * @param  kp: 比例系数
  * @param  ki: 积分系数
  * @param  kd: 微分系数
  * @retval None
  */
void PID_SetParams(PID_Position_t *pid, float kp, float ki, float kd)
{
    if (pid == NULL) return;
    
    pid->Kp = kp;
    pid->Ki = ki;
    pid->Kd = kd;
}

/**
  * @brief  获取PID参数
  * @param  pid: PID结构体指针
  * @param  kp: 比例系数指针（输出）
  * @param  ki: 积分系数指针（输出）
  * @param  kd: 微分系数指针（输出）
  * @retval None
  */
void PID_GetParams(PID_Position_t *pid, float *kp, float *ki, float *kd)
{
    if (pid == NULL) return;
    
    if (kp != NULL) *kp = pid->Kp;
    if (ki != NULL) *ki = pid->Ki;
    if (kd != NULL) *kd = pid->Kd;
}
