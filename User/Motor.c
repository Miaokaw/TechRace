#include "Motor.h"
#include "tim.h"
#include "math.h"

Pid YAW, DISTANCE;     // 偏航校正,距离控制pid结构体
Motor motor[4];        // 电机结构体
uint8_t direction = X; // 设定方向
int time;
uint8_t t1 = 0;           // printf
uint8_t t2 = 0;           // 超声波
uint8_t mode = 1;         // 模式 1(速度) 2(距离) 3(角度) 4(绕轴旋转)
uint8_t flag = 0;         // 角度模式 0 为速度模式 1 为pid控制
uint8_t flag1 = 0;        // 距离模式 0 为速度模式 1 为pid控制
int8_t t = 0;             // 延迟
int8_t n = 0;             // 转圈数
float value;              // 灰度数值和
int Vx;                   // 设定直行速度
int Vy;                   // 设定横移速度
int Vz;                   // 设定旋转速度
int V = 0;                // 设定距离控制速度
int time;                 // 时间
float actYaw;             // 实际偏航角
float actDistance;        // 实际距离
float expectDistance;     // 估计行进距离
float calDistance;        // 记录距离用于判断
float targetAngle = 0;    // 角度目标值
float targetDistance = 0; // 距离目标值
float p, i, d, imax;      // pid参数
float realdistance, distance;
float group[10];
uint8_t range1, range2;
void DataPrint(void)
{
    time++;
    if (t % 5 == 0)
    {
        printf("1");
        t1 = 0;
    }
}
// 电机数值重置函数
void MotorValue_Clear(Motor *motor)
{
    motor->loopNum = 0; // 溢出计数
    for (int8_t i = 0; i < 20; i++)
    {
        motor->speed_Record[i] = 0;
    }
}
// 电机使能
void Motor1_Init(void)
{
    HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL); // 开启编码器定时器
    HAL_TIM_Base_Start_IT(&htim6);                  // 开启10ms定时器中断
    HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_1);       // 开启PWM
    __HAL_TIM_SET_COUNTER(&htim1, 10000);           // 编码器定时器初始值设定为10000
    MotorValue_Clear(&motor[0]);                    // 电机数值重置
}
void Motor2_Init(void)
{
    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL); // 开启编码器定时器
    HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_2);       // 开启PWM
    __HAL_TIM_SET_COUNTER(&htim2, 10000);           // 编码器定时器初始值设定为10000
    MotorValue_Clear(&motor[1]);                    // 电机数值重置
}
void Motor3_Init(void)
{
    HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL); // 开启编码器定时器
    HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_3);       // 开启PWM
    __HAL_TIM_SET_COUNTER(&htim3, 10000);           // 编码器定时器初始值设定为10000
    MotorValue_Clear(&motor[2]);                    // 电机数值重置
}
void Motor4_Init(void)
{
    HAL_TIM_Encoder_Start(&htim8, TIM_CHANNEL_ALL); // 开启编码器定时器
    HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_4);       // 开启PWM
    __HAL_TIM_SET_COUNTER(&htim8, 10000);           // 编码器定时器初始值设定为10000
    MotorValue_Clear(&motor[3]);                    // 电机数值重置
}
void set_PID(float kp, float ki, float kd, int maxi, int maxoutput)
{
    for (int8_t i = 0; i < 4; i++)
    {
        PID_Init(&motor[i].pid, kp, ki, kd, maxi, maxoutput);
    }
}
void Init(void)
{
    Motor1_Init();
    Motor2_Init();
    Motor3_Init();
    Motor4_Init();
    set_PID(0, 0, 0, 0, 0);
    PID_Init(&DISTANCE, 4, 0, 0, 0, 200);
    PID_Init(&YAW, 0, 0, 0, 0, 0);
    HAL_Delay(1000); // 等待传感器初始化完成
}
// 电机速度更新
void Motor1_Send(void) // motor1 速度更新指令
{
    float output = 0;
    PID_SingleCalc(&motor[0].pid, motor[0].targetSpeed, motor[0].speed, mode);
    output = motor1.pid.output;
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
    PID_SingleCalc(&motor[1].pid, motor[1].targetSpeed, motor[1].speed, mode);
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
    PID_SingleCalc(&motor[2].pid, motor[2].targetSpeed, motor[2].speed, mode);
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
    PID_SingleCalc(&motor[3].pid, motor[3].targetSpeed, motor[3].speed, mode);
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
// 速度滤波
int Speed_Low_Filter(Motor *motor)
{
    float sum = 0.0f;
    if (motor->espeed > 280 || motor->espeed < -280)
    {
        return 0;
    }
    for (uint8_t i = 20 - 1; i > 0; i--) // 将现有数据后移一位
    {
        motor->speed_Record[i] = motor->speed_Record[i - 1];
        sum += motor->speed_Record[i - 1];
    }
    motor->speed_Record[0] = motor->espeed; // 第一位是新的数据
    sum += motor->espeed;
    motor->speed = sum / 20; // 计算均值
    return 0;
}
void distance_Filter(void)
{
    if (realdistance > 10 && realdistance < 5000)
    {
        float sum = 0.0f;
        for (uint8_t i = 10 - 1; i > 0; i--) // 将现有数据后移一位
        {
            group[i] = group[i - 1];
            sum += group[i - 1];
        }
        group[0] = realdistance; // 第一位是新的数据
        sum += realdistance;
        distance = sum / 10; // 计算均值
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
        int a = 1;
        Speed_Low_Filter(&motor[i]);
    }

    // 更新电机控制数值
    Motor1_Send();
    Motor2_Send();
    Motor3_Send();
    Motor4_Send();
}
// 距离更新
void Distance_Upgrade(void)
{
    // 估计行进距离  += 电机编码器数值 * 2 * pi * r * 10ms / 1000(ms/s) / 60s * 2
    PID_SingleCalc(&DISTANCE, targetDistance, expectDistance, mode);
    if (direction == X)
    {
        expectDistance += (motor1.speed + motor2.speed + motor3.speed + motor4.speed) / 1273.2396f * 2.0f;
        Vx = DISTANCE.output;
        Vy = 0;
    }
    else if (direction == Y)
    {
        expectDistance += (-motor1.espeed + motor2.espeed + motor3.espeed - motor4.espeed) / 1273.2396f * 2.0f;
        Vy = DISTANCE.output;
        Vx = 0;
    }
    if (targetDistance - expectDistance < 1.0f && targetDistance - expectDistance > -1.0f)
    {
        Move(STOP);
    }
}
// 角度更新
void Angle_Upgrade(void)
{
    PID_SingleCalc(&YAW, targetAngle, actyaw, mode);
    Vz = -YAW.output;
    if (targetAngle - actyaw < 1 && targetAngle - actyaw > -1)
    {
        Move(STOP);
    }
}
// 麦轮运动函数
// 模式请看"Motor.h"宏定义
// 当inputMode为 0 时更新
void Move(int x, int y, int z, uint8_t inputMode)
{
}
// 电机速度计算函数
void MotorSpeedCal(void)
{
    int16_t COUNTERNUM[4] = {COUNTERNUM1, COUNTERNUM2, COUNTERNUM3, COUNTERNUM4};
    int16_t RELOADVALUE[4] = {RELOADVALUE1, RELOADVALUE2, RELOADVALUE3, RELOADVALUE4};
    int16_t pluse = 0;
    for (int8_t i = 0; i < 4; i++)
    {
        pluse = COUNTERNUM[i] - RELOADVALUE[i] / 2;
        // 从开始到现在当前10ms的总脉冲数
        motor[i].totalAngle = pluse + motor[i].loopNum * RELOADVALUE[i] / 2;
        // 进行速度计算
        // motor.totalAngle - motor.lastAngle为当前10ms内的增量，即脉冲数
        motor[i].espeed = motor[i].totalAngle / 0.3124f;
        // 更新转过的圈数
        motor[i].lastAngle = motor[i].totalAngle;
    }
}
// 更新函数
void Upgrade(void)
{
    Distance_Upgrade();
    Angle_Upgrade();
    Velocity_Upgrade();
}
// 中断回调函数
// TIM6 仅用来每10ms触发一次中断
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == htim6.Instance) // 10ms中断
    {
        // 程序流程
        AngleCal();      // 计算角度
        MotorSpeedCal(); // 获得编码器值，并且计算电机速度
        DataUpgrade();   // 更新
        DataPrint();     // 数据输出
    }
}
// 整体偏航角计算函数
void AngleCal(void)
{
    // 消除 180 -180 之间跳变
    // 并且使旋转方向受控制
    if (actyaw - JY901_data.yaw - n * 360 > 180)
        n++;
    else if (actyaw - JY901_data.yaw - n * 360 < -180)
        n--;

    actyaw = JY901_data.yaw + n * 360;
}