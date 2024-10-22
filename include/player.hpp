#pragma once

#include "entity.hpp"

class Player : public Entity
{
public:

	Player();
	~Player();

	void update() override;
	void draw() override;
};
