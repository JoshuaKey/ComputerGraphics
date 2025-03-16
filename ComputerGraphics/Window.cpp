#include "Window.h"
#include <objidl.h>
#include <gdiplus.h>
#include <uxtheme.h>
#include "WindowsUtility.h"
#include "Canvas.h"

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

void Window::Invalidate()
{
	InvalidateRect(m_windowHandle, NULL, false);
}

void Window::Destroy()
{
	PostQuitMessage(0);

	m_instanceHandle = NULL;
	m_windowHandle = NULL;
}

bool Window::IsValid() const
{
	return m_windowHandle != NULL;
}

LRESULT Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	Window* window = nullptr;

	// WM_CREATE does not have access to USERDATA,
	// Luckily, it is passed a CREATESTRUCT which can pass additional Application Data 
	if (message == WM_CREATE)
	{
		CREATESTRUCT * createData = reinterpret_cast<CREATESTRUCTW*>(lParam);
		window = reinterpret_cast<Window*>(createData->lpCreateParams);

		// We also have to set Window Handle manually as WindowFactory has not set it yet.
		window->SetWindowHandle(hWnd);
	}
	// Retrieve Window from UserData
	else
	{
		// Get Userdata, cast to Window Pointer, check Validity
		LONG_PTR data = GetWindowLongPtr(hWnd, GWLP_USERDATA);
		window = reinterpret_cast<Window*>(data);
	}

	// Pass Message to Window Instance
	if (window && window->IsValid())
	{
		LRESULT result = window->HandleMessages(message, wParam, lParam);
		if (result != -1)
		{
			return result;
		}
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT Window::HandleMessages(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_CREATE:
		{
			return 0;
		}
		case WM_ERASEBKGND:
		{
			return 1;
		}
		case WM_PAINT:
		{
			if (!m_canvas) { return -1; }

			// Get Window Digital Context (Paint Object)
			HDC digitalContext = GetDC(m_windowHandle);

			// Get Window Draw Region
			RECT clientRegion { 0 };
			GetClientRect(m_windowHandle, &clientRegion);

			// Create DC and Bitmap for Canvas
			HDC canvasDC = CreateCompatibleDC(digitalContext);
			HBITMAP canvasBitmap = CreateCompatibleBitmap(digitalContext, m_canvas->WIDTH, m_canvas->HEIGHT);
			
			BITMAPINFO bmi;
			bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			bmi.bmiHeader.biWidth = m_canvas->WIDTH;
			bmi.bmiHeader.biHeight = m_canvas->HEIGHT;
			bmi.bmiHeader.biPlanes = 1;
			bmi.bmiHeader.biBitCount = sizeof(COLORREF) * 8;
			bmi.bmiHeader.biCompression = DIB_RGB_COLORS;
			bmi.bmiHeader.biSizeImage = m_canvas->HEIGHT * m_canvas->WIDTH * sizeof(COLORREF);
			bmi.bmiHeader.biXPelsPerMeter = 0;
			bmi.bmiHeader.biYPelsPerMeter = 0;
			bmi.bmiHeader.biClrUsed = 0;
			bmi.bmiHeader.biClrImportant = 0;

			// Copy Canvas Pixels into Canvas DC and Bitmap
			if (SetDIBits(canvasDC, canvasBitmap, 0, m_canvas->HEIGHT, m_canvas->GetPixels(), &bmi, DIB_RGB_COLORS))
			{
				SelectObject(canvasDC, canvasBitmap);

				// Draw Canvas to Window
				// Uses Stretching function, so we don't need to reallocate Canvas Size every time Window is resized
				StretchBlt(
				// Destination DC
					digitalContext, 
					0, 
					0,
					clientRegion.right - clientRegion.left, 
					clientRegion.bottom - clientRegion.top, 
				// Source DC
					canvasDC, 
					0, 
					0,
					m_canvas->WIDTH,
					m_canvas->HEIGHT,
					SRCCOPY);
			}
			
			DeleteObject(canvasBitmap);
			DeleteDC(canvasDC);
			ReleaseDC(m_windowHandle, digitalContext);

			// Validate Painting
			ValidateRect(m_windowHandle, NULL);

			return 0;
		}
		case WM_CLOSE:
		case WM_DESTROY:
		{
			Destroy();

			return 0;
		}
	}
	
	return -1;
}
