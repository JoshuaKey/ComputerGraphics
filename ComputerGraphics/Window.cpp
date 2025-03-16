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

			// Using BitMap Version 4 Header because it supports Color Masking.
			// This allows us to define which RGB color is where in our Bitmap Data
			// Notrmally, Windows Bitmap uses BGRA format, meaning the Red and Blue colors are swapped when using RGBA format.
			// This could be fixed by using RGBQuad, but Initializer List would still have the same problem.
			// Additionally, RGB() and COLORREF were supposed to fix it but didn't? IDK
			// Therefore, we manually change the Color Masks to use RGBA Color Format
			BITMAPV4HEADER bmv4H { 0 };
			bmv4H.bV4Size = sizeof(BITMAPV4HEADER);
			bmv4H.bV4Width = m_canvas->WIDTH;
			bmv4H.bV4Height = m_canvas->HEIGHT;
			bmv4H.bV4Planes = 1;
			bmv4H.bV4BitCount = m_canvas->GetColorSize() * 8;
			bmv4H.bV4V4Compression = BI_BITFIELDS;
			bmv4H.bV4SizeImage = m_canvas->HEIGHT * m_canvas->WIDTH * m_canvas->GetColorSize();
			bmv4H.bV4XPelsPerMeter = 0;
			bmv4H.bV4YPelsPerMeter = 0;
			bmv4H.bV4ClrUsed = 0;
			bmv4H.bV4ClrImportant = 0;
			bmv4H.bV4RedMask = 0x000000FF;
			bmv4H.bV4GreenMask = 0x0000FF00;
			bmv4H.bV4BlueMask = 0x00FF0000;
			bmv4H.bV4AlphaMask = 0xFF000000;
			bmv4H.bV4CSType = LCS_sRGB;
			bmv4H.bV4Endpoints = { 0, 0, 0 };
			bmv4H.bV4GammaRed = 0;
			bmv4H.bV4GammaGreen = 0;
			bmv4H.bV4GammaBlue = 0;

			// Copy Canvas Pixels into Canvas DC and Bitmap
			if (SetDIBits(canvasDC, canvasBitmap, 0, m_canvas->HEIGHT, m_canvas->GetPixels(), (BITMAPINFO*) & bmv4H, DIB_RGB_COLORS))
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
			
			// Cleanup
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
