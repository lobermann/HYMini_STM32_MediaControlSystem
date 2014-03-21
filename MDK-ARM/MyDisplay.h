#ifndef MYDISPLAY_H
#define MYDISPLAY_H

extern "C"
{
	#include "stm32f10x.h"
	#include "TouchPanel.h"
	#include "systick.h"
	#include "GLCD.h"
}

#include "MyText.h"
#include "MyBox.h"
#include "MyTriangle.h"

/**
  * @brief  This class can be used to create a view on the
  *         display. Simply inherit an overload the create
  *         function
  */

class MyDisplay
{
public:
	MyDisplay();
	~MyDisplay();
	void inject_touch(uint16_t x, uint16_t y);

	void draw();
	void undraw();
	void create(bool draw);
	void handle_touch();

private:
	
	uint16_t m_last_touch_x_;
	uint16_t m_last_touch_y_;
	bool m_pressed_;

};

#endif
