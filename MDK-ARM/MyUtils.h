#ifndef MYUTILS_H
#define MYUTILS_H

#include <stdlib.h>
#include <string.h>

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
};

#endif
