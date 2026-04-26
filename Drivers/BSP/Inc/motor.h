/**
  ******************************************************************************
  * @file    motor.h
  * @brief   电机驱动模块头文件 - 智能垃圾桶全向轮底盘驱动
  *          基于TB6612FNG驱动芯片，控制4个直流减速电机
  *          实现全向移动：前进/后退/左移/右移/旋转
  *
  * @note    本文件位于Drivers/BSP目录下，不会被CubeMX重新生成覆盖
  *          使用时需要在CubeMX中配置对应的GPIO和TIM8 PWM输出
  *
  * @author  OUYQ
  * @date    2026-03-25
  ******************************************************************************
  */

#ifndef __MOTOR_H
#define __MOTOR_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "main.h"

/* Exported types ------------------------------------------------------------*/

/**
  * @brief  电机方向枚举
  */
typedef enum {
    MOTOR_DIR_STOP  = 0,    /*!< 停止 */
    MOTOR_DIR_CW    = 1,    /*!< 正转 (顺时针) */
    MOTOR_DIR_CCW   = 2     /*!< 反转 (逆时针) */
} Motor_Dir_t;

/**
  * @brief  底盘运动方向枚举
  */
typedef enum {
    CHASSIS_STOP    = 0,    /*!< 停止 */
    CHASSIS_FORWARD = 1,    /*!< 前进 */
    CHASSIS_BACK    = 2,    /*!< 后退 */
    CHASSIS_LEFT    = 3,    /*!< 左移 */
    CHASSIS_RIGHT   = 4,    /*!< 右移 */
    CHASSIS_ROTATE_CW  = 5, /*!< 顺时针旋转 */
    CHASSIS_ROTATE_CCW = 6  /*!< 逆时针旋转 */
} Chassis_Dir_t;

/**
  * @brief  单个电机结构体
  */
typedef struct {
    GPIO_TypeDef *AIN1_Port;    /*!< 方向控制1 GPIO端口 */
    uint16_t      AIN1_Pin;     /*!< 方向控制1 GPIO引脚 */
    GPIO_TypeDef *AIN2_Port;    /*!< 方向控制2 GPIO端口 */
    uint16_t      AIN2_Pin;     /*!< 方向控制2 GPIO引脚 */
    TIM_HandleTypeDef *htim;    /*!< PWM定时器句柄 */
    uint32_t      Channel;      /*!< PWM通道 */
    int16_t       Speed;        /*!< 当前速度值 (-1000 ~ 1000) */
    Motor_Dir_t   Direction;    /*!< 当前方向 */
} Motor_t;

/**
  * @brief  编码器结构体
  * @note   用于存储编码器相关信息
  */
typedef struct {
    TIM_HandleTypeDef *htim;   /*!< 编码器使用的定时器 */
    int32_t       Count;       /*!< 编码器计数值 */
    int16_t       Speed;       /*!< 计算得到的速度值（编码器脉冲/秒）*/
} Encoder_t;

/**
  * @brief  底盘结构体（包含4个电机和4个编码器）
  */
typedef struct {
    Motor_t Motor1;         /*!< 电机1 - 左前 */
    Motor_t Motor2;         /*!< 电机2 - 右前 */
    Motor_t Motor3;         /*!< 电机3 - 左后 */
    Motor_t Motor4;         /*!< 电机4 - 右后 */
    Encoder_t Encoder1;     /*!< 编码器1 - 对应电机1 */
    Encoder_t Encoder2;     /*!< 编码器2 - 对应电机2 */
    Encoder_t Encoder3;     /*!< 编码器3 - 对应电机3 */
    Encoder_t Encoder4;     /*!< 编码器4 - 对应电机4 */
    int16_t Vx;             /*!< X方向速度分量 */
    int16_t Vy;             /*!< Y方向速度分量 */
    int16_t Omega;          /*!< 旋转角速度 */
} Chassis_t;

