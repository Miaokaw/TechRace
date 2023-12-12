#include "my_uart.h"

/*
    dma配置在 stm32f4xx_it.c 串口3中断中
    dma初始化在 usart.c 串口初始化中
 */

JY901_Data JY901_data;
MS53L1M_Data MS53L1M_data1;
MS53L1M_Data MS53L1M_data2;

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;

// printf重定向(使用串口中断)
int fputc(int ch, FILE *f)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 500);
    return (ch);
}
// JY901 dma串口初始化
void User_USART3_Init(JY901_Data *Data)
{
    for (uint16_t i = 0; i < JY901_RXBUFFER_LEN; i++)
        Data->RxBuffer[i] = 0;
    Data->Rx_flag = 0;
    Data->Rx_len = 0;
}
void User_UART45_Init(MS53L1M_Data *Data)
{
    for (uint16_t i = 0; i < MS53L1M_RXBUFFER_LEN; i++)
        Data->RxBuffer[i] = 0;
    Data->Rx_flag = 0;
    Data->Rx_len = 0;
}
// JY901数据处理
void JY901_Process(void)
{
    // JY901数据接收基本不出问题，所以直接这么写了
    // 只接收yaw
    // 详细见正点原子ATK-JY901使用手册
    JY901_data.yaw = (float)(JY901_data.RxBuffer[8] | (int16_t)(JY901_data.RxBuffer[9] << 8)) / 32768 * 180;
}
void MS53L1M_Process(MS53L1M_Data *MS53L1M_data)
{
    MS53L1M_data->lastDistance = MS53L1M_data->Distance;
    MS53L1M_data->Distance = 0;
    for (int i = 28; i < 33; i++)
    {
        if (MS53L1M_data->RxBuffer[i] != 0x20)
        {
            MS53L1M_data->Distance = MS53L1M_data->Distance * 10 + MS53L1M_data->RxBuffer[i] - 0x30;
        }
        }
        if (MS53L1M_data->Distance < 50 || MS53L1M_data->Distance > 1000)
        MS53L1M_data->Distance = 9999;
        else if (MS53L1M_data->lastDistance == 9999 && MS53L1M_data->Distance < 130)
        MS53L1M_data->Distance = 9999;
}
