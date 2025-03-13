#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

class Window
{
	Window(const TCHAR* windowTitle,
		const TCHAR* className,
		HINSTANCE instance,
		HWND window) :
		m_windowTitle(windowTitle),
		m_className(className),
		m_instanceHandle(instance),
		m_windowHandle(window),
		m_windowsCallback(DefWindowProc)
	{

	}

	//Window(const TCHAR* windowTitle,
	//	const TCHAR* className,
	//	HINSTANCE instance,
	//	HWND window) :
	//	m_windowTitle(windowTitle),
	//	m_className(className),
	//	m_instanceHandle(instance),
	//	m_windowHandle(window),
	//	m_windowsCallback(DefWindowProc)
	//{

	//}

public:
	void Show();
	void Hide();
	void Update();

	bool IsValid();

public:
	const TCHAR* m_windowTitle;
	const TCHAR* m_className;	// Class Name used for registering 

private:
	HINSTANCE m_instanceHandle; // Handle to the "instance" that created this window (Executable / DLL)
	HWND m_windowHandle;		// Handle to the actual Window
	WNDPROC m_windowsCallback;	// Callback function for Events

	friend class WindowFactory;
};
