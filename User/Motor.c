#include "Motor.h"

Pid YAW;                     // 偏航校正pid结构体
Motor motor[4];              // 电机结构体
uint8_t time = 0;            // printf
int Vx = 90, Vy, Vz;         // 设定直行速度
int turns = 0;               // 转圈次数
float pitch, roll, yaw;      // 角度
float actYaw, targetYaw = 0; // 角度目标值
int t=0;
void DataPrint(void)
{
    time++;
    if (time % 5 == 0)
    {
        OLED_Show_Float(60, 1, (float)AD_Buf[0], 16);
        OLED_Show_Float(0, 3, motor[0].speed.value, 16);
        OLED_Show_Float(0, 1, 0, 16);
        // OLED_Show_Float(0, 1, pitch, 16);
        // OLED_Show_Float(0, 3, roll, 16);
        OLED_Show_Float(0, 5, yaw, 16);
        time = 0;
    }
}
void PID_Init_Motor(float kp, float ki, float kd, int maxi, int maxoutput)
{
    for (int8_t i = 0; i < 4; i++)
    {
        PID_Init(&motor[i].pid, kp, ki, kd, maxi, maxoutput);
    }
}
void Motor_Init(void)
{
    motor[0].speed.limitedValue = 200;
    motor[1].speed.limitedValue = 200;
    motor[2].speed.limitedValue = 200;
    motor[3].speed.limitedValue = 200;
    Motor_TIM_Init();
    PID_Init_Motor(VELOCITY_PID);
    PID_Init(YAW_PID);
    HAL_Delay(1000); // 等待传感器初始化完成
}
// 电机速度更新
void Motor1_Send(void) // motor1 速度更新指令
{
    float output = 0;
    PID_SingleCalc(&motor[0].pid, motor[0].targetSpeed, motor[0].speed.value);
    output = motor[0].pid.output;
    if (output > 0) // 对应正转
    {
        __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_1, (uint32_t)output);
        M1AIN1(GPIO_PIN_RESET);
        M1AIN2(GPIO_PIN_SET);
    }
    else // 对应反转
    {
        __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_1, (uint32_t)(-output));
        M1AIN1(GPIO_PIN_SET);
        M1AIN2(GPIO_PIN_RESET);
    }
}
void Motor2_Send(void) // motor2 速度更新指令
{
    float output = 0;
    PID_SingleCalc(&motor[1].pid, motor[1].targetSpeed, motor[1].speed.value);
    output = motor[1].pid.output;
    if (output > 0) // 对应正转
    {
        __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_2, (uint32_t)output);
        M2BIN1(GPIO_PIN_SET);
        M2BIN2(GPIO_PIN_RESET);
    }
    else // 对应反转
    {
        __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_2, (uint32_t)(-output));
        M2BIN1(GPIO_PIN_RESET);
        M2BIN2(GPIO_PIN_SET);
    }
}
void Motor3_Send(void) // motor3 速度更新指令
{
    float output = 0;
    PID_SingleCalc(&motor[2].pid, motor[2].targetSpeed, motor[2].speed.value);
    output = motor[2].pid.output;
    if (output > 0) // 对应正转
    {
        __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_3, (uint32_t)output);
        M3AIN1(GPIO_PIN_RESET);
        M3AIN2(GPIO_PIN_SET);
    }
    else // 对应反转
    {
        __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_3, (uint32_t)(-output));
        M3AIN1(GPIO_PIN_SET);
        M3AIN2(GPIO_PIN_RESET);
    }
}
void Motor4_Send(void) // motor4 速度更新指令
{
    float output = 0;
    PID_SingleCalc(&motor[3].pid, motor[3].targetSpeed, motor[3].speed.value);
    output = motor[3].pid.output;
    if (output > 0) // 对应正转
    {
        __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_4, (uint32_t)output);
        M4BIN1(GPIO_PIN_SET);
        M4BIN2(GPIO_PIN_RESET);
    }
    else // 对应反转
    {
        __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_4, (uint32_t)(-output));
        M4BIN1(GPIO_PIN_RESET);
        M4BIN2(GPIO_PIN_SET);
    }
}
// 速度更新
void Velocity_Upgrade(void)
{
    motor[0].targetSpeed = Vx - Vy + Vz;
    motor[1].targetSpeed = Vx + Vy - Vz;
    motor[2].targetSpeed = Vx + Vy + Vz;
    motor[3].targetSpeed = Vx - Vy - Vz;
    // 过滤掉异常数值（超过电机速度最大值）
    for (int8_t i = 0; i < 4; i++)
    {
        Data_Filter(&motor[i].speed);
    }
    // 更新电机控制数值
    Motor1_Send();
    Motor2_Send();
    Motor3_Send();
    Motor4_Send();
}

