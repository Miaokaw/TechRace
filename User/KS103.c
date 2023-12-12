#include "KS103.h"

#define u8 uint8_t

u8 KS103_ReadOneByte(u8 address, u8 reg)
{
    u8 temp = 0;
    I2C_Start();
    I2C_Send_Byte(address); // 发送低地址
    I2C_Wait_Ack();
    I2C_Send_Byte(reg); // 发送低地址
    I2C_Wait_Ack();
    I2C_Start();
    I2C_Send_Byte(address + 1); // 进入接收模式
    I2C_Wait_Ack();
    Delay_us(50); // 增加此代码通信成功！！！
    temp = I2C_Read_Byte(0); // 读寄存器3
    I2C_Stop(); // 产生一个停止条件
    return temp;
}

void KS103_WriteOneByte(u8 address, u8 reg, u8 command)
{
    I2C_Start();
    I2C_Send_Byte(address); // 发送写命令
    I2C_Wait_Ack();
    I2C_Send_Byte(reg); // 发送高地址
    I2C_Wait_Ack();
    I2C_Send_Byte(command); // 发送低地址
    I2C_Wait_Ack();
    I2C_Stop(); // 产生一个停止条件
}
