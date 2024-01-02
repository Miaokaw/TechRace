/************************************************************************************
 *  Copyright (c), 2014, HelTec Automatic Technology co.,LTD.
 *            All rights reserved.
 *
 * Http:    www.heltec.cn
 * Email:   cn.heltec@gmail.com
 * WebShop: heltec.taobao.com
 *
 * File name: OLED_I2C.c
 * Project  : HelTec.uvprij
 * Processor: STM32F103C8T6
 * Compiler : MDK fo ARM
 *
 * Author : ���1�7�1�7
 * Version: 1.00
 * Date   : 2014.4.8
 * Email  : hello14blog@gmail.com
 * Modification: none
 *
 * Description:128*64�1�7�1�7�1�7�1�7�1�7OLED�1�7�1�7�0�5�1�7�1�7�1�7�1�7�1�7�1�7�1�7�0�4�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7�0�8�1�7�1�7�1�7(heltec.taobao.com)�1�7�1�7SD1306�1�7�1�7�1�7�1�7IIC�0�0�1�7�0�9�1�7�0�4�1�7�1�7�0�5�1�7�1�7
 *
 * Others: none;
 *
 * Function List:
 *	1. void I2C_Configuration(void) -- �1�7�1�7�1�7�1�7CPU�1�7�1�7�0�0�1�7�1�7I2C
 * 2. void I2C_WriteByte(uint8_t addr,uint8_t data) -- �1�7�1�7�0�6�1�7�1�7�1�7�1�7�1�7�0�7�Ձ0�5�1�7�1�7byte�1�7�1�7�1�7�1�7�1�7�1�7
 * 3. void WriteCmd(unsigned char I2C_Command) -- �Մ1�7�1�7�1�7�1�7
 * 4. void WriteDat(unsigned char I2C_Data) -- �Մ1�7�1�7�1�7�1�7
 * 5. void OLED_Init(void) -- OLED�1�7�1�7�1�7�1�7�0�3�1�7�1�7
 * 6. void OLED_SetPos(unsigned char x, unsigned char y) -- �1�7�1�7�1�7�1�7�1�7�1�7�0�3�1�7�1�7�1�7�1�7�1�7�1�7
 * 7. void OLED_Fill(unsigned char fill_Data) -- �0�0�1�7�1�7�1�7�1�7�1�7
 * 8. void OLED_CLS(void) -- �1�7�1�7�1�7�1�7
 * 9. void OLED_ON(void) -- �1�7�1�7�1�7�1�7
 * 10. void OLED_OFF(void) -- �0�9�1�7�1�7
 * 11. void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize) -- �1�7�1�7�0�5�1�7�0�7�1�7�1�7�1�7(�1�7�1�7�1�7�1�7�1�7���1�7�1�76*8�1�7�1�78*16�1�7�1�7�1�7�1�7)
 * 12. void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N) -- �1�7�1�7�0�5�1�7�1�7�1�7�1�7(�1�7�1�7�1�7�1�7�1�7�1�7�0�8�1�7�1�7�0�0�0�0�1�7�1�7�0�6�1�7�1�7�0�7�1�7codetab.h�1�7�1�7)
 * 13. void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]) -- BMP�0�0�0�2
 *
 * History: none;
 *
 *************************************************************************************/

#include "OLED_I2C.h"

void I2C_WriteByte(uint8_t addr, uint8_t data)
{

	I2C_Start();
	I2C_Send_Byte(OLED_ADDRESS);
	I2C_Wait_Ack();
	I2C_Send_Byte(addr);
	I2C_Wait_Ack();
	I2C_Send_Byte(data);
	I2C_Wait_Ack();
	I2C_Stop();
}

void WriteCmd(unsigned char I2C_Command) // 写锟斤拷锟斤拷
{
	I2C_WriteByte(0x00, I2C_Command);
}

void WriteDat(unsigned char I2C_Data) // �Մ1�7�1�7�1�7�1�7
{
	I2C_WriteByte(0x40, I2C_Data);
}

