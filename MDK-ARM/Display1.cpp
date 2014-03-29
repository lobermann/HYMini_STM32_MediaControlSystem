#include "Display1.h"

#include "MyUtils.h"

Display1::Display1()
{
	
}

Display1::~Display1()
{
	
}

/**
  * @brief  This function takes the touch positions and
  *         decides what to do with them.
  * @param  x The x position where the touch occured
  * @param  y The y position where the touch occured
  * @retval None
  */
void Display1::inject_touch(uint16_t x, uint16_t y)
{
	//Check if we are out of the screen bounds or at zero
	if(x == 0 || y == 0 || x > 240 || y > 320)
	{		
		//Out of bound indicates that the touch was released
		
		//Handle all touch actions in here, when the touch is released
		if(m_pressed_)
			handle_touch();
		
		m_pressed_ = false;
		return;
	}
	
	if(m_last_touch_x_ == x && m_last_touch_y_ == y)
	{
		//Nothing changed
	}
	else
	{
		m_last_touch_x_ = x;
		m_last_touch_y_ = y;
		m_pressed_ = true;
	}
}

void Display1::handle_touch()
{
	//Check if a needed screen section was touched
	bool success = false;
	uint8_t section = 0;
	
	//Center 1
	if(m_last_touch_x_ >= 15 && m_last_touch_x_ <= 225 &&
		 m_last_touch_y_ >= 35 && m_last_touch_y_ <= 65)
	{
		success = true;
		section = 11;
	}
	//Center 2
	else if(m_last_touch_x_ >= 15 && m_last_touch_x_ <= 225 &&
		      m_last_touch_y_ >= 70 && m_last_touch_y_ <= 100)
	{
		success = true;
		section = 12;
	}
	//Center 3
	else if(m_last_touch_x_ >= 15 && m_last_touch_x_ <= 225 &&
		      m_last_touch_y_ >= 105 && m_last_touch_y_ <= 135)
	{
		success = true;
		section = 13;
	}
	//Center 4
	else if(m_last_touch_x_ >= 15 && m_last_touch_x_ <= 225 &&
		      m_last_touch_y_ >= 140 && m_last_touch_y_ <= 170)
	{
		success = true;
		section = 14;
	}
	//Center 5
	else if(m_last_touch_x_ >= 15 && m_last_touch_x_ <= 225 &&
		      m_last_touch_y_ >= 175 && m_last_touch_y_ <= 205)
	{
		success = true;
		section = 15;
	}
	//Center 6
	else if(m_last_touch_x_ >= 15 && m_last_touch_x_ <= 225 &&
		      m_last_touch_y_ >= 210 && m_last_touch_y_ <= 240)
	{
		success = true;
		section = 16;
	}
	
	if(success)
	{
		//Debug out
		char* x = new char[3];
		utils::ItoA(section, x);
		MyText* text1 = new MyText(x, 10, 8, utils::color_conv(0xff,0x9c,0x00), 0x000000);
		text1->draw();
		delete text1;
		delete[] x;
		
		char* msg = new char[5];
		msg[0] = 0x02;
		msg[1] = 'T';
		msg[2] = section;
		msg[3] = 0x03;
		msg[4] = 0x00;
		
		for(uint16_t i = 0; i < strlen(msg); i++)
		{
			USART_SendData(USART1,msg[i]);
			while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
		}
		delete[] msg;
		
		//Short delay after sending, to prevent multiple touches per press
		//TODO: Find a better solution then a sleep
		delay_ms(100);
	}
}

