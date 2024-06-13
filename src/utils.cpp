#include "utils.hpp"
#include "graphics.hpp"

#include <codecvt>
#include <locale>

std::wstring toWideString(const std::string &string)
{
	static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter{};

	return converter.from_bytes(string);
}

// Struct Color
Color::Color()
: r(0), g(0), b(0), a(0) {}

Color::Color(const Color &other)
: r(other.r), g(other.g), b(other.b), a(other.a) {}

Color::Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
: r(r), g(g), b(b), a(a) {}

// Color to D2D_COLOR_F converter
D2D_COLOR_F D2DColor(const Color &color)
{
	return
	{
		color.r / 255.0f,
		color.g / 255.0f,
		color.b / 255.0f,
		color.a / 255.0f
	};
}

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

