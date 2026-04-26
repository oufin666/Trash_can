/**
  ******************************************************************************
  * @file    motor.c
  * @brief   电机驱动模块实现 - 智能垃圾桶全向轮底盘驱动
  *          基于TB6612FNG驱动芯片，控制4个直流减速电机
  *
  * @note    本文件位于Drivers/BSP目录下，不会被CubeMX重新生成覆盖
  *          使用时需要在CubeMX中配置TIM8 PWM输出和对应GPIO
  *
  * @author  OUYQ
  * @date    2026-03-25
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "motor.h"
#include "tim.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/**
  * @brief  底盘全局实例
  */
Chassis_t g_Chassis;

/* Private function prototypes -----------------------------------------------*/
static void Motor_SetPWM(uint8_t motor_id, uint16_t pwm_value);

/* Private user code ---------------------------------------------------------*/

/**
  * @brief  设置电机PWM占空比
  * @param  motor_id: 电机编号 (MOTOR_1 ~ MOTOR_4)
  * @param  pwm_value: PWM值 (0 ~ PWM_PERIOD)
  * @retval None
  */
static void Motor_SetPWM(uint8_t motor_id, uint16_t pwm_value)
{
    if (pwm_value > PWM_PERIOD) {
        pwm_value = PWM_PERIOD;
    }
    
    switch (motor_id) {
        case MOTOR_1:
            __HAL_TIM_SET_COMPARE(g_Chassis.Motor1.htim, g_Chassis.Motor1.Channel, pwm_value);
            break;
        case MOTOR_2:
            __HAL_TIM_SET_COMPARE(g_Chassis.Motor2.htim, g_Chassis.Motor2.Channel, pwm_value);
            break;
        case MOTOR_3:
            __HAL_TIM_SET_COMPARE(g_Chassis.Motor3.htim, g_Chassis.Motor3.Channel, pwm_value);
            break;
        case MOTOR_4:
            __HAL_TIM_SET_COMPARE(g_Chassis.Motor4.htim, g_Chassis.Motor4.Channel, pwm_value);
            break;
        default:
            break;
    }
}

/* Exported functions --------------------------------------------------------*/

/**
  * @brief  电机驱动初始化
  * @note   需要在CubeMX配置完成后调用，初始化电机结构体
  * @retval None
  */
