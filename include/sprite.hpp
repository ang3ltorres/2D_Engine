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
	void draw(const Rect &destination);
	void draw(const Rect &destination, const Rect &source);

	Texture *texture;

	Vector2 origin;
	float rotation;
	Rect destination;
	Rect source;

	// Animation
	unsigned int totalFrames;
	unsigned int currentFrame;
	float animationSpeed;
	float animationTime;
};
