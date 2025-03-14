#include "WindowsUtility.h"
#include "WindowFactory.h"
#include "Window.h"
#include <iostream>

int main()
{
	TCHAR const WINDOW_TITLE[] = TEXT("Sample Window");

	std::unique_ptr<Window> window = WindowFactory::Create(WINDOW_TITLE);
	if (!window || !window->IsValid())
	{
		WindowsUtility::DisplayError(TEXT("Could not create Window!"));

		return -1;
	}

	window->Show();
	window->Update();

	// Application Loop
	ULONG frame = 0;
	bool exiting = false;
	do
	{
		frame++;

		std::cout << "Frame: " << frame << std::endl;

		// Process Message Queue
		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			std::cout << "Processing Message: " << msg.message << std::endl;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	} 
	while (!exiting);

    return 0;
}
