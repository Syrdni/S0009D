#pragma once
#include "core/app.h"
#include "stb_image.h"
#include <iostream>
#include <cassert>

struct Pixel {
	unsigned char red = 0;
	unsigned char green = 0;
	unsigned char blue = 0;

	Pixel(unsigned char r, unsigned char g, unsigned char b) {
		red = r;
		green = g;
		blue = b;
	}
	Pixel() {

	}
};

class TextureResource {
public:
	TextureResource();
	~TextureResource();
	void loadFromFile(const char* filename);
	void loadFromArray(Pixel* array, int width, int height);
	Pixel* getImageData();
	int getHeight();
	int getWidth();
	void generaterTextureHandle();
	void bind(unsigned int unit);
	float* getUVtexture(float u, float v);

private:
	int width;
	int	height;
	int numComponents;
	unsigned int texture;
	unsigned char* imageData;

};