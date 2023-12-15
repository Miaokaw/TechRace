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

	WriteCmd(0xAE); // display off
	WriteCmd(0x20); // Set Memory Addressing Mode
	WriteCmd(0x10); // 00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	WriteCmd(0xb0); // Set Page Start Address for Page Addressing Mode,0-7
	WriteCmd(0xc8); // Set COM Output Scan Direction
	WriteCmd(0x00); //---set low column address
	WriteCmd(0x10); //---set high column address
	WriteCmd(0x40); //--set start line address
	WriteCmd(0x81); //--set contrast control register
	WriteCmd(0xff); // �1�7�1�7�1�7�0�0�1�7�1�7�1�7 0x00~0xff
	WriteCmd(0xa1); //--set segment re-map 0 to 127
	WriteCmd(0xa6); //--set normal display
	WriteCmd(0xa8); //--set multiplex ratio(1 to 64)
	WriteCmd(0x3F); //
	WriteCmd(0xa4); // 0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	WriteCmd(0xd3); //-set display offset
	WriteCmd(0x00); //-not offset
	WriteCmd(0xd5); //--set display clock divide ratio/oscillator frequency
	WriteCmd(0xf0); //--set divide ratio
	WriteCmd(0xd9); //--set pre-charge period
	WriteCmd(0x22); //
	WriteCmd(0xda); //--set com pins hardware configuration
	WriteCmd(0x12);
	WriteCmd(0xdb); //--set vcomh
	WriteCmd(0x20); // 0x20,0.77xVcc
	WriteCmd(0x8d); //--set DC-DC enable
	WriteCmd(0x14); //
	WriteCmd(0xaf); //--turn on oled panel
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
