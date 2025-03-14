#include "Window.h"

void Window::Show()
{
	ShowWindow(m_windowHandle, SW_SHOWNORMAL);
}

void Window::Hide()
{
	ShowWindow(m_windowHandle, SW_HIDE);
}

void Window::Update()
{
	UpdateWindow(m_windowHandle);
}

bool Window::IsValid() const
{
	return m_windowHandle != NULL;
}

LRESULT Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// Get Userdata, cast to Window Pointer, check Validity
	LONG_PTR data = GetWindowLongPtr(hWnd, GWLP_USERDATA);
	Window* window = reinterpret_cast<Window*>(data);
	if (window && window->IsValid())
	{
		window->HandleMessages(message, wParam, lParam);
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

bool Window::HandleMessages(UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(m_windowHandle, &ps);

			// All painting occurs here, between BeginPaint and EndPaint.

			//FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
			FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_BACKGROUND + 1));

			EndPaint(m_windowHandle, &ps);
		}
	}
	return false;
}
