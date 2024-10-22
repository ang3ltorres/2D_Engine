#pragma once

#include "entity.hpp"
#include "utils.hpp"

class Player : public Entity
{
public:

	Player();
	~Player();

	Rect rect;

	void update() override;
	void draw() override;
};
