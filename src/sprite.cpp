#include "sprite.hpp"
#include "graphics.hpp"

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
	destination = {0.0f, 0.0f, (float)texture->width, (float)texture->height};
	source = {0.0f, 0.0f, (float)texture->width, (float)texture->height};

	totalFrames = 0;
	currentFrame = 0;
	animationSpeed = 0.0f;
	animationTime = 0.0f;
}

static void updateAnimation(Sprite *sprite)
{
	if (sprite->animationSpeed == 0.0f)
		return;

	sprite->animationTime += (Graphics::deltaTime * 100.0f);
	if (sprite->animationTime >= sprite->animationSpeed)
	{
		sprite->animationTime = 0.0f;
		sprite->currentFrame = (sprite->currentFrame < sprite->totalFrames - 1) ? sprite->currentFrame + 1 : 0;
	}
}

void Sprite::draw()
{
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

	updateAnimation(this);
}

void Sprite::draw(const Rect &destination)
{
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
			.top = source.pos.y,
			.right = source.pos.x + source.size.x + (currentFrame * source.size.x),
			.bottom = source.pos.y + source.size.y
		}
	);

	updateAnimation(this);
}

void Sprite::draw(const Rect &destination, const Rect &source)
{
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
			.top = source.pos.y,
			.right = source.pos.x + source.size.x + (currentFrame * source.size.x),
			.bottom = source.pos.y + source.size.y
		}
	);

	updateAnimation(this);
}
