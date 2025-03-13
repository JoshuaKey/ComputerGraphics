#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

class Window;

class WindowFactory
{
public:
	static Window* Create();

private:

};

