#include "Window.h"
#include <objidl.h>
#include <gdiplus.h>
#include <uxtheme.h>
#include "WindowsUtility.h"

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
	constexpr UINT BITMAP_WIDTH = 125;
	constexpr UINT BITMAP_HEIGHT = 20;

	static HBITMAP bitmapHandle { NULL };
	static HDC bitmapDeviceContext { NULL };
	static void* data = nullptr;

	switch (message)
	{
		case WM_CREATE:
		{
			// Create Device Context for Bitmap
			HDC digitalContext = GetDC(m_windowHandle);
			bitmapDeviceContext = CreateCompatibleDC(digitalContext);
			ReleaseDC(m_windowHandle, digitalContext);
 
			// Create Bitmap
			bitmapHandle = WindowsUtility::CreateBitmap(BITMAP_WIDTH, BITMAP_HEIGHT, bitmapDeviceContext, &data);

			SelectObject(bitmapDeviceContext, bitmapHandle);

			// Set Bitmap Color Data
			for (UINT i = 0; i < BITMAP_WIDTH * BITMAP_HEIGHT; ++i)
			{
				((unsigned char*)data)[(i * 4) + 0] = 191;
				((unsigned char*)data)[(i * 4) + 1] = 255;
				((unsigned char*)data)[(i * 4) + 2] = 232;
				((unsigned char*)data)[(i * 4) + 3] = 0;
			}

			BufferedPaintInit();
			
			return 0;
		}
		case WM_ERASEBKGND:
		{
			return 1;
		}
		case WM_PAINT:
		{
			// Get Window Digital Context (Paint Object)
			HDC digitalContext = GetDC(m_windowHandle);

			// Get Window Draw Region
			RECT clientRegion { 0 };
			GetClientRect(m_windowHandle, &clientRegion);

			// Start Buffered Paint
			// This acts like using a Double Buffer Bitmap
			HDC bufferedDC { 0 };
			HPAINTBUFFER paintBuffer = BeginBufferedPaint(digitalContext, &clientRegion, BPBF_COMPATIBLEBITMAP, NULL, &bufferedDC);
			if (paintBuffer == NULL || bufferedDC == NULL)
			{
				WindowsUtility::DisplayLastError(TEXT("Failed to Begin Buffered Paint"));

				return 0;
			}
			
			// Fill a Rect with a custom color
			COLORREF color = RGB(255, 0, 0);
			HBRUSH colorBrush = CreateSolidBrush(color);
			FillRect(bufferedDC, &clientRegion, colorBrush);

			// Fill a Rect using the DC's current Brush
			SelectObject(bufferedDC, GetStockObject(DC_BRUSH));
			SetDCBrushColor(bufferedDC, RGB(00, 0xff, 00));
			PatBlt(bufferedDC, 100, 100, 10, 10, PATCOPY);

			// Draw a Bitmap to the Digital Context
			if (bitmapHandle != NULL)
			{
				BITMAP bitmap;
				GetObject(bitmapHandle, sizeof(BITMAP), &bitmap);

				BitBlt(bufferedDC, 100, 200, bitmap.bmWidth, bitmap.bmHeight, bitmapDeviceContext, 0, 0, SRCCOPY);
			}

			DeleteObject(colorBrush);

			EndBufferedPaint(paintBuffer, true);
			ReleaseDC(m_windowHandle, digitalContext);

			ValidateRect(m_windowHandle, NULL);

			return 0;
		}
		case WM_LBUTTONDOWN:
		{
			if(!data) 
			{ 
				return -1;
			}

			for (UINT i = 0; i < BITMAP_WIDTH * BITMAP_HEIGHT; ++i)
			{
				((unsigned char*)data)[(i * 4) + 0] -= 5;
				((unsigned char*)data)[(i * 4) + 1] -= 5; 
				((unsigned char*)data)[(i * 4) + 2] -= 5;
				((unsigned char*)data)[(i * 4) + 3] = 0;
			}

			return 0;
		}
		case WM_CLOSE:
		case WM_DESTROY:
		{
			BufferedPaintUnInit();

			Destroy();

			return 0;
		}
	}
	
	return -1;
}
