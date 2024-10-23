#pragma once

#include "utils.hpp"
#include "sprite.hpp"

#include <d2d1.h>

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

	static void toggleFullscreen();
	static void calculateDeltaTime();
	static void resetTarget();
	static void clear(const Color &color);

	static void beginDraw();
	static void endDraw();

	static double deltaTime;

	static void *data;
	static ID2D1Factory *factory;
	static ID2D1HwndRenderTarget *render;
	static ID2D1SolidColorBrush *brush;
	static ID2D1RenderTarget *currentTarget;
};