void OLED_Init(void)
{
	HAL_Delay(100); // �1�7�1�7�1�7�1�7�1�7�1�7�1�7�0�2�1�7�1�7�1�7�1�7�0�8

	WriteCmd(0xAE); /* 关闭显示 */
	WriteCmd(0xD5); /* 设置时钟分频因子,震荡频率 */
	WriteCmd(80);	/* [3:0],分频因子;[7:4],震荡频率 */
	WriteCmd(0xA8); /* 设置驱动路数 */
	WriteCmd(0X3F); /* 默认0X3F(1/64) */
	WriteCmd(0xD3); /* 设置显示偏移 */
	WriteCmd(0X00); /* 默认为0 */

	WriteCmd(0x40); /* 设置显示开始行 [5:0],行数. */

	WriteCmd(0x8D); /* 电荷泵设置 */
	WriteCmd(0x14); /* bit2，开启/关闭 */
	WriteCmd(0x20); /* 设置内存地址模式 */
	WriteCmd(0x02); /* [1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10; */
	WriteCmd(0xA1); /* 段重定义设置,bit0:0,0->0;1,0->127; */
	WriteCmd(0xC8); /* 设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数 */
	WriteCmd(0xDA); /* 设置COM硬件引脚配置 */
	WriteCmd(0x12); /* [5:4]配置 */

	WriteCmd(0x81); /* 对比度设置 */
	WriteCmd(0xEF); /* 1~255;默认0X7F (亮度设置,越大越亮) */
	WriteCmd(0xD9); /* 设置预充电周期 */
	WriteCmd(0xf1); /* [3:0],PHASE 1;[7:4],PHASE 2; */
	WriteCmd(0xDB); /* 设置VCOMH 电压倍率 */
	WriteCmd(0x30); /* [6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc; */

	WriteCmd(0xA4); /* 全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏) */
	WriteCmd(0xA6); /* 设置显示方式;bit0:1,反相显示;0,正常显示 */
	WriteCmd(0xAF); /* 开启显示 */
	OLED_CLS();
}

void OLED_SetPos(unsigned char x, unsigned char y) // �1�7�1�7�1�7�1�7�1�7�1�7�0�3�1�7�1�7�1�7�1�7�1�7�1�7
{
	WriteCmd(0xb0 + y);
	WriteCmd(((x & 0xf0) >> 4) | 0x10);
	WriteCmd((x & 0x0f) | 0x01);
}

void OLED_Fill(unsigned char fill_Data) // �0�0�1�7�1�7�1�7�1�7�1�7
{
	unsigned char m, n;
	for (m = 0; m < 8; m++)
	{
		WriteCmd(0xb0 + m); // page0-page1
		WriteCmd(0x00);		// low column start address
		WriteCmd(0x10);		// high column start address
		for (n = 0; n < 128; n++)
		{
			WriteDat(fill_Data);
		}
	}
}

void OLED_CLS(void) // �1�7�1�7�1�7�1�7
{
	OLED_Fill(0x00);
}

//--------------------------------------------------------------
// Prototype      : void OLED_ON(void)
// Calls          :
// Parameters     : none
// Description    : �1�7�1�7OLED�1�7�1�7�1�7�1�7�1�7�1�7�1�7�݄1�7�1�7�1�7
//--------------------------------------------------------------
void OLED_ON(void)
{
	WriteCmd(0X8D); // �1�7�1�7�1�7�0�1�1�7�0�8�1�7
	WriteCmd(0X14); // �1�7�1�7�1�7�1�7�1�7�1�7�0�8�1�7
	WriteCmd(0XAF); // OLED�1�7�1�7�1�7�1�7
}

//--------------------------------------------------------------
// Prototype      : void OLED_OFF(void)
// Calls          :
// Parameters     : none
// Description    : �1�7�1�7OLED�1�7�1�7�1�7�1�7 -- �1�7�1�7�1�7�1�7�0�0�0�4�1�7�1�7,OLED�1�7�1�7�1�7�0�4�1�7�1�7�1�710uA
//--------------------------------------------------------------
void OLED_OFF(void)
{
	WriteCmd(0X8D); // �1�7�1�7�1�7�0�1�1�7�0�8�1�7
	WriteCmd(0X10); // �1�7�1�9�0�1�1�7�0�8�1�7
	WriteCmd(0XAE); // OLED�1�7�1�7�1�7�1�7
}

