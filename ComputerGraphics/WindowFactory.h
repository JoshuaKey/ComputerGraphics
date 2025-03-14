#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <memory>

class Window;

class WindowFactory
{
public:
	static std::unique_ptr<Window> Create(const TCHAR* WINDOW_TITLE);
};

