#pragma once

#include <memory>
#include <Windows.h>

/// <summary>
/// Canvas is a collection of "Pixels" or colors in an X and Y format.
/// It is used to draw pixels to a Window.
/// </summary>
class Canvas
{
public:
	Canvas(uint32_t width, uint32_t height);
	~Canvas();

public:
	void PutPixel(uint32_t x, uint32_t y, COLORREF color);

	COLORREF const * const GetPixels() const { return pixels.get(); }

public:
	const uint32_t WIDTH;
	const uint32_t HEIGHT;

private:
	std::unique_ptr<COLORREF[]> pixels;
};

