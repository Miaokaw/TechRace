#ifndef __I2C_H__
#define __I2C_H__

#include "stm32f1xx_hal.h"
#include "main.h"
#include "stdint.h"

#define CPU_FREQUENCY_MHZ 72
#define MYI2C_SCL_PIN GPIO_PIN_6
#define MYI2C_SCL_PORT GPIOB
#define MYI2C_SDA_PIN GPIO_PIN_7
#define MYI2C_SDA_PORT GPIOB

#define SDA_Dout_LOW() HAL_GPIO_WritePin(MYI2C_SDA_PORT, MYI2C_SDA_PIN, GPIO_PIN_RESET)
#define SDA_Dout_HIGH() HAL_GPIO_WritePin(MYI2C_SDA_PORT, MYI2C_SDA_PIN, GPIO_PIN_SET)
#define SDA_Data_IN() HAL_GPIO_ReadPin(MYI2C_SDA_PORT, MYI2C_SDA_PIN)
#define SCL_Dout_LOW() HAL_GPIO_WritePin(MYI2C_SCL_PORT, MYI2C_SCL_PIN, GPIO_PIN_RESET)
#define SCL_Dout_HIGH() HAL_GPIO_WritePin(MYI2C_SCL_PORT, MYI2C_SCL_PIN, GPIO_PIN_SET)
#define SCL_Data_IN() HAL_GPIO_ReadPin(MYI2C_SCL_PORT, MYI2C_SCL_PIN)
#define SDA_Write(XX) HAL_GPIO_WritePin(MYI2C_SDA_PORT, MYI2C_SDA_PIN, (XX ? GPIO_PIN_SET : GPIO_PIN_RESET))

void I2C_Init(void);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_Send_Byte(uint8_t txd);
uint8_t I2C_Read_Byte(uint8_t ack);
void I2C_NAck(void);
void I2C_Ack(void);
uint8_t I2C_Wait_Ack(void);

void delay_xus(__IO uint32_t nTime);

#define Delay_us(xx) delay_xus(xx)

#endif
