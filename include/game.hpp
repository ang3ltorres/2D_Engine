#pragma once
#include <minwindef.h>

class RenderTexture;

class Game
{
public:
	unsigned int windowWidth;
	unsigned int windowHeight;
	RenderTexture *rt;

	Game(HINSTANCE &hInstance, int nCmdShow);
	Game(const Game&) = delete;
	~Game();

	void loop();
	void update();
	void draw();
};
