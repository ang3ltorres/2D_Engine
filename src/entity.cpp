#include "entity.hpp"
#include "game.hpp"
#include <algorithm>

Entity::Entity()
{
	Game::entity.push_back(this);
}

static void selfRemove(Entity *entity)
{
	auto it = std::remove(Game::entity.begin(), Game::entity.end(), entity);
	if (it != Game::entity.end())
		Game::entity.erase(it, Game::entity.end());

	delete entity;
}

Entity::~Entity()
{
	selfRemove(this);
}