void Display1::create(bool draw)
{
	MyText* text1 = new MyText("System booting...", 10, 8, utils::color_conv(0xff,0x9c,0x00), 0x000000);
	if(draw) text1->draw();
	else text1->undraw();
	delete text1;
	
	MyBox* box1 = new MyBox(5, 5, 310, 230, utils::color_conv(0xb7,0xb7,0xb7), 0x000000);
	if(draw) box1->draw();
	else box1->undraw();
	delete box1;
	
	MyBox* box2 = new MyBox(5, 5, 20, 230, utils::color_conv(0xb7,0xb7,0xb7), 0x000000);
	if(draw) box2->draw();
	else box2->undraw();
	delete box2;
	
	MyBox* box3 = new MyBox(5, 275, 40, 230, utils::color_conv(0xb7,0xb7,0xb7), 0x000000);
	if(draw) box3->draw();
	else box3->undraw();
	delete box3;
	
	MyBox* playPause = new MyBox(5, 275, 40, 58, utils::color_conv(0xb7,0xb7,0xb7), 0x000000);
	if(draw) playPause->draw();
	else playPause->undraw();
	delete playPause;
	
	MyBox* stop = new MyBox(63, 275, 40, 58, utils::color_conv(0xb7,0xb7,0xb7), 0x000000);
	if(draw) stop->draw();
	else stop->undraw();
	delete stop;
	
	MyBox* volup = new MyBox(121, 275, 40, 57, utils::color_conv(0xb7,0xb7,0xb7), 0x000000);
	if(draw) volup->draw();
	else volup->undraw();
	delete volup;
	
	MyBox* voldown = new MyBox(178, 275, 40, 57, utils::color_conv(0xb7,0xb7,0xb7), 0x000000);
	if(draw) voldown->draw();
	else voldown->undraw();
	delete voldown;
	
	
	{ //Create the PlayPause button in a scope for cleanup
		MyTriangle::Point2D playPauseOrigin(13,280);
		uint16_t playPause_fg_col = utils::color_conv(0x00,0xff,0x1e);
		
		MyTriangle* playpausearrow = new MyTriangle(playPauseOrigin, 
																								MyTriangle::Point2D(0,0), 
																								MyTriangle::Point2D(25,15), 
																								MyTriangle::Point2D(0,30), 
																								playPause_fg_col, 0x000000);
		if(draw) playpausearrow->draw();
		else playpausearrow->undraw();
		delete playpausearrow;
		
		utils::drawBox(playPauseOrigin.x+25, playPauseOrigin.y, 6, 30, playPause_fg_col);
		utils::drawBox(playPauseOrigin.x+35, playPauseOrigin.y, 6, 30, playPause_fg_col);
	}
	
	{ //Stop Button
		MyTriangle::Point2D StopOrigin(77,280);
		uint16_t Stop_fg_col = utils::color_conv(0xff,0x1e,0x00);
		
		if(draw) utils::drawBox(StopOrigin.x, StopOrigin.y, 30, 30, Stop_fg_col);
		else utils::drawBox(StopOrigin.x, StopOrigin.y, 30, 30, 0x0000);
	}
	
	{
		MyTriangle::Point2D Origin(130,280);
		uint16_t fg_col = utils::color_conv(0xaa,0xaa,0xaa);
		
		if(draw) utils::drawBox(Origin.x, Origin.y+10, 10, 10, fg_col);
		else utils::drawBox(Origin.x, Origin.y+10, 10, 10, 0x0000);
		
		MyTriangle* box = new MyTriangle(Origin, 
																			MyTriangle::Point2D(10,10), 
																			MyTriangle::Point2D(20,0), 
																			MyTriangle::Point2D(10,20), 
																			fg_col, 0x000000);
		if(draw) box->draw();
		else box->undraw();
		delete box;
		
		box = new MyTriangle(Origin, 
													MyTriangle::Point2D(10,20), 
													MyTriangle::Point2D(20,0), 
													MyTriangle::Point2D(20,30), 
													fg_col, 0x000000);
		if(draw) box->draw();
		else box->undraw();
		delete box;
		
		if(draw) utils::drawBox(Origin.x+25, Origin.y+14, 16, 3, fg_col);
		else utils::drawBox(Origin.x+25, Origin.y+14, 16, 3, 0x0000);
		
		if(draw) utils::drawBox(Origin.x+32, Origin.y+8, 3, 15, fg_col);
		else utils::drawBox(Origin.x+32, Origin.y+8, 3, 15, 0x0000);
	}
	
	{
		MyTriangle::Point2D Origin(187,280);
		uint16_t fg_col = utils::color_conv(0xaa,0xaa,0xaa);
		
		if(draw) utils::drawBox(Origin.x, Origin.y+10, 10, 10, fg_col);
		else utils::drawBox(Origin.x, Origin.y+10, 10, 10, 0x0000);
		
		MyTriangle* box = new MyTriangle(Origin, 
																			MyTriangle::Point2D(10,10), 
																			MyTriangle::Point2D(20,0), 
																			MyTriangle::Point2D(10,20), 
																			fg_col, 0x000000);
		if(draw) box->draw();
		else box->undraw();
		delete box;
		
		box = new MyTriangle(Origin, 
													MyTriangle::Point2D(10,20), 
													MyTriangle::Point2D(20,0), 
													MyTriangle::Point2D(20,30), 
													fg_col, 0x000000);
		if(draw) box->draw();
		else box->undraw();
		delete box;
		
		if(draw) utils::drawBox(Origin.x+25, Origin.y+14, 16, 3, fg_col);
		else utils::drawBox(Origin.x+25, Origin.y+14, 16, 3, 0x0000);
	}
	
	//TODO: Text below needs to be drawn via I2C
	uint16_t list_y = 30;
	{
		MyTriangle::Point2D Origin(10,list_y);
		/*MyText* text1 = new MyText("Hitradio OE3", Origin.x+10, Origin.y+13, utils::color_conv(0xd0,0xff,0xda), 0x000000);
		text1->draw();
		delete text1;*/
		
		MyBox* box1 = new MyBox(Origin.x+5, Origin.y+5, 30, 210, utils::color_conv(0x77,0x77,0x77), 0x000000);
		box1->draw();
		delete box1;
	}
	
	{
		list_y += 35;
		MyTriangle::Point2D Origin(10,list_y);
		/*MyText* text1 = new MyText("Kronehit", Origin.x+10, Origin.y+13, utils::color_conv(0xd0,0xff,0xda), 0x000000);
		text1->draw();
		delete text1;*/
		
		MyBox* box1 = new MyBox(Origin.x+5, Origin.y+5, 30, 210, utils::color_conv(0x77,0x77,0x77), 0x000000);
		box1->draw();
		delete box1;
	}
	
	{
		list_y += 35;
		MyTriangle::Point2D Origin(10,list_y);
		/*MyText* text1 = new MyText("FM 4", Origin.x+10, Origin.y+13, utils::color_conv(0xd0,0xff,0xda), 0x000000);
		text1->draw();
		delete text1;*/
		
		MyBox* box1 = new MyBox(Origin.x+5, Origin.y+5, 30, 210, utils::color_conv(0x77,0x77,0x77), 0x000000);
		box1->draw();
		delete box1;
	}
	
	{
		list_y += 35;
		MyTriangle::Point2D Origin(10,list_y);
		/*MyText* text1 = new MyText("Antenne Steiermark", Origin.x+10, Origin.y+13, utils::color_conv(0xd0,0xff,0xda), 0x000000);
		text1->draw();
		delete text1;*/
		
		MyBox* box1 = new MyBox(Origin.x+5, Origin.y+5, 30, 210, utils::color_conv(0x77,0x77,0x77), 0x000000);
		box1->draw();
		delete box1;
	}
	
	{
		list_y += 35;
		MyTriangle::Point2D Origin(10,list_y);
		/*MyText* text1 = new MyText("Pirate Radio", Origin.x+10, Origin.y+13, utils::color_conv(0xd0,0xff,0xda), 0x000000);
		text1->draw();
		delete text1;*/
		
		MyBox* box1 = new MyBox(Origin.x+5, Origin.y+5, 30, 210, utils::color_conv(0x77,0x77,0x77), 0x000000);
		box1->draw();
		delete box1;
	}
	
	{
		list_y += 35;
		MyTriangle::Point2D Origin(10,list_y);
		/*MyText* text1 = new MyText("EVE Radio", Origin.x+10, Origin.y+13, utils::color_conv(0xd0,0xff,0xda), 0x000000);
		text1->draw();
		delete text1;*/
		
		MyBox* box1 = new MyBox(Origin.x+5, Origin.y+5, 30, 210, utils::color_conv(0x77,0x77,0x77), 0x000000);
		box1->draw();
		delete box1;
	}
}

