#ifndef DISPLAY1_H
#define DISPLAY1_H

#include "MyDisplay.h"

class Display1 : public MyDisplay
{
public:
	Display1();
	~Display1();

	void create(bool draw);
	void draw(){create(true);};
	void undraw(){create(false);};
};

#endif

