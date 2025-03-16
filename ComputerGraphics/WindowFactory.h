#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <memory>

class Window;

class WindowFactory
{
public:
	static std::unique_ptr<Window> Create(TCHAR const * const WINDOW_TITLE, uint32_t const WINDOW_WIDTH, uint32_t const WINDOW_HEIGHT);
};

