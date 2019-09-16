#include "TextureResource.h"

TextureResource::TextureResource()
{
}

//Destructor for the class.
TextureResource::~TextureResource()
{
	glDeleteTextures(1, &texture);
	//delete[] imageData;
}

//Loads a file to use as a texture.
void TextureResource::loadFromFile(const char * fileName)
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(1);
	imageData = stbi_load(fileName, &width, &height, &numComponents, 3);
	if (imageData)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Texture failed to load " << fileName << std::endl;
	}
	//stbi_image_free(imageData);
}

void TextureResource::loadFromArray(Pixel* array, int width, int height)
{
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, array);
	glGenerateMipmap(GL_TEXTURE_2D);

	//stbi_image_free(array);
}

Pixel * TextureResource::getImageData()
{
	return (Pixel*)&imageData;
}

int TextureResource::getHeight()
{
	return height;
}

int TextureResource::getWidth()
{
	return width;
}

void TextureResource::generaterTextureHandle()
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

//Binds the texture
void TextureResource::bind(unsigned int unit)
{
	assert(unit >= 0 && unit <= 31);

	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, texture);
}

float * TextureResource::getUVtexture(float u, float v)
{
	if (u > 1)
	{
		u = 1;
	}
	else if (u < 0)
	{
		u = 0;
	}
	if (v > 1)
	{
		v = 1;
	}
	else if (v < 0)
	{
		v = 0;
	}

	int x = width * u;
	int y = (height-1) * v;
	int uv = (x * numComponents) + (y * width * numComponents);

	float *c = new float[3];
	c[0] = imageData[uv++];
	c[1] = imageData[uv++];
	c[2] = imageData[uv++];

	return c;
}
