#pragma once

#include <memory>
#include <Windows.h>
#include "Color.h"

/// <summary>
/// Canvas is a collection of "Pixels" or colors in an X and Y format.
/// It is used to draw pixels to a Window.
/// </summary>
class Canvas
{
	typedef Color ColorType;

public:
	Canvas(uint32_t width, uint32_t height) :
		WIDTH(width),
		HEIGHT(height),
		pixels(new ColorType[WIDTH * HEIGHT])
	{ }
	~Canvas() { }

public:
	void PutPixel(uint32_t x, uint32_t y, ColorType color)
	{
		{
			if (x >= WIDTH || y >= HEIGHT)
			{
				return;
			}

			pixels[y * WIDTH + x] = color;
		}
	}

	ColorType const * const GetPixels() const 
	{ 
		return pixels.get(); 
	}

	uint32_t GetColorSize() const
	{
		return sizeof(ColorType);
	}

public:
	const uint32_t WIDTH;
	const uint32_t HEIGHT;

private:
	std::unique_ptr<ColorType[]> pixels;
};

