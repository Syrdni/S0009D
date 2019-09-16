#pragma once
#include <vector>
#include "core/app.h"

class MeshResource {
public:
	MeshResource(int *indexBuffer, float *vertexBuffer, int indexlength, int vertexlength);
	MeshResource();
	~MeshResource();
	void bindIndexBuffer();
	void bindVertexBuffer();
	void setupVertexAttributePointers();
	void bindVAO();
	void unbindVAO();
	void setup();
	void destroy();


private:
	float* vertexBuffer;
	int* indexBuffer;
	int indexLength;
	int vertexLength;
	unsigned int EBO;
	unsigned int VAO;
	unsigned int VBO;
	unsigned int tex;
};