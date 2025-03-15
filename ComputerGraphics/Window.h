#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include "Renderer.h"

class WindowFactory;

class Window
{
	friend class WindowFactory;

	Window(TCHAR const * const windowTitle,
		HINSTANCE const instance,
		HWND const window) :
		m_windowTitle(windowTitle),
		m_renderer(),
		m_instanceHandle(instance),
		m_windowHandle(window)
	{ 
	}
public:
	~Window()
	{
		m_instanceHandle = NULL;
		m_windowHandle = NULL;
	}

public:
	void Show();
	void Hide();
	void Update();
	void Invalidate();

	bool IsValid() const;

private:
	// Static Windows Procedure Callback for Message handling.
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	// Instanced Callback function for Message handling
	bool HandleMessages(UINT message, WPARAM wParam, LPARAM lParam);

	void SetInstanceHandle(HINSTANCE const handle) { m_instanceHandle = handle; }
	void SetWindowHandle(HWND const handle) { m_windowHandle = handle; }

private:
	TCHAR const* const m_windowTitle;

	Renderer m_renderer; 

	HINSTANCE m_instanceHandle; // Handle to the "instance" that created this window (Executable / DLL)
	HWND m_windowHandle;		// Handle to the actual Window
	
};
