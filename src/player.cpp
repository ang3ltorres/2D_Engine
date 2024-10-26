#include "player.hpp"
#include "graphics.hpp"

Player::Player()
: Entity(), rect(0, 0, 32, 32)
{
	
}

Player::~Player()
{

}

void Player::update()
{
	rect.pos.x++;
}

void Player::draw()
{
	Graphics::drawRectangle(rect, {255, 0, 0, 255}, true);
}
