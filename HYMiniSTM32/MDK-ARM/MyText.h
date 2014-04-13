#ifndef MYTEXT_H
#define MYTEXT_H

extern "C"
{
    #include "stm32f10x.h"
    #include "TouchPanel.h"
    #include "systick.h"
    #include "GLCD.h"
}
#include <stdlib.h>
#include <string.h>

class MyText
{
public:
    MyText(char* text, uint16_t x, uint16_t y, uint16_t fg_col, uint16_t bg_col);
    ~MyText();
    void setText(char* text);
    void setFGColor(uint16_t col);
    void setBGColor(uint16_t col);
    void setPosition(uint16_t x, uint16_t y);
    void draw();
    void undraw();

private:
    char* m_text_;
    uint16_t m_fg_color_;
    uint16_t m_bg_color_;
    uint16_t m_x_;
    uint16_t m_y_;
};

#endif
