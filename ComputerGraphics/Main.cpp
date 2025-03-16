#include <Windows.h>
#include <iostream>
#include "WindowsUtility.h"
#include "WindowFactory.h"
#include "Window.h"
#include "Canvas.h"
#include "Scene.h"
#include "Rectangle.h"
#include "Camera.h"

int main()
{
	/// CONSTANTS
	constexpr uint32_t WINDOW_WIDTH = 1280;
	constexpr uint32_t WINDOW_HEIGHT = 720;
	constexpr TCHAR WINDOW_TITLE[] = TEXT("Sample Window");

	/// SCENE DECLARATION
	Scene scene;

	Camera & camera = scene.GetCamera();
	camera.m_transform = { 0.0f, 0.0f, 0.0f };

	{
		Rect* rect = static_cast<Rect*>(scene.AddObject<Rect>());
		rect->m_transform = { 0.0f, 0.0f, -5.0f };
		rect->SetHeight(1000.0f);
		rect->SetWidth(1000.0f);
		rect->SetColor({ 0, 255, 0 });
	}

	{
		Rect* rect = static_cast<Rect*>(scene.AddObject<Rect>());
		rect->m_transform = { 0.0f, 150.0f, -4.0f };
		rect->SetHeight(1000.0f);
		rect->SetWidth(10.0f);
		rect->SetColor({ 128, 255, 128 });
	}

	{
		Rect* rect = static_cast<Rect*>(scene.AddObject<Rect>());
		rect->m_transform = { 300.0f, 0.0f, -3.0f };
		rect->SetHeight(900.0f);
		rect->SetWidth(10.0f);
		rect->SetColor({ 0, 0, 255 });
	}

	{
		Rect* rect = static_cast<Rect*>(scene.AddObject<Rect>());
		rect->m_transform = { -50.0f, -250.0f, -2.0f };
		rect->SetHeight(500.0f);
		rect->SetWidth(175.0f);
		rect->SetColor({ 0, 255, 255 });
	}

	/// RENDERING
	Canvas canvas(WINDOW_WIDTH, WINDOW_HEIGHT);

	Renderer renderer(&canvas, &scene);

	/// WINDOW CREATION
	std::unique_ptr<Window> window = WindowFactory::Create(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT);
	if (!window || !window->IsValid())
	{
		WindowsUtility::DisplayError(TEXT("Could not create Window!"));

		return -1;
	}

	window->SetCanvas(&canvas);
	window->Show();
	window->Update();

	/// APPLICATION LOOP
	ULONG frame = 0;
	while (window->IsValid())
	{
		frame++;
		std::cout << "Frame: " << frame << std::endl;

		renderer.Render();
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
