#include "Window.h"
#include <objidl.h>
#include <gdiplus.h>
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
		if (window->HandleMessages(message, wParam, lParam))
		{
			return 0;
		}
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

bool Window::HandleMessages(UINT message, WPARAM wParam, LPARAM lParam)
{
	constexpr UINT BITMAP_WIDTH = 125;
	constexpr UINT BITMAP_HEIGHT = 20;

	static HBITMAP bitmapHandle { NULL };
	static HDC bitmapDeviceContext { NULL };

	switch (message)
	{
		case WM_CREATE:
		{
			// Create BitMap Info structs
			BITMAPINFOHEADER bitmapInfoHeader;
			bitmapInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
			bitmapInfoHeader.biWidth = BITMAP_WIDTH;
			bitmapInfoHeader.biHeight = BITMAP_HEIGHT;
			bitmapInfoHeader.biPlanes = 1;
			bitmapInfoHeader.biBitCount = sizeof(char) * 4 * 8;
			bitmapInfoHeader.biCompression = BI_RGB;
			bitmapInfoHeader.biSizeImage = BITMAP_WIDTH * BITMAP_HEIGHT * sizeof(char) * 4;
			bitmapInfoHeader.biClrUsed = 0;
			bitmapInfoHeader.biClrImportant = 0;

			BITMAPINFO bitmapInfo;
			bitmapInfo.bmiHeader = bitmapInfoHeader;
			bitmapInfo.bmiColors->rgbBlue = 0;
			bitmapInfo.bmiColors->rgbGreen = 0;
			bitmapInfo.bmiColors->rgbRed = 0;
			bitmapInfo.bmiColors->rgbReserved = 0;

			// Create Device Context for Bitmap
			HDC digitalContext = GetDC(m_windowHandle);
			bitmapDeviceContext = CreateCompatibleDC(digitalContext);
			ReleaseDC(m_windowHandle, digitalContext);

			// Create Bitmap
			// This will give us a Handle to our new Bitmap and will point Data to a valid array where the Bitmap Color is located
			void *data = nullptr;
			bitmapHandle = CreateDIBSection(bitmapDeviceContext, &bitmapInfo, DIB_RGB_COLORS, &data, NULL, 0);
			if (bitmapHandle == NULL || data == NULL)
			{
				WindowsUtility::DisplayLastError(TEXT("Failed to create Bitmap"));
				return true;
			}

			// Set Bitmap Color Data
			for (UINT i = 0; i < BITMAP_WIDTH * BITMAP_HEIGHT; ++i)
			{
				((unsigned char*)data)[(i * 4) + 0] = 191;
				((unsigned char*)data)[(i * 4) + 1] = 255;
				((unsigned char*)data)[(i * 4) + 2] = 232;
				((unsigned char*)data)[(i * 4) + 3] = 0;
			}
			
			return true;
		}
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC digitalContext = BeginPaint(m_windowHandle, &ps);

			/// Fill a Rect with a custom color
			COLORREF color = RGB(255, 0, 0);
			HBRUSH colorBrush = CreateSolidBrush(color);
			FillRect(digitalContext, &ps.rcPaint, colorBrush);


			/// Fill a Rect using the DC's current Brush
			SelectObject(digitalContext, GetStockObject(DC_BRUSH));
			SetDCBrushColor(digitalContext, RGB(00, 0xff, 00));
			PatBlt(digitalContext, 100, 100, 10, 10, PATCOPY);

			/// Draw a Bitmap to the Digital Context
			if (bitmapHandle != NULL)
			{
				BITMAP bitmap;
				GetObject(bitmapHandle, sizeof(BITMAP), &bitmap);

				HBITMAP oldBitmapHandle = (HBITMAP)SelectObject(bitmapDeviceContext, bitmapHandle);

				BitBlt(digitalContext, 100, 200, bitmap.bmWidth, bitmap.bmHeight, bitmapDeviceContext, 0, 0, SRCCOPY);

				SelectObject(bitmapDeviceContext, oldBitmapHandle);
			}

			EndPaint(m_windowHandle, &ps);
			return true;
		}
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return true;
		}
	}
	return false;
}
