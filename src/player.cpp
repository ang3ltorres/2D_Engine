#include "player.hpp"

#include "graphics.hpp"

Player::Player()
{
	
}

Player::~Player()
{

}

void Player::update()
{

}

void Player::draw()
{
	Graphics::currentTarget->FillRectangle(D2D1_RECT_F {0.0f, 0.0f, 32.0f, 32.0f}, Graphics::brush);
}
