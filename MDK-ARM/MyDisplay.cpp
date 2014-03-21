#include "MyDisplay.h"

/**
  * @brief  Sets the base values
  */
MyDisplay::MyDisplay()
{
	m_last_touch_x_ = 0;
	m_last_touch_y_ = 0;
	m_pressed_ = false;
}

/**
  * @brief  Destructs the class
  */
MyDisplay::~MyDisplay()
{
}

/**
  * @brief  This function should be used to create elements on
  *         the screen and to handle screen draw and undraw via param
  * @param  draw To indicate if it should be created or deleted
  * @retval None
  */
void MyDisplay::create(bool draw)
{

}

/**
  * @brief  Handle the touch actions in here
  * @param  None
  * @retval None
  */
void MyDisplay::handle_touch()
{
	
}

/**
  * @brief  This function takes the touch positions and
  *         decides what to do with them.
  * @param  x The x position where the touch occured
  * @param  y The y position where the touch occured
  * @retval None
  */
void MyDisplay::inject_touch(uint16_t x, uint16_t y)
{
	//Check if we are out of the screen bounds or at zero
	if(x == 0 || y == 0 || x > 240 || y > 320)
	{		
		//Out of bound indicates that the touch was released
		m_pressed_ = false;
		return;
	}
	
	//As we only handle touch presses, ignore everything
	//until the touch was released again
	if(m_pressed_) return;
	
	if(m_last_touch_x_ == x && m_last_touch_y_ == y)
	{
		//Nothing changed
	}
	else
	{
		m_last_touch_x_ = x;
		m_last_touch_y_ = y;
		m_pressed_ = true;
		
		//Handle all touch actions in here
		handle_touch();
		
		//Delay of 300ms after a touch action
		delay_ms(300);
	}
}

/**
  * @brief  Draws the defined elements from the create function
  * @param  None
  * @retval None
  */
void MyDisplay::draw()
{
	create(true);
}

/**
  * @brief  Undraws the defined elements from the create function
  * @param  None
  * @retval None
  */
void MyDisplay::undraw()
{
	create(false);
}