//--------------------------------------------------------------
// Prototype      : void OLED_ShowChar(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize)
// Calls          :
// Parameters     : x,y -- �1�7�1�7�0�3�1�7�1�7�1�7�1�7�1�7�1�7(x:0~127, y:0~7); ch[] -- �0�8�1�7�1�7�0�5�1�7�1�7�1�7�0�7�1�7�1�7�1�7; TextSize -- �1�7�0�7�1�7�1�7�1�7��(1:6*8 ; 2:8*16)
// Description    : �1�7�1�7�0�5codetab.h�1�7�ք1�7ASCII�1�7�0�7�1�7,�1�7�1�76*8�1�7�1�78*16�1�7�1�7�0�5�1�7�1�7
//--------------------------------------------------------------
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize)
{
	unsigned char c = 0, i = 0, j = 0;
	switch (TextSize)
	{
	case 1:
	{
		while (ch[j] != '\0')
		{
			c = ch[j] - 32;
			if (x > 126)
			{
				x = 0;
				y++;
			}
			OLED_SetPos(x, y);
			for (i = 0; i < 6; i++)
				WriteDat(F6x8[c][i]);
			x += 6;
			j++;
		}
	}
	break;
	case 2:
	{
		while (ch[j] != '\0')
		{
			c = ch[j] - 32;
			if (x > 120)
			{
				x = 0;
				y++;
			}
			OLED_SetPos(x, y);
			for (i = 0; i < 8; i++)
				WriteDat(F8X16[c * 16 + i]);
			OLED_SetPos(x, y + 1);
			for (i = 0; i < 8; i++)
				WriteDat(F8X16[c * 16 + i + 8]);
			x += 8;
			j++;
		}
	}
	break;
	}
}

//--------------------------------------------------------------
// Prototype      : void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N)
// Calls          :
// Parameters     : x,y -- �1�7�1�7�0�3�1�7�1�7�1�7�1�7�1�7�1�7(x:0~127, y:0~7); N:�1�7�1�7�1�7�1�7�1�7�1�7codetab.h�1�7�ք1�7�1�7�1�7�1�7�1�7
// Description    : �1�7�1�7�0�5codetab.h�1�7�ց0�2�1�7�1�7�1�7,16*16�1�7�1�7�1�7�1�7
//--------------------------------------------------------------
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N)
{
	unsigned char wm = 0;
	unsigned int adder = 32 * N;
	OLED_SetPos(x, y);
	for (wm = 0; wm < 16; wm++)
	{
		WriteDat(F16x16[adder]);
		adder += 1;
	}
	OLED_SetPos(x, y + 1);
	for (wm = 0; wm < 16; wm++)
	{
		WriteDat(F16x16[adder]);
		adder += 1;
	}
}

//--------------------------------------------------------------
// Prototype      : void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);
// Calls          :
// Parameters     : x0,y0 -- �1�7�1�7�0�3�1�7�1�7�1�7�1�7�1�7�1�7(x0:0~127, y0:0~7); x1,y1 -- �1�7�1�7�1�7�0�5�1�7�1�7�1�7(�1�7�1�7�1�7�1�7�1�7�1�7)�1�7�1�7�1�7�1�7�1�7�1�7(x1:1~128,y1:1~8)
// Description    : �1�7�1�7�0�5BMP�ˁ0�0
//--------------------------------------------------------------
void OLED_DrawBMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char BMP[])
{
	unsigned int j = 0;
	unsigned char x, y;

	if (y1 % 8 == 0)
		y = y1 / 8;
	else
		y = y1 / 8 + 1;
	for (y = y0; y < y1; y++)
	{
		OLED_SetPos(x0, y);
		for (x = x0; x < x1; x++)
		{
			WriteDat(BMP[j++]);
		}
	}
}

/**
 * @brief       在指定位置显示一个字符,包括部分字符
 * @param       x   : 0~127
 * @param       y   : 0~7
 * @param       size: 选择字体 6/16
 * @retval      无
 */
void OLED_Show_Char(uint8_t x, uint8_t y, uint8_t chr, uint8_t size)
{
	uint8_t *pfont = 0;
	chr = chr - ' '; /* 得到偏移后的值,因为字库是从空格开始存储的,第一个字符是空格 */

	if (size == 6) /* 调用0806字体 */
	{
		pfont = (uint8_t *)oled_asc2_0806[chr];
	}
	else if (size == 16) /* 调用1608字体 */
	{
		pfont = (uint8_t *)oled_asc2_1608[chr];
	}
	else /* 没有的字库 */
	{
		return;
	}

	if (size == 6)
	{
		if (x > 122)
		{
			x = 0;
			y++;
		}
		OLED_SetPos(x, y);
		for (int i = 0; i < 6; i++)
		{
			WriteDat(pfont[i]);
		}
	}
	else if (size == 16)
	{
		if (x > 120)
		{
			x = 0;
			y++;
		}
		OLED_SetPos(x, y);
		for (int i = 0; i < 8; i++)
		{
			WriteDat(pfont[i]);
		}
		OLED_SetPos(x, y + 1);
		for (int i = 0; i < 8; i++)
		{
			WriteDat(pfont[i + 8]);
		}
	}
}

