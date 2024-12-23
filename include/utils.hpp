#pragma once

#include <string>

std::wstring toWideString(const std::string &string);

struct Color
{
	Color();
	Color(const Color &other);
	Color(unsigned char r = 0, unsigned char g = 0, unsigned char b = 0, unsigned char a = 255);
	~Color() = default;

	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
};

struct Vector2
{
	Vector2();
	Vector2(const Vector2 &other);
	Vector2(float x, float y);
	~Vector2() = default;

	float x;
	float y;
};

struct Rect
{
	Rect();
	Rect(const Rect &other);
	Rect(float x, float y, float w, float h);
	Rect(const Vector2 &pos, const Vector2 &size);
	~Rect() = default;

	Vector2 pos;
	Vector2 size;

	bool collision(const Rect &other);
};
