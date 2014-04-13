#include "MyBox.h"

#include "MyUtils.h"

/**
  * @brief  Constructs the MyBox Object, which is a border only box, not filled
  * @param  x The x position where the box should be drawn
	* @param  y The y position where the box should be drawn
	* @param  width The width of the box
	* @param  height The height of the box
	* @param  fg_col The color of the box
	* @param  fg_col The background color of the box, used when undrawn
  * @retval None
  */
MyBox::MyBox(uint16_t x, uint16_t y, uint16_t height, uint16_t width, uint16_t fg_col, uint16_t bg_col)
{
	m_fg_color_ = fg_col;
	m_bg_color_ = bg_col;
	
	m_x_ = x;
	m_y_ = y;
	
	m_height_ = height;
	m_width_ = width;
}

/**
  * @brief  Destructs the MyBox Object
  * @param  None
  * @retval None
  */
MyBox::~MyBox()
{
	
}

/**
  * @brief  Draws the Box
  * @param  None
  * @retval None
  */
void MyBox::draw()
{
	LCD_DrawLine(m_x_, m_y_, m_x_+m_width_, m_y_, m_fg_color_);
	LCD_DrawLine(m_x_, m_y_, m_x_, m_y_+m_height_, m_fg_color_);
	
	LCD_DrawLine(m_x_+m_width_, m_y_, m_x_+m_width_, m_y_+m_height_, m_fg_color_);
	LCD_DrawLine(m_x_, m_y_+m_height_, m_x_+m_width_, m_y_+m_height_, m_fg_color_);
}

/**
  * @brief  Undraws the Box
  * @param  None
  * @retval None
  */
void MyBox::undraw()
{
	LCD_DrawLine(m_x_, m_y_, m_x_+m_width_, m_y_, m_bg_color_);
	LCD_DrawLine(m_x_, m_y_, m_x_, m_y_+m_height_, m_bg_color_);
	
	LCD_DrawLine(m_x_+m_width_, m_y_, m_x_+m_width_, m_y_+m_height_, m_bg_color_);
	LCD_DrawLine(m_x_, m_y_+m_height_, m_x_+m_width_, m_y_+m_height_, m_bg_color_);
}
