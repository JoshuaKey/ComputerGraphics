#include "Canvas.h"

Canvas::Canvas(uint32_t width, uint32_t height) :
	WIDTH(width),
	HEIGHT(height),
	pixels(new COLORREF[WIDTH * HEIGHT])
{

}

Canvas::~Canvas()
{

}

void Canvas::PutPixel(uint32_t x, uint32_t y, COLORREF color)
{
	if (x >= WIDTH || y >= HEIGHT)
	{
		return;
	}

	pixels[y * WIDTH + x] = color;
}
