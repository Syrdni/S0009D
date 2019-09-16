#pragma once
#include "core/app.h"
#include "stb_image.h"
#include <iostream>
#include <cassert>

class TextureResource {
public:
	TextureResource();
	~TextureResource();
	void loadFromFile(const char* filename);
	void bind(unsigned int unit);

private:
	int width;
	int	height;
	int numComponents;
	unsigned int texture;
	unsigned char* imageData;

};