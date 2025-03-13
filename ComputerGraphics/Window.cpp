#include "Window.h"

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

bool Window::IsValid()
{
	return m_windowHandle != NULL;
}
