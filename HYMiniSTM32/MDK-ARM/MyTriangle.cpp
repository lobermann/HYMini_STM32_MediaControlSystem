#include "MyTriangle.h"

#include "MyUtils.h"

/**
  * @brief  Constructs the MyTriangle Object
	* @note   The points must be provided in a clockwise manner
  * @param  origin Point2D of the origin of the triangle
	* @param  a The first Point2D of the triangle
	* @param  b The second Point2D of the triangle
	* @param  c The third Point2D of the triangle
  * @param  fg_col The 16bit foreground color
	* @param  bg_col The 16bit background color
  * @retval None
  */
MyTriangle::MyTriangle(Point2D origin, Point2D a, Point2D b, Point2D c, uint16_t fg_col, uint16_t bg_col)
{
	m_fg_color_ = fg_col;
	m_bg_color_ = bg_col;
	
	m_a_ = a;
	m_a_.x += origin.x;
	m_a_.y += origin.y;
	
	m_b_ = b;
	m_b_.x += origin.x;
	m_b_.y += origin.y;
	
	m_c_ = c;
	m_c_.x += origin.x;
	m_c_.y += origin.y;
}

/**
  * @brief  Destructs the MyTriangle Object
	* @param  None
  * @retval None
  */
MyTriangle::~MyTriangle()
{
	
}

/**
  * @brief  A 2d floating point arithmetic orientation predicate
	* @param  a A first 2d point on a line
  * @param  a A second  2d point on a line
  * @param  a A 2d point
  * @retval This predicate is
	*         - positive if a, b and c occur in counterclockwise orientation.
	*         - zero if a, b and c are collinear.
	*         - negative if a, b and c occur in clockwise orientation.
  */
int32_t MyTriangle::orient2d(const Point2D& a, const Point2D& b, const Point2D& c)
{
	return (b.x - a.x) * (c.y - a.y) - (b.y - a.y)*(c.x - a.x);
}

/**
  * @brief  Returns the smallest of 3 32bit values
	* @param  a The first value
	* @param  b The second value
	* @param  c The third value
  * @retval The smalles of the given values
  */
int32_t MyTriangle::min3(int32_t& a, int32_t& b, int32_t& c)
{
	if( a <= b && a <= c) return a;
	if( b <= a && b <= c) return b;
	else return c;
}

/**
  * @brief  Returns the largest of 3 32bit values
	* @param  a The first value
	* @param  b The second value
	* @param  c The third value
  * @retval The largest of the given values
  */
int32_t MyTriangle::max3(int32_t& a, int32_t& b, int32_t& c)
{
	if( a >= b && a >= c) return a;
	if( b >= a && b >= c) return b;
	else return c;
}

/**
  * @brief  Draws the Triangle
  * @param  None
  * @retval None
  */
void MyTriangle::draw()
{
	drawTri(m_fg_color_);
}

/**
  * @brief  Undraws the Triangle
  * @param  None
  * @retval None
  */
void MyTriangle::undraw()
{
	drawTri(m_bg_color_);
}

/**
  * @brief  Rasterizes the Triangle in the given color
  * @param  color The 16bit color of the Triangle
  * @retval None
  */
void MyTriangle::drawTri(uint16_t color)
{
    // Compute triangle bounding box
    int minX = min3(m_a_.x, m_b_.x, m_c_.x);
    int minY = min3(m_a_.y, m_b_.y, m_c_.y);
    int maxX = max3(m_a_.x, m_b_.x, m_c_.x);
    int maxY = max3(m_a_.y, m_b_.y, m_c_.y);

    // Clip against screen bounds
		if(minX < 0) minX = 0;
		if(minY < 0) minY = 0;
		if(maxX < (SCREEN_WITH - 1) ) maxX = SCREEN_WITH - 1;
		if(maxY < (SCREEN_HEIGHT - 1) ) maxY = SCREEN_HEIGHT - 1;

    // Rasterize
    Point2D p;
    for (p.y = minY; p.y <= maxY; p.y++) 
		{
        for (p.x = minX; p.x <= maxX; p.x++) 
				{
            // Determine barycentric coordinates
            int w0 = orient2d(m_b_, m_c_, p);
            int w1 = orient2d(m_c_, m_a_, p);
            int w2 = orient2d(m_a_, m_b_, p);

            // If p is on or inside all edges, render pixel.
            if (w0 >= 0 && w1 >= 0 && w2 >= 0)
						{
							LCD_SetPoint(p.x, p.y, color);     
						}							
        }
    }
}
