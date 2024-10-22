#pragma once
#include <minwindef.h>
#include <vector>

class RenderTexture;
class Player;
class Entity;

class Game
{
public:
	unsigned int windowWidth;
	unsigned int windowHeight;
	RenderTexture *rt;
	Player *player;

	static std::vector<Entity*> entity;

	Game(HINSTANCE &hInstance, int nCmdShow);
	Game(const Game&) = delete;
	~Game();

	void loop();
	void update();
	void draw();
};
