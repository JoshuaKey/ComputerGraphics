#include "WindowFactory.h"
#include <Windows.h>
#include <tchar.h> 
#include "WindowsUtility.h"
#include "Window.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

Window* WindowFactory::Create()
{
	const TCHAR CLASS_NAME[] = TEXT("DefaultWindow");
	const TCHAR WINDOW_TITLE[] = TEXT("Sample Window");

	HINSTANCE currentInstance = WindowsUtility::GetCurrentInstance();

	// A Window Class acts as a template for how all Windows should be created when used.
	WNDCLASS windowClass = { };
	windowClass.lpfnWndProc = DefWindowProc;         // Windows Procedure Callback function
	windowClass.hInstance = currentInstance;   // Window "Instance"
	windowClass.lpszClassName = CLASS_NAME;    // Window Class Name; used to refer to the class

	// Register the Widndow Class so it can be used to "create" Windows.
	if (!RegisterClass(&windowClass))
	{
		MessageBox(NULL, TEXT("Call to RegisterClassEx failed!"), TEXT("Windows Desktop Guided Tour"), NULL);

		return nullptr;
	}

	HWND handle = CreateWindowEx(
		0,									// Optional window styles.
		CLASS_NAME,							// Window class
		WINDOW_TITLE,						// Window text
		WS_OVERLAPPEDWINDOW,	// Window style

		CW_USEDEFAULT,	// X Position
		CW_USEDEFAULT,	// Y Position
		500,	// Width
		500,	// Height

		NULL,				// Parent window    
		NULL,				// Menu
		currentInstance,	// Instance handle
		NULL				// Additional application data
	);
	if (handle == NULL)
	{
		return nullptr;
	}
	
	return new Window(WINDOW_TITLE, CLASS_NAME, currentInstance, handle);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    TCHAR greeting[] = TEXT("Hello, Windows desktop!");

    switch (message)
    {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);

        // Here your application is laid out.
        // For this introduction, we just print out "Hello, Windows desktop!"
        // in the top left corner.
        TextOut(hdc,
            5, 5,
            greeting, _tcslen(greeting));
        // End application specific layout section.

        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }

    return 0;
}