/**
  * @brief  视觉数据结构体
  * @note   用于存储从摄像头获取的目标信息
  */
typedef struct {
    int16_t x;      /*!< 目标X坐标 */
    int16_t y;      /*!< 目标Y坐标 */
    int16_t speed;  /*!< 目标速度 */
    uint8_t valid;  /*!< 数据有效性 */
} VisionData_t;

/**
  * @brief  运动指令结构体
  * @note   用于任务间传递运动控制指令
  */
typedef struct {
    int16_t vx;     /*!< X方向速度 */
    int16_t vy;     /*!< Y方向速度 */
    int16_t omega;  /*!< 角速度 */
    uint8_t mode;   /*!< 运动模式 */
} MotionCmd_t;

/* Exported constants --------------------------------------------------------*/

#define MOTOR_SPEED_MAX     1000    /*!< 电机最大速度（PWM占空比对应的速度值）*/
#define MOTOR_SPEED_MIN     (-1000) /*!< 电机最小速度 */
#define PWM_PERIOD          999     /*!< PWM周期值，对应CubeMX中TIM8的Counter Period */

/* 电机最大速度（脉冲/秒）- 需根据实际电机参数调整 */
/* 计算公式：MAX_SPEED_PPS = (电机额定RPM / 60) × 编码器PPR × 减速比 */
/* 示例：(300 / 60) × 52 × 30 = 7800 pps */
#define MOTOR_MAX_SPEED_PPS 7800    /*!< 电机最大速度（脉冲/秒）*/

/* 编码器参数 */
#define ENCODER_RESOLUTION  13      /*!< 编码器线数 */
#define ENCODER_PPR         (ENCODER_RESOLUTION * 4)  /*!< 编码器每转脉冲数（四倍频）*/

/* 电机减速比 */
#define MOTOR_GEAR_RATIO    30      /*!< 电机减速比 */

/* 车轮参数 */
#define WHEEL_DIAMETER      0.065f  /*!< 车轮直径（米），65mm */
#define WHEEL_CIRCUMFERENCE (WHEEL_DIAMETER * 3.1415926f)  /*!< 车轮周长（米）*/

/* PID控制周期（毫秒）*/
#define PID_CONTROL_PERIOD  10      /*!< PID控制周期，建议5-20ms */

/* 速度单位转换系数：脉冲/秒 -> 米/秒 */
#define SPEED_CONV_FACTOR   (WHEEL_CIRCUMFERENCE / (ENCODER_PPR * MOTOR_GEAR_RATIO))

/* 速度限幅（防止溢出）*/
#define MAX_SPEED_PPS       30000   /*!< 最大速度（脉冲/秒），小于int16_t最大值32767 */
#define MIN_SPEED_PPS       (-MAX_SPEED_PPS)

/* 调试输出模式选择（修改为单个宏定义）*/
/* DEBUG_MODE = 0: 输出编码器累计脉冲数 */
/* DEBUG_MODE = 1: 输出速度PID信息（目标速度、实际速度）*/
#define DEBUG_MODE           1       /*!< 当前调试模式，默认1（速度PID模式）*/

/* 串口指令定义 */
#define CMD_HEAD             0xFF    /*!< 指令包头 */
#define CMD_TAIL             0xFE    /*!< 指令包尾 */

/* 运动控制指令 */
#define CMD_MOVE_FORWARD     0x01    /*!< 前进 */
#define CMD_MOVE_BACKWARD    0x02    /*!< 后退 */
#define CMD_MOVE_LEFT        0x03    /*!< 向左移动 */
#define CMD_MOVE_RIGHT       0x04    /*!< 向右移动 */
#define CMD_MOVE_STOP        0x05    /*!< 停止 */

