#include "window.hpp"
#include "graphics.hpp"
#include "texture.hpp"
#include "sprite.hpp"
#include "audio.hpp"
#include "input.hpp"

// Resized callback
void resized(unsigned int width, unsigned int height)
{
	Graphics::render->Resize(D2D1::SizeU(width, height));

	auto renderSize = reinterpret_cast<RenderTexture*>(Graphics::data)->texture->bitmap->GetPixelSize();
	unsigned int scaleX = width / renderSize.width;
	unsigned int scaleY = height / renderSize.height;

	Window::scale = scaleX < scaleY ? scaleX : scaleY;

	Window::offsetX = (width - renderSize.width * Window::scale) / 2;
	Window::offsetY = (height - renderSize.height * Window::scale) / 2;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	unsigned int windowWidth = 256;
	unsigned int windowHeight = 240;

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(pCmdLine);

	Window::initialize(windowWidth, windowHeight, "D2D UwU", hInstance, nCmdShow);
	Graphics::initialize(Window::hwnd);
	Audio::initialize();
	Input::initialize();

	Sound snd{"../res/untitled.ogg"};
	snd.play();

	// DEBUG / TEST
	Rect r{0.0f, 0.0f, 20.0f, 20.0f};
	Texture *txr = new Texture("../res/texture.png");
	Sprite *spr = new Sprite(txr);

	spr->origin = {16.0f/2.0f*4, 27.0f/2.0f*4};
	spr->rotation = 45.0f;
	spr->destination = {64.0f, 64.0f, 16.0f*4, 27.0f*4};
	spr->source = {0.0f, 0.0f, 16.0f, 27.0f};

	spr->totalFrames = 2;
	spr->currentFrame = 0;
	spr->animationSpeed = 2.0f;

	RenderTexture *rt = new RenderTexture(256, 240);
	
	Window::resizedCallback = &resized;
	Graphics::data = rt;
	Window::resizedCallback(windowWidth, windowHeight);

	while (!Window::shouldClose())
	{
		Graphics::calculateDeltaTime();
		Input::update();
		if (Input::keyPressed[Input::Actions::UP]) printf("DOWN");
		r.pos.x++;
		r.size.y++;

		rt->beginDraw();
			Graphics::clear({0, 255, 0});
			r.draw({255, 0, 255});
			spr->draw();
			spr->rotation += 16.0f;
		rt->endDraw();

		Graphics::beginDraw();
			Graphics::clear({0, 100, 255});
			rt->destination = Rect{ (float)Window::offsetX, (float)Window::offsetY, 256.0f * Window::scale, 240.0f * Window::scale };
			rt->draw();
		Graphics::endDraw();
	}

	delete rt;
	delete spr;
	delete txr;
	Input::finalize();
	Audio::finalize();
	Graphics::finalize();
	Window::finalize();

	return 0;
}