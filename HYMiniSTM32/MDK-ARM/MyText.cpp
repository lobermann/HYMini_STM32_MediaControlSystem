#include "MyText.h"

#include "MyUtils.h"

/**
  * @brief  Constructs the MyText Object
  * @param  text The text it should draw
  * @param  x The x position where the text should be drawn
	* @param  y The y position where the text should be drawn
  * @param  fg_col The color of the text
	* @param  bg_col The color of the background
  * @retval None
  */
MyText::MyText(char* text, uint16_t x, uint16_t y, uint16_t fg_col, uint16_t bg_col)
{
	m_fg_color_ = fg_col;
	m_bg_color_ = bg_col;
	
	m_x_ = x;
	m_y_ = y;
	
	m_text_ = new char[strlen(text)+1];
	strcpy(m_text_, text);
}

/**
  * @brief  Detructs the MyText Object
  * @retval None
  */
MyText::~MyText()
{
	delete[] m_text_;
}

/**
  * @brief  Changes the text it should draw and redraws it
  * @param  text The text it should draw
  * @retval None
  */
void MyText::setText(char* text)
{
	undraw();
	delete[] m_text_;
	m_text_ = new char[strlen(text)+1];
	strcpy(m_text_, text);
	draw();
}

/**
  * @brief  Changes the text color
  * @param  col 16bit color
  * @retval None
  */
void MyText::setFGColor(uint16_t col)
{
	undraw();
	m_fg_color_ = col;
	draw();
}

/**
  * @brief  Changes the background color
  * @param  col 16bit color
  * @retval None
  */
void MyText::setBGColor(uint16_t col)
{
	undraw();
	m_bg_color_ = col;
	draw();
}

/**
  * @brief  Changes the position of the text
  * @param  x The x position on the display
  * @param  y The y position on the display
  * @retval None
  */
void MyText::setPosition(uint16_t x, uint16_t y)
{
	undraw();
	m_x_ = x;
	m_y_ = y;
	draw();
}

/**
  * @brief  Draws the text
  * @param  None
  * @retval None
  */
void MyText::undraw()
{
	utils::writeText(m_text_, m_x_, m_y_, m_bg_color_, m_bg_color_);
}

/**
  * @brief  Undraws the text
  * @param  None
  * @retval None
  */
void MyText::draw()
{
	utils::writeText(m_text_, m_x_, m_y_, m_fg_color_, m_bg_color_);
}
