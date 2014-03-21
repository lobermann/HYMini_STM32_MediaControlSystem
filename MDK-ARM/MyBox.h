#ifndef MYBOX_H
#define MYBOX_H

extern "C"
{
	#include "stm32f10x.h"
	#include "TouchPanel.h"
	#include "systick.h"
	#include "GLCD.h"
}

#include "MyUtils.h"

class MyBox
{
public:
	MyBox(uint16_t x, uint16_t y, uint16_t height, uint16_t width, uint16_t fg_col, uint16_t bg_col);
  ~MyBox();

	void draw();
	void undraw();
		
private:
	uint16_t m_x_;
	uint16_t m_y_;
	uint16_t m_height_;
	uint16_t m_width_;
	uint16_t m_fg_color_;
	uint16_t m_bg_color_;
};

#endif
