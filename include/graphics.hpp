#pragma once

#include <d2d1.h>

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

	void draw(const Color &color);
};

struct RenderTexture
{
	RenderTexture() = delete;
	RenderTexture(const RenderTexture &other);
	RenderTexture(unsigned int width, unsigned int height);
	~RenderTexture();

	ID2D1BitmapRenderTarget *renderTarget;
	ID2D1Bitmap *bitmap;

	void beginDraw();
	void endDraw();
	void draw();
	void draw(const Rect &destination);
	void draw(const Rect &destination, const Rect &source);
};

class Graphics
{
private:
	Graphics() = default;
	~Graphics() = default;
	
public:
	static void initialize(HWND &hwnd);
	static void finalize();

	static void resetTarget();
	static void clear(const Color &color);

	static void beginDraw();
	static void endDraw();

	static ID2D1Factory *factory;
	static ID2D1HwndRenderTarget *render;
	static ID2D1SolidColorBrush *brush;

	static ID2D1RenderTarget *currentTarget;
};
