#pragma once

#include <string>

struct ID2D1Bitmap;

class Texture
{
public:
	Texture() = default;
	Texture(const std::string &fileName);
	Texture(const Texture &other);
	~Texture();
	
	ID2D1Bitmap *bitmap;
	unsigned int width;
	unsigned int height;
};

