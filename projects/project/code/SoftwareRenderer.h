#pragma once
#include <functional>
#include "Matrix4D.h"
#include "MeshResource.h"
#include "TextureResource.h"
#include <math.h>
#include <cmath>
#include <algorithm>

struct Line {
	int* pixels;
	int size;
};

class SoftwareRenderer {
public:
	SoftwareRenderer();
	~SoftwareRenderer();
	void addVertexBuffer(std::vector<Vertex> vertexBuffer);
	void addIndexBuffer(std::vector<int> indexBuffer);
	void setupFrameBuffer(int width, int height);
	void clearFrameBuffer();
	Pixel* getFrameBufferPointer();
	int getFrameBufferSize();
	void setVertexShader(std::function<Vertex(Vertex, Matrix4D, Matrix4D)> lambda);
	void setPixelShader(std::function<Vector4D(Vertex, Vector4D, TextureResource, int, int)> lambda);
	void setMVPMatrix(Matrix4D matrix);
	void setModelMatrix(Matrix4D matrix);
	void setTexture(TextureResource);
	void setCameraPosition(Vector4D camera);
	void rasterizeTriangle(Vertex v1, Vertex v2, Vertex v3);
	void rasterize();
	Line createLine(Vertex v1, Vertex v2);
	void scanLine(int x1, int x2, int y, Vertex &v1, Vertex &v2, Vertex &v3, float* coordinates);
	void getBarycentric(Vector4D point, Vector4D vec1, Vector4D vec2, Vector4D vec3, float& p1, float& p2, float& p3);
	float calculateTriangleArea(int x, int y, Vertex &v1, Vertex &v2);
	float calculateVertexTriangleArea(Vertex &v1, Vertex &v2, Vertex &v3);
	void setPixel(int position, Vector4D color);

private:
	Matrix4D mvp;
	Matrix4D modelMatrix;

	Pixel* frameBuffer;
	int frameBufferSize;
	int width;
	int height;
	int textureHeight;
	int textureWidth;
	TextureResource texture;
	Vector4D cameraPosition;
	std::vector<Vertex> vertexBuffer;
	std::vector<int> indexBuffer;
	float* zBuffer;
	std::function<Vertex(Vertex, Matrix4D, Matrix4D)> vertexShader;
	std::function<Vector4D(Vertex, Vector4D, TextureResource, int, int)> pixelShader;
	bool ttt = false;
	float vertexArea;
};