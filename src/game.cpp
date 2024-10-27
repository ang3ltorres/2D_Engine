#include "game.hpp"
//*--------------------------
#include "window.hpp"
#include "graphics.hpp"
#include "audio.hpp"
#include "input.hpp"
#include "player.hpp"
//*--------------------------
#include <d2d1.h>
//*--------------------------

static Sound *sound;

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

Game::Game(HINSTANCE &hInstance, int nCmdShow)
{
	windowWidth = 800;
	windowHeight = 600;

	Window::initialize(windowWidth, windowHeight, "D2D UwU", hInstance, nCmdShow);
	Graphics::initialize();
	Audio::initialize();
	Input::initialize();

	rt = new RenderTexture(256, 256);
	player = new Player();
	Window::resizedCallback = &resized;
	Graphics::data = rt;
	Window::resizedCallback(windowWidth, windowHeight);

	sound = new Sound("../sound.ogg");
}

Game::~Game()
{
	delete sound;
	delete rt;
	Input::finalize();
	Audio::finalize();
	Graphics::finalize();
	Window::finalize();
}

void Game::loop()
{
	while (!Window::shouldClose())
	{
		Graphics::calculateDeltaTime();
		Input::update();
		Game::update();

		rt->beginDraw();
			Graphics::clear({0, 255, 0});
			draw();
		rt->endDraw();

		Graphics::beginDraw();
			Graphics::clear({0, 100, 255});
			rt->destination = Rect{
				Window::offsetX,
				Window::offsetY,
				rt->texture->width * Window::scale,
				rt->texture->height * Window::scale
			};
			rt->draw();
		Graphics::endDraw();
	}
}

void Game::update()
{
	if (Input::keyPressed[Input::F11]) Window::toggleFullscreen();
	if (Input::key[Input::ESC]) Window::forceClose = true;
	if (Input::keyPressed[Input::DOWN]) sound->play();

	player->update();
}

void Game::draw()
{
	player->draw();
}