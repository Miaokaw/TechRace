#ifndef __OLED_I2C_H__
#define __OLED_I2C_H__

#include "I2c.h"
#include "codetab.h"

#define OLED_ADDRESS 0x7A // 通过调整0R电阻,屏可以0x78和0x7A两个地址 -- 默认0x78

void I2C_WriteByte(uint8_t addr, uint8_t data);
void WriteCmd(unsigned char I2C_Command);
void WriteDat(unsigned char I2C_Data);
void OLED_Init(void);
void OLED_SetPos(unsigned char x, unsigned char y);
void OLED_Fill(unsigned char fill_Data);
void OLED_CLS(void);
void OLED_ON(void);
void OLED_OFF(void);
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize);
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N);
void OLED_DrawBMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char BMP[]);
void OLED_Show_Char(uint8_t x, uint8_t y, uint8_t chr, uint8_t size); /* OLED显示字符 */
void OLED_Show_Num(uint8_t x, uint8_t y, uint32_t num, uint8_t size);  /* OLED显示数字 */
void OLED_Show_Float(uint8_t x, uint8_t y, float num, uint8_t size);  /* OLED显示小数 */

#endif
