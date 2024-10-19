#pragma once

#include "texture.hpp"
#include "utils.hpp"

class Sprite
{
public:
	Sprite();
	Sprite(Texture *texture);
	~Sprite() = default;

	void draw();

	Texture *texture;

	Vector2 origin;
	float rotation;
	Rect destination;
	Rect source;

	// Animation
	unsigned char totalFrames;
	unsigned char currentFrame;
	float animationSpeed;
	float animationTime;
};