void Motor_Init(void)
{
    /* 电机1初始化 - 左前电机 */
    g_Chassis.Motor1.AIN1_Port = M1_AIN1_GPIO_Port;
    g_Chassis.Motor1.AIN1_Pin  = M1_AIN1_Pin;
    g_Chassis.Motor1.AIN2_Port = M1_AIN2_GPIO_Port;
    g_Chassis.Motor1.AIN2_Pin  = M1_AIN2_Pin;
    g_Chassis.Motor1.htim      = &htim8;
    g_Chassis.Motor1.Channel   = TIM_CHANNEL_1;
    g_Chassis.Motor1.Speed     = 0;
    g_Chassis.Motor1.Direction = MOTOR_DIR_STOP;
    
    /* 电机2初始化 - 右前电机 */
    g_Chassis.Motor2.AIN1_Port = M2_BIN1_GPIO_Port;
    g_Chassis.Motor2.AIN1_Pin  = M2_BIN1_Pin;
    g_Chassis.Motor2.AIN2_Port = M2_BIN2_GPIO_Port;
    g_Chassis.Motor2.AIN2_Pin  = M2_BIN2_Pin;
    g_Chassis.Motor2.htim      = &htim8;
    g_Chassis.Motor2.Channel   = TIM_CHANNEL_2;
    g_Chassis.Motor2.Speed     = 0;
    g_Chassis.Motor2.Direction = MOTOR_DIR_STOP;
    
    /* 电机3初始化 - 左后电机 */
    g_Chassis.Motor3.AIN1_Port = M3_AIN1_GPIO_Port;
    g_Chassis.Motor3.AIN1_Pin  = M3_AIN1_Pin;
    g_Chassis.Motor3.AIN2_Port = M3_AIN2_GPIO_Port;
    g_Chassis.Motor3.AIN2_Pin  = M3_AIN2_Pin;
    g_Chassis.Motor3.htim      = &htim8;
    g_Chassis.Motor3.Channel   = TIM_CHANNEL_3;
    g_Chassis.Motor3.Speed     = 0;
    g_Chassis.Motor3.Direction = MOTOR_DIR_STOP;
    
    /* 电机4初始化 - 右后电机 */
    g_Chassis.Motor4.AIN1_Port = M4_BIN1_GPIO_Port;
    g_Chassis.Motor4.AIN1_Pin  = M4_BIN1_Pin;
    g_Chassis.Motor4.AIN2_Port = M4_BIN2_GPIO_Port;
    g_Chassis.Motor4.AIN2_Pin  = M4_BIN2_Pin;
    g_Chassis.Motor4.htim      = &htim8;
    g_Chassis.Motor4.Channel   = TIM_CHANNEL_4;
    g_Chassis.Motor4.Speed     = 0;
    g_Chassis.Motor4.Direction = MOTOR_DIR_STOP;
    
    /* 初始化编码器1 - 使用TIM1 */
    g_Chassis.Encoder1.htim  = &htim1;
    g_Chassis.Encoder1.Count = 0;
    g_Chassis.Encoder1.Speed = 0;
    HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL);
    
    /* 初始化编码器2 - 使用TIM2 */
    g_Chassis.Encoder2.htim  = &htim2;
    g_Chassis.Encoder2.Count = 0;
    g_Chassis.Encoder2.Speed = 0;
    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
    
    /* 初始化编码器3 - 使用TIM3 */
    g_Chassis.Encoder3.htim  = &htim3;
    g_Chassis.Encoder3.Count = 0;
    g_Chassis.Encoder3.Speed = 0;
    HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
    
    /* 初始化编码器4 - 使用TIM4 */
    g_Chassis.Encoder4.htim  = &htim4;
    g_Chassis.Encoder4.Count = 0;
    g_Chassis.Encoder4.Speed = 0;
    HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);
    
    /* 初始化速度分量 */
    g_Chassis.Vx    = 0;
    g_Chassis.Vy    = 0;
    g_Chassis.Omega = 0;
    
    /* 初始状态：所有电机停止 */
    Chassis_Stop();
}

/**
  * @brief  设置单个电机方向
  * @param  motor_id: 电机编号 (MOTOR_1 ~ MOTOR_4)
  * @param  dir: 方向 (MOTOR_DIR_STOP/MOTOR_DIR_CW/MOTOR_DIR_CCW)
  * @retval None
  * 
  * @note   TB6612控制逻辑：
  *         AIN1=0, AIN2=0 -> 停止
  *         AIN1=1, AIN2=0 -> 正转
  *         AIN1=0, AIN2=1 -> 反转
  *         AIN1=1, AIN2=1 -> 刹车（短接制动）
  */
