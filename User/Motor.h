#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "tim.h"
#include "math.h"
#include "pid.h"
#include "my_uart.h"
#include "usart.h"
#include "gpio.h"
#include "KS103.h"
#include "MPU6050.h"

#define RELOADVALUE1 __HAL_TIM_GetAutoreload(&htim1) // 获取自动装载值
#define RELOADVALUE2 __HAL_TIM_GetAutoreload(&htim2) // arr = 20000
#define RELOADVALUE3 __HAL_TIM_GetAutoreload(&htim3)
#define RELOADVALUE4 __HAL_TIM_GetAutoreload(&htim8)

#define COUNTERNUM1 __HAL_TIM_GetCounter(&htim1) // 获取编码器定时器中的计数值
#define COUNTERNUM2 __HAL_TIM_GetCounter(&htim2) // crr
#define COUNTERNUM3 __HAL_TIM_GetCounter(&htim3)
#define COUNTERNUM4 __HAL_TIM_GetCounter(&htim8)

#define M1M2STBY(state) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, (GPIO_PinState)(state)) // 驱动板1STBY(PB2)
#define M3M4STBY(state) HAL_GPIO_WritePin(GPIOG, GPIO_PIN_8, (GPIO_PinState)(state)) // 驱动板2STBY(PG8)
#define M1AIN1(state) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, (GPIO_PinState)(state))   // 驱动板1AIN1(PC4)
#define M1AIN2(state) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, (GPIO_PinState)(state))   // 驱动板1AIN2(PC5)
#define M2BIN1(state) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, (GPIO_PinState)(state))   // 驱动板1BIN1(PC9)
#define M2BIN2(state) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, (GPIO_PinState)(state))   // 驱动板1BIN2(PC8)
#define M3AIN1(state) HAL_GPIO_WritePin(GPIOG, GPIO_PIN_10, (GPIO_PinState)(state))  // 驱动板2AIN1(PG10)
#define M3AIN2(state) HAL_GPIO_WritePin(GPIOG, GPIO_PIN_11, (GPIO_PinState)(state))  // 驱动板2AIN2(PG11)
#define M4BIN1(state) HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, (GPIO_PinState)(state))  // 驱动板2BIN1(PG13)
#define M4BIN2(state) HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, (GPIO_PinState)(state))  // 驱动板2BIN2(PG14)

#define YAW_PID &YAW, 8, 0, 0, 0, 160
#define VELOCITY_PID 2.7, 1.1, 0, 10000, 10000

#define READ(PIN) HAL_GPIO_ReadPin(PIN) // 读取GPIO

// 电机结构体
typedef struct _Motor
{
    int32_t lastAngle;      // 上次计数结束时转过的角度
    int32_t totalAngle;     // 总共转过的角度
    int16_t loopNum;        // 电机计数过零计数
    Data speed;             // 添加电机数据结构
    float targetSpeed;      // 添加设定的目标速度
    float speed_Record[20]; // 速度记录
    Pid pid;                // 添加电机对应PID
} Motor;

extern Motor motor[4];
extern Pid YAW;
extern float actyaw, targetYaw;

void Motor_Init(void);

#endif
