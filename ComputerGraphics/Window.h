#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include "Renderer.h"

class WindowFactory;
class Canvas;

class Window
{
	friend class WindowFactory;

	Window(TCHAR const * const windowTitle,
		HINSTANCE const instance,
		HWND const window) :
		m_windowTitle(windowTitle),
		m_canvas(nullptr),
		m_instanceHandle(instance),
		m_windowHandle(window)
	{ 
	}
public:
	~Window()
	{
		m_canvas = nullptr;
		m_instanceHandle = NULL;
		m_windowHandle = NULL;
	}

public:
	void Show();
	void Hide();
	void Update();
	void Invalidate();
	void Destroy();

	bool IsValid() const;

	void SetCanvas(Canvas const* const canvas) { m_canvas = canvas; }
	Canvas const* GetCanvas() { return m_canvas; }

private:
	// Static Windows Procedure Callback for Message handling.
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	// Instanced Callback function for Message handling
	LRESULT HandleMessages(UINT message, WPARAM wParam, LPARAM lParam);

	void SetInstanceHandle(HINSTANCE const handle) { m_instanceHandle = handle; }
	void SetWindowHandle(HWND const handle) { m_windowHandle = handle; }

private:
	TCHAR const* const m_windowTitle;

	Canvas const * m_canvas;

	HINSTANCE m_instanceHandle; // Handle to the "instance" that created this window (Executable / DLL)
	HWND m_windowHandle;		// Handle to the actual Window
};