/**
 * @brief       平方函数, m^n
 * @param       m: 底数
 * @param       n: 指数
 * @retval      无
 */
static uint32_t oled_pow(uint8_t m, uint8_t n)
{
	uint32_t result = 1;

	while (n--)
	{
		result *= m;
	}

	return result;
}

/**
 * @brief       显示数字num
 * @param       x,y : 起始坐标
 * @param       num : 数值(0 ~ 2^32)
 * @param       size: 选择字体 6/16
 * @retval      无
 */
void OLED_Show_Num(uint8_t x, uint8_t y, uint32_t num, uint8_t size)
{
	uint8_t t, temp = num;
	uint8_t enshow = 0;
	uint8_t len = 1;
	while (temp / 10 != 0)
	{
		len++;
		temp /= 10;
	}

	for (t = 0; t < len; t++) /* 按总显示位数循环 */
	{
		temp = (num / oled_pow(10, len - t - 1)) % 10; /* 获取对应位的数字 */

		if (enshow == 0 && t < (len - 1)) /* 没有使能显示,且还有位要显示 */
		{
			if (temp == 0)
			{
				OLED_Show_Char(x + ((size % 8) ? 6 : 8) * t, y, ' ', size); /* 显示空格,站位 */
				continue;													/* 继续下个一位 */
			}
			else
			{
				enshow = 1; /* 使能显示 */
			}
		}

		OLED_Show_Char(x + ((size % 8) ? 6 : 8) * t, y, temp + '0', size); /* 显示字符 */
	}
}

/**
 * @brief       显示小数num
 * @param       x,y : 起始坐标
 * @param       num : 数值(0 ~ 2^32)
 * @param       size: 选择字体 6/16
 * @retval      无
 */
void OLED_Show_Float(uint8_t x, uint8_t y, float num, uint8_t size)
{
	float num_int = num;
	int temp;
	uint8_t t;
	uint8_t enshow = 0;
	uint8_t len = 1, dot_place = 0;
	if (num == 0)
	{
		OLED_Show_Char(x, y, '0', size); /* 显示字符 */
		return ;
	}

	if (num_int > -1 && num_int < 1) /* 处理[0,1]的数 */
	{
		len++;
	}
	if (num_int < 0) /* 负数加负号并取反 */
	{
		num_int = -num_int;
		OLED_Show_Char(x, y, '-', size); /* 显示字符 */
		x += ((size % 8) ? 6 : 8);
	}

	while (floor(num_int) != num_int)
	{
		dot_place++; /* 小数点后位数 */
		num_int *= 10;
		if (floor(num_int) == 0)
		{
			len++;
		}
		else if (num_int > oled_pow(10, 6)) /* 最大有效位  */
		{
			num_int = floor(num_int);
			break;
		}
	}
	temp = (int)floor(num_int);
	while (temp / 10 != 0)
	{
		len++; /* 总长度 */
		temp /= 10;
	}

	for (t = 0; t < len - dot_place; t++) /* 按小数点前位数循环 */
	{
		temp = ((int)num_int / oled_pow(10, len - t - 1)) % 10; /* 获取对应位的数字 */

		if (enshow == 0 && t < (len - 1)) /* 没有使能显示,且还有位要显示 */
		{
			if (temp == 0)
			{
				OLED_Show_Char(x + ((size % 8) ? 6 : 8) * t, y, temp + '0', size); /* 显示空格,站位 */
				continue;														   /* 继续下个一位 */
			}
			else
			{
				enshow = 1; /* 使能显示 */
			}
		}

		OLED_Show_Char(x + ((size % 8) ? 6 : 8) * t, y, temp + '0', size); /* 显示字符 */
	}

	OLED_Show_Char(x + ((size % 8) ? 6 : 8) * (len - dot_place), y, '.', size); /* 显示小数点 */

	for (t = len - dot_place; t < len; t++) /* 按小数点后位数循环 */
	{
		temp = ((int)num_int / oled_pow(10, len - t - 1)) % 10; /* 获取对应位的数字 */

		if (enshow == 0 && t < (len - 1)) /* 没有使能显示,且还有位要显示 */
		{
			if (temp == 0)
			{
				OLED_Show_Char(x + ((size % 8) ? 6 : 8) * (t + 1), y, temp + '0', size); /* 显示空格,站位 */
				continue;																 /* 继续下个一位 */
			}
			else
			{
				enshow = 1; /* 使能显示 */
			}
		}

		OLED_Show_Char(x + ((size % 8) ? 6 : 8) * (t + 1), y, temp + '0', size); /* 显示字符 */
	}
}
