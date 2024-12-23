#include "window.hpp"
#include "utils.hpp"

#include <windows.h>

bool Window::forceClose = false;
bool Window::fullscreen = false;
void (*Window::resizedCallback)(unsigned int width, unsigned int height) = nullptr;

WNDCLASSEX Window::windowClass;
HWND Window::hwnd;
MSG Window::msg;
LONG_PTR Window::savedStyle;
RECT Window::savedRect;

unsigned int Window::width = 0;
unsigned int Window::height = 0;

unsigned int Window::scale = 1;
unsigned int Window::offsetX = 0;
unsigned int Window::offsetY = 0;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{	
	switch (uMsg)
	{
		case WM_CLOSE:
		{
			DestroyWindow(hwnd);
			return 0;
		}

		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		case WM_SIZE:
		{
			RECT rc;

			if (Window::fullscreen)
				GetWindowRect(Window::hwnd, &rc);
			else
				GetClientRect(Window::hwnd, &rc);

			Window::width = rc.right - rc.left;
			Window::height = rc.bottom - rc.top;

			if (Window::resizedCallback)
				Window::resizedCallback(Window::width, Window::height);

			return 0;
		}
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void Window::initialize(int width, int height, const std::string &windowName, HINSTANCE hInstance, int nCmdShow)
{
	// Initilize COM
	CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);

	Window::width = width;
	Window::height = height;

	ZeroMemory(&windowClass, sizeof(WNDCLASSEX));
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	windowClass.hInstance = hInstance;
	windowClass.lpfnWndProc = WindowProc;
	windowClass.lpszClassName = L"MainWindow";
	windowClass.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClassEx(&windowClass);
	RECT rect = { 0, 0, long{Window::width}, long{Window::height} };
	AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, false, WS_EX_OVERLAPPEDWINDOW);

	// Calculate window position to center it on the screen
	int screenWidth  = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	int posX = (screenWidth - (rect.right - rect.left)) / 2;
	int posY = (screenHeight - (rect.bottom - rect.top)) / 2;

	Window::hwnd = CreateWindowEx(
		WS_EX_OVERLAPPEDWINDOW, L"MainWindow", toWideString(windowName).c_str(), WS_OVERLAPPEDWINDOW,
		posX, posY, rect.right - rect.left, rect.bottom - rect.top,
		NULL, NULL, hInstance, NULL
	);
	
	ShowWindow(Window::hwnd, nCmdShow);
	SetForegroundWindow(Window::hwnd);
	SetActiveWindow(Window::hwnd);
	SetFocus(Window::hwnd);
	SetCursor(LoadCursor(NULL, IDC_ARROW));

	savedStyle = GetWindowLongPtr(hwnd, GWL_STYLE);
	GetWindowRect(hwnd, &savedRect);

	ZeroMemory(&Window::msg, sizeof(MSG));
	Window::msg.message = WM_NULL;
}

void Window::finalize()
{
	UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);

	// Unitilize COM
	CoUninitialize();
}

bool Window::shouldClose()
{
	if (forceClose)
		return true;

	while (PeekMessage(&Window::msg, NULL, 0, 0, PM_REMOVE))
	{
		if (Window::msg.message == WM_QUIT)
			return true;

		TranslateMessage(&Window::msg);
		DispatchMessage(&Window::msg);
	}

	return false;
}

void Window::toggleFullscreen()
{
	LONG_PTR style = GetWindowLongPtr(Window::hwnd, GWL_STYLE);
	Window::fullscreen = (style & WS_POPUP) == 0;

	if (Window::fullscreen)
	{
		GetWindowRect(Window::hwnd, &Window::savedRect);

		SetWindowLong(Window::hwnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
		SetWindowPos(Window::hwnd, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_FRAMECHANGED);
	}
	else
	{
		// Calculate window position to center it on the screen
		int screenWidth = GetSystemMetrics(SM_CXSCREEN);
		int screenHeight = GetSystemMetrics(SM_CYSCREEN);
		int posX = (screenWidth - (Window::savedRect.right - Window::savedRect.left)) / 2;
		int posY = (screenHeight - (Window::savedRect.bottom - Window::savedRect.top)) / 2;

		// Re-Center window
		SetWindowLongPtr(Window::hwnd, GWL_STYLE, Window::savedStyle & ~WS_POPUP);
		SetWindowPos(Window::hwnd, HWND_NOTOPMOST,
			posX, posY,
			Window::savedRect.right - Window::savedRect.left, Window::savedRect.bottom - Window::savedRect.top,
			SWP_FRAMECHANGED | SWP_NOZORDER | SWP_NOOWNERZORDER);
	}
}
