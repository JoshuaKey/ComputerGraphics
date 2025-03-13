#include "WindowsUtility.h"
#include "WindowFactory.h"
#include "Window.h"
#include <iostream>

int main()
{
	Window* window = WindowFactory::Create();
	if (!window || !window->IsValid())
	{
		MessageBoxW(NULL, TEXT("ERROR"), TEXT("Could not create Window!"), MB_OK | MB_ICONINFORMATION);
		return -1;
	}

	window->Show();
	window->Update();

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;

    return 0;
}
