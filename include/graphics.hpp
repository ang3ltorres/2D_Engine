#pragma once

#include "utils.hpp"
#include "sprite.hpp"

class ID2D1BitmapRenderTarget;
class ID2D1Factory;
class ID2D1HwndRenderTarget;
class ID2D1SolidColorBrush;
class ID2D1RenderTarget;

class RenderTexture : public Sprite
{
public:
	RenderTexture() = delete;
	RenderTexture(const RenderTexture &other);
	RenderTexture(unsigned int width, unsigned int height);
	~RenderTexture();

	ID2D1BitmapRenderTarget *renderTarget;

	void beginDraw();
	void endDraw();
};

class Graphics
{
private:
	Graphics() = default;
	~Graphics() = default;
	
public:
	static void initialize();
	static void finalize();

	static void calculateDeltaTime();
	static void resetTarget();
	static void clear(const Color &color);

	static void beginDraw();
	static void endDraw();

	//* SHAPES
	static void drawRectangle(const Rect &rect, const Color &color = {0, 0, 0, 255}, bool fill = true);

	static double deltaTime;

	static void *data;
	static ID2D1Factory *factory;
	static ID2D1HwndRenderTarget *render;
	static ID2D1SolidColorBrush *brush;
	static ID2D1RenderTarget *currentTarget;
};
