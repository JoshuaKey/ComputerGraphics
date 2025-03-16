#include <Windows.h>
#include <iostream>
#include "WindowsUtility.h"
#include "WindowFactory.h"
#include "Window.h"
#include "Canvas.h"

int main()
{
	constexpr uint32_t WINDOW_WIDTH = 1280;
	constexpr uint32_t WINDOW_HEIGHT = 720;
	constexpr TCHAR WINDOW_TITLE[] = TEXT("Sample Window");

	Canvas canvas(WINDOW_WIDTH, WINDOW_HEIGHT);
	for (uint32_t y = 0; y < WINDOW_HEIGHT; y++)
	{
		for (uint32_t x = 0; x < WINDOW_WIDTH; x++)
		{
			COLORREF color = RGB(0, 0, 255);
			canvas.PutPixel(x, y, color);
		}
	}

	std::unique_ptr<Window> window = WindowFactory::Create(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT);
	if (!window || !window->IsValid())
	{
		WindowsUtility::DisplayError(TEXT("Could not create Window!"));

		return -1;
	}

	window->SetCanvas(&canvas);
	window->Show();
	window->Update();

	// Application Loop
	ULONG frame = 0;
	while (window->IsValid())
	{
		frame++;
		std::cout << "Frame: " << frame << std::endl;

		window->Invalidate();

		// Process Message Queue
		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			std::cout << "Processing Message: " << msg.message << std::endl;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	} 

    return 0;
}
