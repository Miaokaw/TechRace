#include "pid.h"
// PID初始化函数
void PID_Init(Pid *pid, float p, float i, float d, float maxI, float maxOut)
{
    pid->kp = p;             // pid参数p
    pid->ki = i;             // pid参数i
    pid->kd = d;             // pid参数d
    pid->maxIntegral = maxI; // 积分上限
    pid->maxOutput = maxOut; // pwm输出上限
}

float Data_Filter(float *data, float measuredValue, float minValue, float maxValue)
{
    float sum = 0.0f;
    if (measuredValue > minValue && measuredValue < maxValue)
    {
        for (int i = 20 - 1; i > 0; i--) // 将现有数据后移一位
        {
            data[i] = data[i - 1];
            sum += data[i - 1];
        }
        data[0] = measuredValue; // 第一位是新的数据
        sum += measuredValue;
    }
    return sum / 20; // 返回均值
}

// 此为位置式pid，另外还有增量式pid
// 位置式pid调法经验：
//  全部置为零，先调p，直到电机抖动，将p * 0.8,再调 i;
//  可以不要d;
// 单级PID计算，      PID结构体，     目标值，         反馈值
void PID_SingleCalc(Pid *pid, float reference, float feedback)
{
    // 更新数据
    pid->lastError = pid->error;
    pid->error = reference - feedback;

    // if (pid->error < 1 && pid->error > -1) pid->error = 0; //死区控制

    // 计算比例
    pid->output = pid->error * pid->kp;

    // 计算微分
    pid->derivative = (pid->error - pid->lastError) * pid->kd;

    // 计算积分
    pid->integral += pid->error * pid->ki;
    LIMIT(pid->integral, -pid->maxIntegral, pid->maxIntegral); // 积分限幅

    // 计算和
    pid->output = pid->output + pid->integral + pid->derivative;

    // 输出限幅
    LIMIT(pid->output, -pid->maxOutput, pid->maxOutput);
}

// pid数值重置
void PID_Clear(Pid *pid)
{
    pid->derivative = 0;
    pid->error = 0;
    pid->integral = 0;
    pid->lastError = 0;
    pid->output = 0;
}
