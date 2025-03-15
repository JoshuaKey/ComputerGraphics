#include "WindowsUtility.h"
#include "WindowFactory.h"
#include "Window.h"
#include <iostream>

int main()
{
	constexpr UINT WINDOW_WIDTH = 1280;
	constexpr UINT WINDOW_HEIGHT = 720;
	constexpr TCHAR WINDOW_TITLE[] = TEXT("Sample Window");

	std::unique_ptr<Window> window = WindowFactory::Create(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT);
	if (!window || !window->IsValid())
	{
		WindowsUtility::DisplayError(TEXT("Could not create Window!"));

		return -1;
	}

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
