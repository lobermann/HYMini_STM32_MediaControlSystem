#ifndef MYTRIANGLE_H
#define MYTRIANGLE_H

extern "C"
{
	#include "stm32f10x.h"
	#include "TouchPanel.h"
	#include "systick.h"
	#include "GLCD.h"
}

//Screen height and width for the bounding box
#define SCREEN_WITH 240
#define SCREEN_HEIGHT 320

class MyTriangle
{
public:
	//A helper class to represent a point in 2d space
	class Point2D 
	{
	public:
		Point2D(int32_t a, int32_t b) {x = a; y = b;};
		Point2D() {x = 0; y = 0;};
    int32_t x, y;
	};
	
	MyTriangle(Point2D origin, Point2D a, Point2D b, Point2D c, uint16_t fg_col, uint16_t bg_col);
	~MyTriangle();
	
	void draw();
	void undraw();
	
		
private:
	int32_t orient2d(const Point2D& a, const Point2D& b, const Point2D& c);
  void drawTri(uint16_t color);

	int32_t min3(int32_t& a, int32_t& b, int32_t& c);
	int32_t max3(int32_t& a, int32_t& b, int32_t& c);

	Point2D m_a_;
	Point2D m_b_;
	Point2D m_c_;
	Point2D m_origin_;

	uint16_t m_fg_color_;
	uint16_t m_bg_color_;

};

#endif
