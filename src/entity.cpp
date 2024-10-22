#include "entity.hpp"
#include "game.hpp"

Entity::Entity()
{
	Game::entity.push_back(this);
}