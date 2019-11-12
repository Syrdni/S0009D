#pragma once
#include "core/app.h"
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include "../Vector4D.h"
#include <map>
#include <string.h>

struct Vertex {
	float pos[3];
	float uv[2];
	float normal[3];
};

class MeshResource {
public:
	MeshResource(int *indexBuffer, float *vertexBuffer, int indexlength, int vertexlength);
	MeshResource();
	~MeshResource();
	void bindIndexBuffer();
	void bufferIndexBuffer();
	void bindVertexBuffer();
	void bufferVertexBuffer();
	void setupVertexAttributePointers();
	void bindVAO();
	void unbindVAO();
	void setup();
	void destroy();
	int getIndexLength();
	int getVertexLength();
	bool loadFromOBJFile(const char * fileName);
	void bindVBO();
	void unbindVBO();
	std::vector<Vertex> getVertexBuffer();
	std::vector<int> getIndexBuffer();


private:
	float* vertexBuffer;
	int* indexBuffer;
	float* textureCordinateBuffer;
	float* normalBuffer;
	int indexLength;
	int vertexLength;
	int vertexIndicesSize;
	int normalIndicesSize;
	unsigned int EBO;
	unsigned int VAO;
	unsigned int VBO;
	unsigned int TEX;

	std::vector<Vector4D> vertices;
	std::vector<Vector4D> textureCordinates;
	std::vector<Vector4D> normals;
	std::vector<Vertex> combinedBuffer;
	std::vector<int> indexes;
};