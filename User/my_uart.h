#ifndef _MY_USART_H
#define _MY_USART_H

#include "main.h"
#include "stdio.h"
#include "string.h"
#include "Motor.h"

#define JY901_RXBUFFER_LEN 28 // JY901接收数组长度
#define MS53L1M_RXBUFFER_LEN 34 // 激光接收数组长度

// JY901数据结构体
typedef struct User1_USART3
{
    uint8_t Rx_flag;                      // 接收完成标志（没用到）
    uint8_t Rx_len;                       // 接收长度
    uint8_t RxBuffer[JY901_RXBUFFER_LEN]; // 数据存储
    float yaw;                            // 接收yaw
} JY901_Data;

typedef struct User2_UART45
{
    uint8_t Rx_flag;
    uint8_t Rx_len;
    uint8_t RxBuffer[MS53L1M_RXBUFFER_LEN];
    float Distance;
    float lastDistance;
} MS53L1M_Data;
extern JY901_Data JY901_data;
extern MS53L1M_Data MS53L1M_data1;
extern MS53L1M_Data MS53L1M_data2;

void JY901_Process(void);
void MS53L1M_Process(MS53L1M_Data *MS53L1M_data);

void User_UART45_Init(MS53L1M_Data *Data);
void User_USART3_Init(JY901_Data *Data);

#endif
