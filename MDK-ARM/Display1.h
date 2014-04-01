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

	virtual void create(bool draw);
	virtual void handle_touch();
};

#endif

