#include "graphics.hpp"

#include <Windows.h>

// Static members
ID2D1Factory *Graphics::factory = nullptr;
ID2D1HwndRenderTarget *Graphics::render = nullptr;
ID2D1SolidColorBrush *Graphics::brush = nullptr;
ID2D1RenderTarget *Graphics::currentTarget = nullptr;

// Static helpers
static const auto D2DColor = [](const Color &color) -> D2D_COLOR_F
{
	return {color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f};
};

// Struct Color
Color::Color()
: r(0), g(0), b(0), a(0) {}

Color::Color(const Color &other)
: r(other.r), g(other.g), b(other.b), a(other.a) {}

Color::Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
: r(r), g(g), b(b), a(a) {}

// Struct Vector2
Vector2::Vector2()
: x(0.0f), y(0.0f) {}

Vector2::Vector2(const Vector2 &other)
: x(other.x), y(other.y) {}

Vector2::Vector2(float x, float y)
: x(x), y(y) {}

// Struct Rect
Rect::Rect()
: pos(0.0f, 0.0f), size(0.0f, 0.0f) {}

Rect::Rect(const Rect &other)
: pos(other.pos), size(other.size) {}

Rect::Rect(float x, float y, float w, float h)
: pos(x, y), size(w, h) {}

Rect::Rect(const Vector2 &pos, const Vector2 &size)
: pos(pos), size(size) {}

void Rect::draw(const Color &color)
{
	Graphics::brush->SetColor(D2DColor(color));
	Graphics::currentTarget->FillRectangle({pos.x, pos.y, pos.x + size.x, pos.y + size.y}, Graphics::brush);
}

RenderTexture::RenderTexture(const RenderTexture &other)
: RenderTexture::RenderTexture(other.bitmap->GetPixelSize().width, other.bitmap->GetPixelSize().height)
{	
	const D2D1_POINT_2U destination = {0, 0};
	const D2D1_RECT_U source = {0, 0, other.bitmap->GetPixelSize().width, other.bitmap->GetPixelSize().height};
	bitmap->CopyFromBitmap(&destination, other.bitmap, &source);
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

	renderTarget->GetBitmap(&bitmap);
}

RenderTexture::~RenderTexture()
{
	bitmap->Release();
	renderTarget->Release();
}

void RenderTexture::beginDraw()
{
	Graphics::currentTarget = renderTarget;
	renderTarget->BeginDraw();
}

void RenderTexture::endDraw()
{
	Graphics::resetTarget();
	renderTarget->EndDraw();
}

void RenderTexture::draw()
{
	Graphics::currentTarget->DrawBitmap
	(
		bitmap,
		{
			0.0f,
			0.0f,
			float(bitmap->GetPixelSize().width),
			float(bitmap->GetPixelSize().height)
		},
		1.0f,
		D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
		{
			0.0f,
			0.0f,
			float(bitmap->GetPixelSize().width),
			float(bitmap->GetPixelSize().height)
		}
	);
}

void RenderTexture::draw(const Rect &destination)
{
	Graphics::currentTarget->DrawBitmap
	(
		bitmap,
		{
			destination.pos.x,
			destination.pos.y,
			destination.pos.x + destination.size.x,
			destination.pos.y + destination.size.y
		},
		1.0f,
		D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
		{
			0.0f,
			0.0f,
			float(bitmap->GetPixelSize().width),
			float(bitmap->GetPixelSize().height)
		}
	);
}

void RenderTexture::draw(const Rect &destination, const Rect &source)
{
	Graphics::currentTarget->DrawBitmap
	(
		bitmap,
		{
			destination.pos.x,
			destination.pos.y,
			destination.pos.x + destination.size.x,
			destination.pos.y + destination.size.y
		},
		1.0f,
		D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
		{
			source.pos.x,
			source.pos.y,
			source.size.x,
			source.size.y
		}
	);
}

void Graphics::initialize(HWND &hwnd)
{
	// Create factory
	const D2D1_FACTORY_OPTIONS factoryOptions = { .debugLevel = D2D1_DEBUG_LEVEL::D2D1_DEBUG_LEVEL_NONE };
	D2D1CreateFactory(D2D1_FACTORY_TYPE::D2D1_FACTORY_TYPE_SINGLE_THREADED, factoryOptions, &factory);

	// Create render target
	RECT rect;
	GetClientRect(hwnd, &rect);
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
		.hwnd = hwnd,
		.pixelSize = { .width = (UINT32)rect.right, .height = (UINT32)rect.bottom },
		.presentOptions = D2D1_PRESENT_OPTIONS::D2D1_PRESENT_OPTIONS_NONE
	};

	factory->CreateHwndRenderTarget(rtProperties, hwndRtProperties, &render);
	
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
