#ifndef DISPLAY1_H
#define DISPLAY1_H

extern "C"
{
	#include "stm32f10x.h"
	#include "stm32f10x_usart.h"
}

#include "MyDisplay.h"

#include <stdlib.h>
#include <stdio.h>

class Display1 : public MyDisplay
{
public:
	Display1();
	~Display1();

	void create(bool draw);
	void draw(){create(true);};
	void undraw(){create(false);};
	
	void inject_touch(uint16_t x, uint16_t y);
	void handle_touch();
};

#endif

