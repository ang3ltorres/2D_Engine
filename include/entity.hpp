#pragma once

class Entity
{
public:
	Entity();
	Entity(const Entity&) = delete;
	virtual ~Entity() = default;

	virtual void update() = 0;
	virtual void draw() = 0;
};
