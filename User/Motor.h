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

#define STOP 0, 0, 0, 0 // 停止

#define X 1 // 直行
#define Y 2 // 横移

#define VorD_YAW &YAW, 5, 0.12, 0, 5, 100
#define A_YAW &YAW, 8, 0, 0, 0, 160
#define VELOCITY_PID 2.7, 1.1, 0, 10000, 10000
#define STOP_PID &YAW, 0, 0, 0, 0, 0

#define L2 GPIOF, GPIO_PIN_11 // L2 PF11
#define L1 GPIOD, GPIO_PIN_10 // L1 PD10
#define M GPIOD, GPIO_PIN_9   // M  PD9
#define R1 GPIOD, GPIO_PIN_8  // R1 PD8
#define R2 GPIOE, GPIO_PIN_14 // R2 PE14
#define S1 GPIOB, GPIO_PIN_5  // 光电1
#define S2 GPIOB, GPIO_PIN_6  // 光电2
#define S3 GPIOB, GPIO_PIN_7  // 光电3
#define SMP GPIOD, GPIO_PIN_2 // 树莓派

#define READ(PIN) HAL_GPIO_ReadPin(PIN)                                 // 读取GPIO
#define SET_SMP HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_SET)     // 输出正
#define RESET_SMP HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET) // 输出负

// 电机结构体
typedef struct _Motor
{
    int32_t lastAngle;      // 上次计数结束时转过的角度
    int32_t totalAngle;     // 总共转过的角度
    int16_t loopNum;        // 电机计数过零计数
    float speed, espeed;    // 电机输出轴速度
    float targetSpeed;      // 添加设定的目标速度
    float speed_Record[20]; // 速度记录
    Pid pid;                // 添加电机对应PID
} Motor;

extern int8_t state, state1, state2, state3, smp;
extern Motor motor[4];
extern Pid YAW, DISTANCE;
extern float distance, actyaw, targetAngle, actDistance, expectDistance, targetDistance, calDistance, value;
extern uint8_t mode;

void Init(void);
void Move(int x, int y, int z, uint8_t inputMode);

#endif
