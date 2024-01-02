#ifndef __PID_H__
#define __PID_H__


// 限幅定义
#define LIMIT(x, min, max) (x) = (((x) <= (min)) ? (min) : (((x) >= (max)) ? (max) : (x)))

// pid参数及数值结构体
typedef struct _PID
{
    float kp, ki, kd;            // pid参数
    float error, lastError;      // 当前误差和上一次的误差
    float integral, maxIntegral; // 积分量和积分的限幅
    float derivative;            // 微分量
    float output, maxOutput;     // pwm输出和pwm最大输出
} Pid;

typedef struct _Data
{
    float measuredValue, value;
    float limitedValue;
    float data[20];
} Data;

void Data_Filter(Data *data);
void PID_Init(Pid *pid, float p, float i, float d, float maxI, float maxOut);
void PID_SingleCalc(Pid *pid, float reference, float feedback);
void PID_Clear(Pid *pid);

#endif
