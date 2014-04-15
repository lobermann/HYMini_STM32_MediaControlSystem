#include "Display1.h"

#include "MyUtils.h"
#include "MyBox.h"
#include "MyTriangle.h"
#include "MyText.h"

Display1::Display1()
{
	
}

Display1::~Display1()
{
	
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
        utils::USARTWriteTouch(section);
        
        //Short delay after sending, to prevent multiple touches per press
        //TODO: Find a better solution then a sleep
        delay_ms(100);
    }
}

void Display1::write_header(uint16_t color, char* text)
{
    //First clear the area
    utils::drawBox(6, 6, 228, 18, 0x000000);
    
    //Limit the text to 30 chars
    if(strlen(text) > 30) 
        text[30] = 0;
    
    MyText* header = new MyText(text, 10, 8, color, 0x000000);
    header->draw();
    delete header;
}

void Display1::write_slot(uint8_t slot, uint16_t color, char* text)
{
    //10, 30
    if(slot == 1)
    {
        //First clear the area
        utils::drawBox(15+1, 35+1, 208, 28, 0x000000);
        MyText* slottext = new MyText(text, 20, 30+13, color, 0x000000);
        slottext->draw();
        delete slottext;
    }
    else if(slot == 2)
    {
        //First clear the area
        utils::drawBox(15+1, 70+1, 208, 28, 0x000000);
        MyText* slottext = new MyText(text, 20, 65+13, color, 0x000000);
        slottext->draw();
        delete slottext;
    }
    else if(slot == 3)
    {
        //First clear the area
        utils::drawBox(15+1, 105+1, 208, 28, 0x000000);
        MyText* slottext = new MyText(text, 20, 100+13, color, 0x000000);
        slottext->draw();
        delete slottext;
    }
    else if(slot == 4)
    {
        //First clear the area
        utils::drawBox(15+1, 140+1, 208, 28, 0x000000);
        MyText* slottext = new MyText(text, 20, 135+13, color, 0x000000);
        slottext->draw();
        delete slottext;
    }
    else if(slot == 5)
    {
        //First clear the area
        utils::drawBox(15+1, 175+1, 208, 28, 0x000000);
        MyText* slottext = new MyText(text, 20, 170+13, color, 0x000000);
        slottext->draw();
        delete slottext;
    }
    else if(slot == 6)
    {
        //First clear the area
        utils::drawBox(15+1, 210+1, 208, 28, 0x000000);
        MyText* slottext = new MyText(text, 20, 205+13, color, 0x000000);
        slottext->draw();
        delete slottext;
    }
    else
    {
        //We only have six slots, so ignore the rest
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
	
    uint16_t list_y = 30;
    {
        MyTriangle::Point2D Origin(10,list_y);
        MyBox* box1 = new MyBox(Origin.x+5, Origin.y+5, 30, 210, utils::color_conv(0x77,0x77,0x77), 0x000000);
        if(draw) box1->draw();
        else box1->undraw();
        delete box1;
    }
	
    {
        list_y += 35;
        MyTriangle::Point2D Origin(10,list_y);

        MyBox* box1 = new MyBox(Origin.x+5, Origin.y+5, 30, 210, utils::color_conv(0x77,0x77,0x77), 0x000000);
        if(draw) box1->draw();
        else box1->undraw();
        delete box1;
    }
	
    {
        list_y += 35;
        MyTriangle::Point2D Origin(10,list_y);

        MyBox* box1 = new MyBox(Origin.x+5, Origin.y+5, 30, 210, utils::color_conv(0x77,0x77,0x77), 0x000000);
        if(draw) box1->draw();
        else box1->undraw();
        delete box1;
    }
	
    {
        list_y += 35;
        MyTriangle::Point2D Origin(10,list_y);
        
        MyBox* box1 = new MyBox(Origin.x+5, Origin.y+5, 30, 210, utils::color_conv(0x77,0x77,0x77), 0x000000);
        if(draw) box1->draw();
        else box1->undraw();
        delete box1;
    }
	
    {
        list_y += 35;
        MyTriangle::Point2D Origin(10,list_y);
        
        MyBox* box1 = new MyBox(Origin.x+5, Origin.y+5, 30, 210, utils::color_conv(0x77,0x77,0x77), 0x000000);
        if(draw) box1->draw();
        else box1->undraw();
        delete box1;
    }
	
    {
        list_y += 35;
        MyTriangle::Point2D Origin(10,list_y);
        
        MyBox* box1 = new MyBox(Origin.x+5, Origin.y+5, 30, 210, utils::color_conv(0x77,0x77,0x77), 0x000000);
        if(draw) box1->draw();
        else box1->undraw();
        delete box1;
    }
}

