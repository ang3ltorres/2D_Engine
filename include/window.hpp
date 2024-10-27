#pragma once

#include <string>
#include <windef.h>
#include <winuser.h>

class Window
{
private:
	Window() = default;
	~Window() = default;

public:
	static void initialize(int width, int height, const std::string &windowName, HINSTANCE hInstance, int nCmdShow);
	static void finalize();

	static bool shouldClose();
	static bool forceClose;

	static void toggleFullscreen();
	static bool fullscreen;

	static void (*resizedCallback)(unsigned int width, unsigned int height);
	
	static WNDCLASSEX windowClass;
	static HWND hwnd;
	static MSG msg;
	static LONG_PTR savedStyle;
	static RECT savedRect;

	static unsigned int width;
	static unsigned int height;

	static unsigned int scale;
	static unsigned int offsetX;
	static unsigned int offsetY;
};
