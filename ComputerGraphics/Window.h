#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include "Renderer.h"

class WindowFactory;

class Window
{
	friend class WindowFactory;

private:
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
	//Window(Window const& window) noexcept :
	//	m_windowTitle(window.m_windowTitle),
	//	m_renderer(window.m_renderer),
	//	m_instanceHandle(window.m_instanceHandle),
	//	m_windowHandle(window.m_windowHandle)
	//{
	//}
	//Window(Window const&& window) noexcept :
	//	m_windowTitle(window.m_windowTitle),
	//	m_renderer(window.m_renderer),
	//	m_instanceHandle(window.m_instanceHandle),
	//	m_windowHandle(window.m_windowHandle)
	//{
	//}
	~Window()
	{
		m_instanceHandle = NULL;
		m_windowHandle = NULL;
	}

public:
	void Show();
	void Hide();
	void Update();

	bool IsValid() const;

private:
	// Static Windows Procedure Callback for Message handling.
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	// Instanced Callback function for Message handling
	bool HandleMessages(UINT message, WPARAM wParam, LPARAM lParam);

public:
	TCHAR const * const m_windowTitle;

private:
	Renderer m_renderer; 

	HINSTANCE m_instanceHandle; // Handle to the "instance" that created this window (Executable / DLL)
	HWND m_windowHandle;		// Handle to the actual Window
	
	friend class WindowFactory;
};
