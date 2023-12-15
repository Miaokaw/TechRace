#ifndef __MPU6050_H__
#define __MPU6050_H__

#include "math.h"
#include "I2c.h"
#include "../DMP/inv_mpu.h"
#include "../DMP/inv_mpu_dmp_motion_driver.h"

void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data);
uint8_t MPU6050_ReadReg(uint8_t RegAddress);

uint8_t MPU6050_Write_Len(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf);
uint8_t MPU6050_Read_Len(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf);

uint8_t MPU6050_init(void);
uint8_t MPU6050_DMP_get_data(float *pitch, float *roll, float *yaw);

// 空函数防止报错
void fake_delay_ms(unsigned long num_ms);
void fake_get_ms(unsigned long *count);

#endif