// 角度更新
void Angle_Upgrade(void)
{
    PID_SingleCalc(&YAW, targetYaw, actYaw);
    Vz = -YAW.output;
    if (targetYaw - actYaw < 1 && targetYaw - actYaw > -1)
    {
        Vz = 0;
    }
}
// 电机速度计算函数
void MotorSpeedCal(void)
{
    // int16_t COUNTERNUM[4] = {COUNTERNUM1, COUNTERNUM2, COUNTERNUM3, COUNTERNUM4};
    // int16_t RELOADVALUE[4] = {RELOADVALUE1, RELOADVALUE2, RELOADVALUE3, RELOADVALUE4};
    // int16_t pluse = 0;
    // for (int8_t i = 0; i < 4; i++)
    // {
    //     pluse = COUNTERNUM[i] - RELOADVALUE[i] / 2;
    //     // 从开始到现在当前10ms的总脉冲数
    //     motor[i].totalAngle = pluse + motor[i].loopNum * RELOADVALUE[i] / 2;
    //     // 进行速度计算
    //     // motor.totalAngle - motor.lastAngle为当前10ms内的增量，即脉冲数
    //     motor[i].speed.measuredValue = motor[i].totalAngle / 0.8213f;
    //     // 更新转过的圈数
    //     motor[i].lastAngle = motor[i].totalAngle;
    // }
    int16_t pluse1 = COUNTERNUM1 - RELOADVALUE1 / 2;
    int16_t pluse2 = COUNTERNUM2 - RELOADVALUE2 / 2;
    int16_t pluse3 = COUNTERNUM3 - RELOADVALUE3 / 2;
    int16_t pluse4 = COUNTERNUM4 - RELOADVALUE4 / 2;

    // 从开始到现在当前10ms的总脉冲数
    motor[0].totalAngle = pluse1 + motor[0].loopNum * RELOADVALUE1 / 2;
    motor[1].totalAngle = pluse2 + motor[1].loopNum * RELOADVALUE2 / 2;
    motor[2].totalAngle = pluse3 + motor[2].loopNum * RELOADVALUE3 / 2;
    motor[3].totalAngle = pluse4 + motor[3].loopNum * RELOADVALUE4 / 2;

    // 进行速度计算
    // motor.totalAngle - motor.lastAngle为当前10ms内的增量，即脉冲数
    motor[0].speed.measuredValue = -(motor[0].totalAngle - motor[0].lastAngle) / 0.4106f;
    motor[1].speed.measuredValue = (motor[1].totalAngle - motor[1].lastAngle) / 0.4106f;
    motor[2].speed.measuredValue = -(motor[2].totalAngle - motor[2].lastAngle) / 0.4106f;
    motor[3].speed.measuredValue = (motor[3].totalAngle - motor[3].lastAngle) / 0.4106f;

    // 更新转过的圈数
    motor[0].lastAngle = motor[0].totalAngle;
    motor[1].lastAngle = motor[1].totalAngle;
    motor[2].lastAngle = motor[2].totalAngle;
    motor[3].lastAngle = motor[3].totalAngle;
}
// 整体偏航角计算函数
void AngleCal(void)
{
    MPU6050_DMP_get_data(&pitch, &roll, &yaw);
    // 消除 180 -180 之间跳变
    // 并且使旋转方向受控制
    if (actYaw - yaw - turns * 360 > 180)
        turns++;
    else if (actYaw - yaw - turns * 360 < -180)
        turns--;

    actYaw = yaw + turns * 360;
}
// 中断回调函数
// TIM6 仅用来每10ms触发一次中断
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == htim6.Instance) // 10ms中断
    {
        if (t >= 20000)
        {
            Vx = -Vx;
            t = 0;
        }
        t++;

        // 程序流程
        AngleCal();      // 计算角度
        MotorSpeedCal(); // 获得编码器值，并且计算电机速度
        // Angle_Upgrade();
        Velocity_Upgrade();
        DataPrint(); // 数据输出
    }
}
