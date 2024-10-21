#pragma once

#include <string>
#include <windef.h>
#include <winuser.h>
#include <functional>

class Window
{
private:
	Window() = default;
	~Window() = default;

public:
	static void initialize(int width, int height, const std::string &windowName, HINSTANCE hInstance, int nCmdShow);
	static void finalize();

	static bool shouldClose();

	static std::function<void(unsigned int, unsigned int)> resizedCallback;
	
	static WNDCLASSEX windowClass;
	static HWND hwnd;
	static MSG msg;
	static LONG_PTR savedStyle;
	static RECT savedRect;

	static int width;
	static int height;

	static unsigned int scale;
	static unsigned int offsetX;
	static unsigned int offsetY;
};
