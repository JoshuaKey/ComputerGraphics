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
		MessageBoxW(NULL, TEXT("Could not create Window!"), TEXT("ERROR"), MB_OK);
		return -1;
	}

	window->Show();
	window->Update();

	// Message Loop for Windows Events
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;

    return 0;
}
