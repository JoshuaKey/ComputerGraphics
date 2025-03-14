#include "WindowFactory.h"
#include <Windows.h>
#include <tchar.h> 
#include "WindowsUtility.h"
#include "Window.h"

namespace
{
	TCHAR const CLASS_NAME[] = TEXT("DefaultWindow");
}

std::unique_ptr<Window> WindowFactory::Create(TCHAR const * const WINDOW_TITLE)
{
	DWORD result = NULL;
	HINSTANCE instanceHandle = WindowsUtility::GetCurrentInstance();

	// Check if Window Class is already created
	WNDCLASSEX windowClass = { };
	windowClass.cbSize = sizeof(WNDCLASSEX);

	if (!GetClassInfoEx(instanceHandle, CLASS_NAME, &windowClass))
	{
		// Otherwise fill out data
		windowClass.lpfnWndProc = Window::WndProc;	// Windows Procedure Callback function
		windowClass.hInstance = instanceHandle;		// Window "Instance"
		windowClass.lpszClassName = CLASS_NAME;		// Window Class Name; used to refer to the class

		// Register the Window Class so it can be used to "create" a Window.
		if (!RegisterClassEx(&windowClass))
		{
			WindowsUtility::DisplayLastError(TEXT("Call to RegisterClassEx failed!"));

			return nullptr;
		}
	}

	// Create Window
	HWND windowHandle = CreateWindowEx(
		0,						// Optional window styles.
		CLASS_NAME,				// Window class
		WINDOW_TITLE,			// Window text
		WS_OVERLAPPEDWINDOW,	// Window style

		CW_USEDEFAULT,			// X Position
		CW_USEDEFAULT,			// Y Position
		500,					// Width
		500,					// Height

		NULL,					// Parent window    
		NULL,					// Menu
		instanceHandle,			// Instance handle
		NULL					// Additional application data
	);

	// Check Window Handle
	if (!windowHandle)
	{
		WindowsUtility::DisplayLastError(TEXT("Call to CreateWindowEx failed!"));

		return nullptr;
	}

	std::unique_ptr<Window> window(new Window(WINDOW_TITLE, instanceHandle, windowHandle));

	// Set the Userdata so the Callback function knows which Window instance is being called
	SetWindowLongPtrW(windowHandle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window.get()));

	return window;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	
	
	//PAINTSTRUCT ps;
 //   HDC hdc;
 //   TCHAR greeting[] = TEXT("Hello, Windows desktop!");

 //   switch (message)
 //   {
 //   case WM_PAINT:
 //       hdc = BeginPaint(hWnd, &ps);

 //       // Here your application is laid out.
 //       // For this introduction, we just print out "Hello, Windows desktop!"
 //       // in the top left corner.
 //       TextOut(hdc,
 //           5, 5,
 //           greeting, _tcslen(greeting));
 //       // End application specific layout section.

 //       EndPaint(hWnd, &ps);
 //       break;
 //   case WM_DESTROY:
 //       PostQuitMessage(0);
 //       break;
 //   default:
 //       return DefWindowProc(hWnd, message, wParam, lParam);
 //       break;
 //   }

    return 0;
}