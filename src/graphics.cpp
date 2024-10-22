#include "graphics.hpp"
#include "window.hpp"

#include <Windows.h>
#include <cstdio>

// Local
LARGE_INTEGER frequency;
LARGE_INTEGER lastTime;

// Static members
double Graphics::deltaTime = 0.0f;
void *Graphics::data = nullptr;
ID2D1Factory *Graphics::factory = nullptr;
ID2D1HwndRenderTarget *Graphics::render = nullptr;
ID2D1SolidColorBrush *Graphics::brush = nullptr;
ID2D1RenderTarget *Graphics::currentTarget = nullptr;

RenderTexture::RenderTexture(const RenderTexture &other)
: RenderTexture::RenderTexture(other.texture->bitmap->GetPixelSize().width, other.texture->bitmap->GetPixelSize().height)
{	
	const D2D1_POINT_2U destination = {0, 0};
	const D2D1_RECT_U source = {0, 0, other.texture->bitmap->GetPixelSize().width, other.texture->bitmap->GetPixelSize().height};
	texture->bitmap->CopyFromBitmap(&destination, other.texture->bitmap, &source);
}

RenderTexture::RenderTexture(unsigned int width, unsigned int height)
{
	static const D2D1_PIXEL_FORMAT pixelFormat =
	{
		.format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM,
		.alphaMode = D2D1_ALPHA_MODE::D2D1_ALPHA_MODE_PREMULTIPLIED
	};

	const D2D1_SIZE_U size =
	{
		.width = width,
		.height = height
	};

	Graphics::render->CreateCompatibleRenderTarget
	(
		nullptr,
		&size,
		&pixelFormat,
		D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS::D2D1_COMPATIBLE_RENDER_TARGET_OPTIONS_NONE,
		&renderTarget
	);

	texture = new Texture();

	origin = Vector2( {0.0f, 0.0f} );
	rotation = 0.0f;
	destination = Rect( 0.0f, 0.0f, width, height );
	source = Rect( 0.0f, 0.0f, width, height );

	totalFrames = 0;
	currentFrame = 0;

	texture->width = width;
	texture->height = height;

	renderTarget->GetBitmap(&texture->bitmap);
}

RenderTexture::~RenderTexture()
{
	delete texture;
	renderTarget->Release();
}

void RenderTexture::beginDraw()
{
	Graphics::currentTarget = renderTarget;
	renderTarget->BeginDraw();
}

void RenderTexture::endDraw()
{
	renderTarget->EndDraw();
}

void Graphics::initialize()
{
	// Create factory
	const D2D1_FACTORY_OPTIONS factoryOptions = { .debugLevel = D2D1_DEBUG_LEVEL::D2D1_DEBUG_LEVEL_NONE };
	D2D1CreateFactory(D2D1_FACTORY_TYPE::D2D1_FACTORY_TYPE_SINGLE_THREADED, factoryOptions, &factory);

	// Create render target
	RECT rect;
	GetClientRect(Window::hwnd, &rect);
	const D2D1_RENDER_TARGET_PROPERTIES rtProperties =
	{
		.type = D2D1_RENDER_TARGET_TYPE_HARDWARE,
		.pixelFormat = { .format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, .alphaMode = D2D1_ALPHA_MODE::D2D1_ALPHA_MODE_PREMULTIPLIED },
		.dpiX = 0.0f,
		.dpiY = 0.0f,
		.usage = D2D1_RENDER_TARGET_USAGE::D2D1_RENDER_TARGET_USAGE_NONE,
		.minLevel = D2D1_FEATURE_LEVEL::D2D1_FEATURE_LEVEL_10
	};

	const D2D1_HWND_RENDER_TARGET_PROPERTIES hwndRtProperties =
	{
		.hwnd = Window::hwnd,
		.pixelSize = { .width = (UINT32)rect.right, .height = (UINT32)rect.bottom },
		.presentOptions = D2D1_PRESENT_OPTIONS::D2D1_PRESENT_OPTIONS_NONE
		// Disable VSync
		// .presentOptions = D2D1_PRESENT_OPTIONS::D2D1_PRESENT_OPTIONS_IMMEDIATELY
	};

	factory->CreateHwndRenderTarget(rtProperties, hwndRtProperties, &render);

	// Delta Time
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&lastTime);

	// Set as initial target
	currentTarget = render;

	// Create default brush
	const D2D1_COLOR_F color = { .r = 0.0f, .g = 0.0f, .b = 0.0f, .a = 0.0f };
	render->CreateSolidColorBrush(color, &brush);
}

void Graphics::finalize()
{
	brush->Release();
	render->Release();
	factory->Release();
}

void Graphics::toggleFullscreen()
{
	LONG_PTR style = GetWindowLongPtr(Window::hwnd, GWL_STYLE);

	if ((style & WS_POPUP) == 0)
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

void Graphics::calculateDeltaTime()
{
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);
	deltaTime = ((double)(currentTime.QuadPart - lastTime.QuadPart)) / frequency.QuadPart;
	lastTime = currentTime;
}

void Graphics::resetTarget()
{
	currentTarget = render;
}

void Graphics::clear(const Color &color)
{
	currentTarget->Clear(D2DColor(color));
}

void Graphics::beginDraw()
{
	resetTarget();
	render->BeginDraw();
}
void Graphics::endDraw()
{
	render->EndDraw();
}
