#include "graphics.hpp"
#include "window.hpp"

#include <Windows.h>
#include <d2d1.h>

//& Local
LARGE_INTEGER frequency;
LARGE_INTEGER lastTime;

//& Static members
double Graphics::deltaTime = 0.0f;
void *Graphics::data = nullptr;
ID2D1Factory *Graphics::factory = nullptr;
ID2D1HwndRenderTarget *Graphics::render = nullptr;
ID2D1SolidColorBrush *Graphics::brush = nullptr;
ID2D1RenderTarget *Graphics::currentTarget = nullptr;

//& Static
static inline D2D_COLOR_F D2DColor(const Color &color)
{
	return
	{
		color.r / 255.0f,
		color.g / 255.0f,
		color.b / 255.0f,
		color.a / 255.0f
	};
}

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
	renderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE::D2D1_ANTIALIAS_MODE_ALIASED);
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
	render->SetAntialiasMode(D2D1_ANTIALIAS_MODE::D2D1_ANTIALIAS_MODE_ALIASED);

	// Delta Time
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&lastTime);

	// Set as initial target
	currentTarget = render;

	// Create default brush
	const D2D1_COLOR_F color = { .r = 0.0f, .g = 0.0f, .b = 0.0f, .a = 1.0f };
	render->CreateSolidColorBrush(color, &brush);
}

void Graphics::finalize()
{
	brush->Release();
	render->Release();
	factory->Release();
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

void Graphics::drawRectangle(const Rect &rect, const Color &color, bool fill)
{
	Graphics::brush->SetColor(D2DColor(color));
	
	if (fill)
		Graphics::currentTarget->FillRectangle({rect.pos.x, rect.pos.y, rect.pos.x + rect.size.x, rect.pos.y + rect.size.y}, Graphics::brush);
	else
		Graphics::currentTarget->DrawRectangle({rect.pos.x, rect.pos.y, rect.pos.x + rect.size.x, rect.pos.y + rect.size.y}, Graphics::brush);
}
