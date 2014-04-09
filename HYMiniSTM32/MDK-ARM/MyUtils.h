#ifndef MYUTILS_H
#define MYUTILS_H

#include <stdlib.h>
#include <string.h>
#include "stm32f10x_usart.h"

class utils
{
public:
	
	/**
  * @brief  Converts a RGB 24bit color code into a 16bit color code
  * @param  r Red value 0x00 to 0xFF
  * @param  g Green value 0x00 to 0xFF
  * @param  b Blue value 0x00 to 0xFF
  * @retval 16bit color value
  */
	static uint16_t color_conv(char r, char g, char b)
	{
		return ((r & 0xf8) << 8) | ((g & 0xfc) << 3) | ((b & 0xf8) >> 3);
	};
	
	/**
  * @brief  Writes text to the display
  * @param  c char array that should be written
  * @param  x the x position where it should be written
  * @param  y the y position where it should be written
	* @param  fg_color the color of the text
	* @param  bg_color the color of the background of the text
  * @retval None
  */
	static void writeText(char* c, uint16_t x, uint16_t y, uint16_t fg_color, uint16_t bg_color)
	{
		int count;
		for(count = 0; count < strlen(c); count++)
		{
			PutChar(x,y,c[count],fg_color, bg_color);
			x += 7;
		}
	};
	
	/**
  * @brief  Draws a solid box on the display
  * @param  x the x position the box should start
  * @param  y the y position the box should start
	* @param  width the width of the box
	* @param  height the height of the box
	* @param  color the color of the box
  * @retval None
  */
	static void drawBox(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color)
	{
		for(uint16_t i = 0;i <= width; ++i)
		{
			for(uint16_t j = 0; j <= height; ++j)
			{
				LCD_SetPoint(x+i, y+j, color);
			}
		}
	}
	
	/**
  * @brief  As the c itoa is not available, this is a custom implementation
  * @param  z uint16_t value that should be converted
  * @param  buf the c string that should contain the string representation
	* @note		buf must be already be allocated
  * @retval None
  */
	static void ItoA( uint16_t z, char* buf )
	{
		int i = 0;
		int j;
		char tmp;
		unsigned u;
	 

		u = (unsigned)z;
		do {
			buf[i++] = '0' + u % 10;
			u /= 10;
		} while( u > 0 );

		for( j = 0; j < i / 2; ++j ) {
			tmp = buf[j];
			buf[j] = buf[i-j-1];
			buf[i-j-1] = tmp;
		}
		buf[i] = '\0';
	}
	
	/**
  * @brief  Write a touch event to the USART1
  * @param  pos the position/element that was touched
  * @retval None
  */
	static void USARTWriteTouch(int pos)
	{
		char* msg = new char[5];
		msg[0] = 0x02;
		msg[1] = 'T';
		msg[2] = pos;
		msg[3] = 0x03;
		msg[4] = 0x00;
		
		for(uint16_t i = 0; i < strlen(msg); i++)
		{
			USARTWrite(msg[i]);
		}
		delete[] msg;
	}

	/**
  * @brief  Write a character to the USART1
  * @param  c the character that should be written
  * @retval None
  */
	static void USARTWrite(char& c)
	{
		USART_SendData(USART1,c);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
	}
};

#endif
