#include "sprite.hpp"
#include "graphics.hpp"

#include <d2d1.h>

Sprite::Sprite()
{
	origin = {0.0f, 0.0f};
	rotation = 0.0f;
	destination = {0.0f, 0.0f, 0.0f, 0.0f};
	source = {0.0f, 0.0f, 0.0f, 0.0f};

	totalFrames = 0;
	currentFrame = 0;
	animationSpeed = 0.0f;
	animationTime = 0.0f;
}

Sprite::Sprite(Texture *texture)
: texture(texture) 
{
	origin = {0.0f, 0.0f};
	rotation = 0.0f;
	destination = {0.0f, 0.0f, texture->width, texture->height};
	source = {0.0f, 0.0f, texture->width, texture->height};

	totalFrames = 0;
	currentFrame = 0;
	animationSpeed = 0.0f;
	animationTime = 0.0f;
}

void Sprite::draw()
{
	D2D1_MATRIX_3X2_F transform = D2D1::Matrix3x2F::Identity();
	transform = D2D1::Matrix3x2F::Rotation(rotation, D2D1_POINT_2F{origin.x + destination.pos.x, origin.y + destination.pos.y}) * transform;
	Graphics::currentTarget->SetTransform(transform);

	Graphics::currentTarget->DrawBitmap
	(
		texture->bitmap,
		{
			.left = destination.pos.x,
			.top = destination.pos.y,
			.right = destination.pos.x + destination.size.x,
			.bottom = destination.pos.y + destination.size.y
		},
		1.0f,
		D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
		{
			.left = source.pos.x + (currentFrame * source.size.x),
			.top = 	source.pos.y,
			.right = source.size.x + (currentFrame * source.size.x),
			.bottom = source.pos.y + source.size.y
		}
	);

	Graphics::currentTarget->SetTransform(D2D1::Matrix3x2F::Identity());

	// Update animation
	if (animationSpeed == 0.0f || totalFrames == 0)
		return;

	animationTime += Graphics::deltaTime;
	if (animationTime >= animationSpeed)
	{
		animationTime = 0.0f;
		currentFrame = (currentFrame < totalFrames - 1) ? currentFrame + 1 : 0;
	}
}