/* PID调参指令 */
#define CMD_KP_INC           0x11    /*!< KP +1 */
#define CMD_KP_DEC           0x12    /*!< KP -1 */
#define CMD_KI_INC           0x13    /*!< KI +0.1 */
#define CMD_KI_DEC           0x14    /*!< KI -0.1 */
#define CMD_KD_INC           0x15    /*!< KD +1 */
#define CMD_KD_DEC           0x16    /*!< KD -1 */

/* 默认移动速度 */
#define DEFAULT_SPEED        500     /*!< 默认移动速度（脉冲/秒）*/

/* 电机编号定义 */
#define MOTOR_1             0
#define MOTOR_2             1
#define MOTOR_3             2
#define MOTOR_4             3
#define MOTOR_ALL           0xFF

/* Exported macro ------------------------------------------------------------*/

/* 根据引脚分配宏定义（与CubeMX中设置的标签对应） */
#ifndef M1_AIN1_Pin
  #define M1_AIN1_Pin       GPIO_PIN_11
  #define M1_AIN1_GPIO_Port GPIOG
  #define M1_AIN2_Pin       GPIO_PIN_15
  #define M1_AIN2_GPIO_Port GPIOG
#endif

#ifndef M2_BIN1_Pin
  #define M2_BIN1_Pin       GPIO_PIN_2
  #define M2_BIN1_GPIO_Port GPIOD
  #define M2_BIN2_Pin       GPIO_PIN_3
  #define M2_BIN2_GPIO_Port GPIOD
#endif

#ifndef M3_AIN1_Pin
  #define M3_AIN1_Pin       GPIO_PIN_10
  #define M3_AIN1_GPIO_Port GPIOC
  #define M3_AIN2_Pin       GPIO_PIN_8
  #define M3_AIN2_GPIO_Port GPIOA
#endif

#ifndef M4_BIN1_Pin
  #define M4_BIN1_Pin       GPIO_PIN_5
  #define M4_BIN1_GPIO_Port GPIOG
  #define M4_BIN2_Pin       GPIO_PIN_6
  #define M4_BIN2_GPIO_Port GPIOG
#endif

/* Exported variables --------------------------------------------------------*/

extern Chassis_t g_Chassis;     /*!< 底盘全局实例 */
extern TIM_HandleTypeDef htim3; /*!< TIM3句柄声明（由CubeMX生成） */

/* Exported functions prototypes ---------------------------------------------*/

/* 初始化函数 */
void Motor_Init(void);

/* 单电机控制 */
void Motor_SetSpeed(uint8_t motor_id, int16_t speed);
void Motor_SetDirection(uint8_t motor_id, Motor_Dir_t dir);
void Motor_Stop(uint8_t motor_id);
void Motor_Brake(uint8_t motor_id);

/* 底盘运动控制 */
void Chassis_Move(int16_t vx, int16_t vy, int16_t omega);
void Chassis_MoveDirection(Chassis_Dir_t dir, int16_t speed);
void Chassis_Stop(void);
void Chassis_Brake(void);

/* 全向轮解算函数 */
void Chassis_CalculateMecanum(int16_t vx, int16_t vy, int16_t omega, 
                               int16_t *speed1, int16_t *speed2, 
                               int16_t *speed3, int16_t *speed4);

/* 速度限制函数 */
int16_t Motor_LimitSpeed(int16_t speed);

/* 编码器相关函数 */
int32_t Encoder_GetCount(uint8_t encoder);
int16_t Encoder_GetSpeed(uint8_t encoder);
float Encoder_GetSpeedMS(uint8_t encoder);  /* 获取速度（米/秒）*/
void Encoder_Update(uint8_t encoder);
void Encoder_CalculateSpeed(uint8_t encoder, uint16_t period);
float Encoder_GetRPM(uint8_t encoder, float encoder_res);
void Encoder_GetAllRPM(float *rpm1, float *rpm2, float *rpm3, float *rpm4, float encoder_res);

#ifdef __cplusplus
}
#endif

#endif /* __MOTOR_H */
