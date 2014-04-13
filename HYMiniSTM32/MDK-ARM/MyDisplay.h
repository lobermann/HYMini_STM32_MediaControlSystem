#ifndef MYDISPLAY_H
#define MYDISPLAY_H

extern "C"
{
	#include "stm32f10x.h"
	#include "TouchPanel.h"
	#include "systick.h"
	#include "GLCD.h"
	#include "stm32f10x_usart.h"
}

/**
  * @brief  This class can be used to create a view on the
  *         display. Simply inherit an overload the functions
  */

class MyDisplay
{
public:
	MyDisplay();
	~MyDisplay();
	void inject_touch(uint16_t x, uint16_t y);

    virtual void write_header(uint16_t color, char* text);

	void draw();
	void undraw();
	virtual void create(bool draw);
	virtual void handle_touch();

	uint16_t m_last_touch_x_;
	uint16_t m_last_touch_y_;
	bool m_pressed_;

};

#endif