void Motor_SetDirection(uint8_t motor_id, Motor_Dir_t dir)
{
    Motor_t *pMotor = NULL;
    
    switch (motor_id) {
        case MOTOR_1: pMotor = &g_Chassis.Motor1; break;
        case MOTOR_2: pMotor = &g_Chassis.Motor2; break;
        case MOTOR_3: pMotor = &g_Chassis.Motor3; break;
        case MOTOR_4: pMotor = &g_Chassis.Motor4; break;
        default: return;
    }
    
    pMotor->Direction = dir;
    
    switch (dir) {
        case MOTOR_DIR_STOP:
            HAL_GPIO_WritePin(pMotor->AIN1_Port, pMotor->AIN1_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(pMotor->AIN2_Port, pMotor->AIN2_Pin, GPIO_PIN_RESET);
            break;
        case MOTOR_DIR_CW:
            HAL_GPIO_WritePin(pMotor->AIN1_Port, pMotor->AIN1_Pin, GPIO_PIN_SET);
            HAL_GPIO_WritePin(pMotor->AIN2_Port, pMotor->AIN2_Pin, GPIO_PIN_RESET);
            break;
        case MOTOR_DIR_CCW:
            HAL_GPIO_WritePin(pMotor->AIN1_Port, pMotor->AIN1_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(pMotor->AIN2_Port, pMotor->AIN2_Pin, GPIO_PIN_SET);
            break;
        default:
            break;
    }
}

/**
  * @brief  设置单个电机速度
  * @param  motor_id: 电机编号 (MOTOR_1 ~ MOTOR_4)
  * @param  speed: 速度值 (-1000 ~ 1000)
  *         正值为正转，负值为反转，0为停止
  * @retval None
  */
void Motor_SetSpeed(uint8_t motor_id, int16_t speed)
{
    Motor_t *pMotor = NULL;
    uint16_t pwm_value;
    
    switch (motor_id) {
        case MOTOR_1: pMotor = &g_Chassis.Motor1; break;
        case MOTOR_2: pMotor = &g_Chassis.Motor2; break;
        case MOTOR_3: pMotor = &g_Chassis.Motor3; break;
        case MOTOR_4: pMotor = &g_Chassis.Motor4; break;
        default: return;
    }
    
    /* 速度限幅 */
    speed = Motor_LimitSpeed(speed);
    pMotor->Speed = speed;
    
    if (speed > 0) {
        /* 正转 */
        Motor_SetDirection(motor_id, MOTOR_DIR_CW);
        pwm_value = (uint16_t)((speed * PWM_PERIOD) / MOTOR_SPEED_MAX);
    } else if (speed < 0) {
        /* 反转 */
        Motor_SetDirection(motor_id, MOTOR_DIR_CCW);
        pwm_value = (uint16_t)((-speed * PWM_PERIOD) / MOTOR_SPEED_MAX);
    } else {
        /* 停止 */
        Motor_SetDirection(motor_id, MOTOR_DIR_STOP);
        pwm_value = 0;
    }
    
    Motor_SetPWM(motor_id, pwm_value);
}

/**
  * @brief  电机停止（惯性滑行）
  * @param  motor_id: 电机编号 (MOTOR_1 ~ MOTOR_4 或 MOTOR_ALL)
  * @retval None
  */
void Motor_Stop(uint8_t motor_id)
{
    if (motor_id == MOTOR_ALL) {
        Motor_SetSpeed(MOTOR_1, 0);
        Motor_SetSpeed(MOTOR_2, 0);
        Motor_SetSpeed(MOTOR_3, 0);
        Motor_SetSpeed(MOTOR_4, 0);
    } else {
        Motor_SetSpeed(motor_id, 0);
    }
}

/**
  * @brief  电机刹车（短接制动）
  * @param  motor_id: 电机编号 (MOTOR_1 ~ MOTOR_4 或 MOTOR_ALL)
  * @retval None
  */
void Motor_Brake(uint8_t motor_id)
{
    Motor_t *pMotor = NULL;
    
    if (motor_id == MOTOR_ALL) {
        Motor_Brake(MOTOR_1);
        Motor_Brake(MOTOR_2);
        Motor_Brake(MOTOR_3);
        Motor_Brake(MOTOR_4);
        return;
    }
    
    switch (motor_id) {
        case MOTOR_1: pMotor = &g_Chassis.Motor1; break;
        case MOTOR_2: pMotor = &g_Chassis.Motor2; break;
        case MOTOR_3: pMotor = &g_Chassis.Motor3; break;
        case MOTOR_4: pMotor = &g_Chassis.Motor4; break;
        default: return;
    }
    
    /* TB6612刹车：两个控制脚都置高 */
    HAL_GPIO_WritePin(pMotor->AIN1_Port, pMotor->AIN1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(pMotor->AIN2_Port, pMotor->AIN2_Pin, GPIO_PIN_SET);
    Motor_SetPWM(motor_id, PWM_PERIOD);
    
    pMotor->Speed = 0;
    pMotor->Direction = MOTOR_DIR_STOP;
}

/**
  * @brief  速度限幅函数
  * @param  speed: 输入速度值
  * @retval 限幅后的速度值
  */
int16_t Motor_LimitSpeed(int16_t speed)
{
    if (speed > MOTOR_SPEED_MAX) {
        return MOTOR_SPEED_MAX;
    } else if (speed < MOTOR_SPEED_MIN) {
        return MOTOR_SPEED_MIN;
    }
    return speed;
}

/**
  * @brief  全向轮底盘运动解算（麦克纳姆轮）
  * @param  vx: X方向速度分量 (-1000 ~ 1000)
  * @param  vy: Y方向速度分量 (-1000 ~ 1000)
  * @param  omega: 旋转角速度 (-1000 ~ 1000)
  * @param  speed1: 电机1速度输出指针
  * @param  speed2: 电机2速度输出指针
  * @param  speed3: 电机3速度输出指针
  * @param  speed4: 电机4速度输出指针
  * @retval None
  * 
  * @note   全向轮速度解算公式（X型布局）：
  *         电机1(左前) =  Vx + Vy - Omega
  *         电机2(右前) = -Vx + Vy - Omega  
  *         电机3(左后) =  Vx - Vy - Omega
  *         电机4(右后) = -Vx - Vy - Omega
  */
void Chassis_CalculateMecanum(int16_t vx, int16_t vy, int16_t omega,
                               int16_t *speed1, int16_t *speed2,
                               int16_t *speed3, int16_t *speed4)
{
    int16_t s1, s2, s3, s4;
    int16_t max_speed;
    float scale;
    
    /* 麦克纳姆轮逆运动学解算 */
    s1 = vx + vy - omega;   /* 左前 */
    s2 = -vx + vy - omega;  /* 右前 */
    s3 = vx - vy - omega;   /* 左后 */
    s4 = -vx - vy - omega;  /* 右后 */
    
    /* 归一化处理，防止超过最大速度 */
    max_speed = s1;
    if (s2 > max_speed) max_speed = s2;
    if (s3 > max_speed) max_speed = s3;
    if (s4 > max_speed) max_speed = s4;
    if (-s1 > max_speed) max_speed = -s1;
    if (-s2 > max_speed) max_speed = -s2;
    if (-s3 > max_speed) max_speed = -s3;
    if (-s4 > max_speed) max_speed = -s4;
    
    if (max_speed > MOTOR_SPEED_MAX) {
        scale = (float)MOTOR_SPEED_MAX / max_speed;
        s1 = (int16_t)(s1 * scale);
        s2 = (int16_t)(s2 * scale);
        s3 = (int16_t)(s3 * scale);
        s4 = (int16_t)(s4 * scale);
    }
    
    *speed1 = s1;
    *speed2 = s2;
    *speed3 = s3;
    *speed4 = s4;
}

/**
  * @brief  底盘全向运动控制
  * @param  vx: X方向速度分量 (-1000 ~ 1000)，正为右，负为左
  * @param  vy: Y方向速度分量 (-1000 ~ 1000)，正为前，负为后
  * @param  omega: 旋转角速度 (-1000 ~ 1000)，正为顺时针，负为逆时针
  * @retval None
  */
void Chassis_Move(int16_t vx, int16_t vy, int16_t omega)
{
    int16_t speed1, speed2, speed3, speed4;
    
    /* 保存速度分量 */
    g_Chassis.Vx    = Motor_LimitSpeed(vx);
    g_Chassis.Vy    = Motor_LimitSpeed(vy);
    g_Chassis.Omega = Motor_LimitSpeed(omega);
    
    /* 解算各电机速度 */
    Chassis_CalculateMecanum(g_Chassis.Vx, g_Chassis.Vy, g_Chassis.Omega,
                              &speed1, &speed2, &speed3, &speed4);
    
    /* 设置各电机速度 */
    Motor_SetSpeed(MOTOR_1, speed1);
    Motor_SetSpeed(MOTOR_2, speed2);
    Motor_SetSpeed(MOTOR_3, speed3);
    Motor_SetSpeed(MOTOR_4, speed4);
}

/**
  * @brief  底盘按指定方向运动
  * @param  dir: 运动方向
  * @param  speed: 运动速度 (0 ~ 1000)
  * @retval None
  */
void Chassis_MoveDirection(Chassis_Dir_t dir, int16_t speed)
{
    speed = Motor_LimitSpeed(speed);
    
    switch (dir) {
        case CHASSIS_STOP:
            Chassis_Stop();
            break;
        case CHASSIS_FORWARD:
            Chassis_Move(0, speed, 0);
            break;
        case CHASSIS_BACK:
            Chassis_Move(0, -speed, 0);
            break;
        case CHASSIS_LEFT:
            Chassis_Move(-speed, 0, 0);
            break;
        case CHASSIS_RIGHT:
            Chassis_Move(speed, 0, 0);
            break;
        case CHASSIS_ROTATE_CW:
            Chassis_Move(0, 0, speed);
            break;
        case CHASSIS_ROTATE_CCW:
            Chassis_Move(0, 0, -speed);
            break;
        default:
            Chassis_Stop();
            break;
    }
}

/**
  * @brief  底盘停止（惯性滑行）
  * @retval None
  */
void Chassis_Stop(void)
{
    g_Chassis.Vx    = 0;
    g_Chassis.Vy    = 0;
    g_Chassis.Omega = 0;
    
    Motor_Stop(MOTOR_ALL);
}

/**
  * @brief  底盘刹车（短接制动）
  * @retval None
  */
void Chassis_Brake(void)
{
    g_Chassis.Vx    = 0;
    g_Chassis.Vy    = 0;
    g_Chassis.Omega = 0;
    
    Motor_Brake(MOTOR_ALL);
}

/**
  * @brief  读取单个编码器值
  * @param  encoder: 编码器索引 (1-4)
  * @retval 编码器计数值
  */
int32_t Encoder_GetCount(uint8_t encoder)
{
    switch(encoder)
    {
        case 1:
            return (int32_t)__HAL_TIM_GET_COUNTER(g_Chassis.Encoder1.htim);
        case 2:
            return (int32_t)__HAL_TIM_GET_COUNTER(g_Chassis.Encoder2.htim);
        case 3:
            return (int32_t)__HAL_TIM_GET_COUNTER(g_Chassis.Encoder3.htim);
        case 4:
            return (int32_t)__HAL_TIM_GET_COUNTER(g_Chassis.Encoder4.htim);
        default:
            return 0;
    }
}

/**
  * @brief  读取单个编码器速度
  * @param  encoder: 编码器索引 (1-4)
  * @retval 编码器速度值（脉冲/秒）
  */
int16_t Encoder_GetSpeed(uint8_t encoder)
{
    switch(encoder)
    {
        case 1:
            return g_Chassis.Encoder1.Speed;
        case 2:
            return g_Chassis.Encoder2.Speed;
        case 3:
            return g_Chassis.Encoder3.Speed;
        case 4:
            return g_Chassis.Encoder4.Speed;
        default:
            return 0;
    }
}

/**
  * @brief  获取编码器速度（米/秒）
  * @param  encoder: 编码器索引 (1-4)
  * @retval 速度值（米/秒）
  */
float Encoder_GetSpeedMS(uint8_t encoder)
{
    int16_t speed = Encoder_GetSpeed(encoder);
    return (float)speed * SPEED_CONV_FACTOR;
}

/**
  * @brief  更新编码器计数值（从定时器读取）
  * @param  encoder: 编码器索引 (1-4)
  * @retval None
  */
void Encoder_Update(uint8_t encoder)
{
    switch(encoder)
    {
        case 1:
            g_Chassis.Encoder1.Count = (int32_t)__HAL_TIM_GET_COUNTER(g_Chassis.Encoder1.htim);
            break;
        case 2:
            g_Chassis.Encoder2.Count = (int32_t)__HAL_TIM_GET_COUNTER(g_Chassis.Encoder2.htim);
            break;
        case 3:
            g_Chassis.Encoder3.Count = (int32_t)__HAL_TIM_GET_COUNTER(g_Chassis.Encoder3.htim);
            break;
        case 4:
            g_Chassis.Encoder4.Count = (int32_t)__HAL_TIM_GET_COUNTER(g_Chassis.Encoder4.htim);
            break;
    }
}

/**
  * @brief  计算编码器速度
  * @param  encoder: 编码器索引 (1-4)
  * @param  period: 计算周期 (ms)
  * @retval None
  * 
  * @note   速度计算公式：
  *         速度 = (脉冲变化量 * 1000) / 周期(ms) -> 脉冲/秒
  */
void Encoder_CalculateSpeed(uint8_t encoder, uint16_t period)
{
    static int32_t last_count[4] = {0, 0, 0, 0};
    int32_t current_count, delta;
    int32_t speed;
    
    switch(encoder)
    {
        case 1:
            current_count = (int32_t)__HAL_TIM_GET_COUNTER(g_Chassis.Encoder1.htim);
            delta = current_count - last_count[0];
            speed = (delta * 1000) / period;
            /* 限幅保护 */
            if (speed > MAX_SPEED_PPS) speed = MAX_SPEED_PPS;
            if (speed < MIN_SPEED_PPS) speed = MIN_SPEED_PPS;
            g_Chassis.Encoder1.Speed = (int16_t)speed;
            last_count[0] = current_count;
            break;
        
        case 2:
            current_count = (int32_t)__HAL_TIM_GET_COUNTER(g_Chassis.Encoder2.htim);
            delta = current_count - last_count[1];
            speed = (delta * 1000) / period;
            if (speed > MAX_SPEED_PPS) speed = MAX_SPEED_PPS;
            if (speed < MIN_SPEED_PPS) speed = MIN_SPEED_PPS;
            g_Chassis.Encoder2.Speed = (int16_t)speed;
            last_count[1] = current_count;
            break;
        
        case 3:
            current_count = (int32_t)__HAL_TIM_GET_COUNTER(g_Chassis.Encoder3.htim);
            delta = current_count - last_count[2];
            speed = (delta * 1000) / period;
            if (speed > MAX_SPEED_PPS) speed = MAX_SPEED_PPS;
            if (speed < MIN_SPEED_PPS) speed = MIN_SPEED_PPS;
            g_Chassis.Encoder3.Speed = (int16_t)speed;
            last_count[2] = current_count;
            break;
        
        case 4:
            current_count = (int32_t)__HAL_TIM_GET_COUNTER(g_Chassis.Encoder4.htim);
            delta = current_count - last_count[3];
            speed = (delta * 1000) / period;
            if (speed > MAX_SPEED_PPS) speed = MAX_SPEED_PPS;
            if (speed < MIN_SPEED_PPS) speed = MIN_SPEED_PPS;
            g_Chassis.Encoder4.Speed = (int16_t)speed;
            last_count[3] = current_count;
            break;
    }
}

/**
  * @brief  计算单个编码器的转速（RPM）
  * @param  encoder: 编码器索引 (1-4)
  * @param  encoder_res: 编码器线数
  * @retval 转速值（RPM）
  */
float Encoder_GetRPM(uint8_t encoder, float encoder_res)
{
    int16_t speed = Encoder_GetSpeed(encoder);
    /* 转换为RPM：速度 = 脉冲/秒，RPM = (速度 / 编码器线数) * 60 */
    return (speed / encoder_res) * 60.0;
}

/**
  * @brief  计算所有编码器的转速（RPM）
  * @param  rpm1: 电机1转速输出指针
  * @param  rpm2: 电机2转速输出指针
  * @param  rpm3: 电机3转速输出指针
  * @param  rpm4: 电机4转速输出指针
  * @param  encoder_res: 编码器线数
  * @retval None
  */
void Encoder_GetAllRPM(float *rpm1, float *rpm2, float *rpm3, float *rpm4, float encoder_res)
{
    *rpm1 = Encoder_GetRPM(1, encoder_res);
    *rpm2 = Encoder_GetRPM(2, encoder_res);
    *rpm3 = Encoder_GetRPM(3, encoder_res);
    *rpm4 = Encoder_GetRPM(4, encoder_res);
}

/* USER CODE END 1 */
