#include "graphics.hpp"

#include <Windows.h>
#include <d2d1.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_DESTROY)
	{
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(pCmdLine);

	WNDCLASSEX windowClass;
	ZeroMemory(&windowClass, sizeof(WNDCLASSEX));
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	windowClass.hInstance = hInstance;
	windowClass.lpfnWndProc = WindowProc;
	windowClass.lpszClassName = L"MainWindow";
	windowClass.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClassEx(&windowClass);
	RECT rect = { 0, 0, 800, 800 };
	AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, false, WS_EX_OVERLAPPEDWINDOW);
	HWND hwnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, L"MainWindow", L"D2D", WS_OVERLAPPEDWINDOW, 100, 100, rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, hInstance, NULL);
	ShowWindow(hwnd, nCmdShow);

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	msg.message = WM_NULL;
	
	Rect r{0.0f, 0.0f, 20.0f, 20.0f};

	Graphics::initialize(hwnd);
	RenderTexture rt(200, 200);
	rt.beginDraw();
	Graphics::clear({0, 255, 0});
	r.draw({255, 0, 255});
	rt.endDraw();

	RenderTexture rtCopy(rt);


	while (true)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				return 0;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		r.pos.x++;
		r.size.y++;

		Graphics::beginDraw();
		Graphics::clear({0, 100, 255});

		rt.draw();
		rtCopy.draw({400, 400, 300, 300});
		r.draw({255, 0, 0, 125});

		Graphics::endDraw();

	}

	Graphics::finalize();
	UnregisterClass(windowClass.lpszClassName, hInstance);
	return 0;
}