#include "SoftwareRenderer.h"

SoftwareRenderer::SoftwareRenderer()
{
}

SoftwareRenderer::~SoftwareRenderer()
{
	delete[] frameBuffer;
	delete[] zBuffer;
}

void SoftwareRenderer::addVertexBuffer(std::vector<Vertex> vertexBuffer)
{
	this->vertexBuffer = vertexBuffer;
}

void SoftwareRenderer::addIndexBuffer(std::vector<int> indexBuffer)
{
	this->indexBuffer = indexBuffer;
}


void SoftwareRenderer::setupFrameBuffer(int width, int height)
{
	frameBufferSize = width * height;
	frameBuffer = new Pixel[frameBufferSize];
	this->width = width;
	this->height = height;
	zBuffer = new float[frameBufferSize];
	std::fill_n(zBuffer, frameBufferSize, -10000);
}

void SoftwareRenderer::clearFrameBuffer()
{
	std::fill_n(zBuffer, frameBufferSize, -10000);
	std::fill_n(frameBuffer, frameBufferSize, Pixel(25, 25, 25));
}

Pixel* SoftwareRenderer::getFrameBufferPointer()
{
	return frameBuffer;
}

int SoftwareRenderer::getFrameBufferSize()
{
	return frameBufferSize;
}

void SoftwareRenderer::setVertexShader(std::function<Vertex(Vertex, Matrix4D, Matrix4D)> lambda)
{
	vertexShader = lambda;
}

void SoftwareRenderer::setPixelShader(std::function<Vector4D(Vertex, Vector4D, TextureResource, int, int)> lambda)
{
	pixelShader = lambda;
}

void SoftwareRenderer::setMVPMatrix(Matrix4D matrix)
{
	mvp = matrix;
}

void SoftwareRenderer::setModelMatrix(Matrix4D matrix)
{
	modelMatrix = matrix;
}

void SoftwareRenderer::setTexture(TextureResource textureResource)
{
	texture = textureResource;
}

void SoftwareRenderer::setCameraPosition(Vector4D camera)
{
	cameraPosition = camera;
}

void SoftwareRenderer::rasterizeTriangle(Vertex v1, Vertex v2, Vertex v3)
{
	//if (v1.pos[0] == v2.pos[0] && v1.pos[1] == v2.pos[1] ||
	//	v1.pos[0] == v3.pos[0] && v1.pos[1] == v3.pos[1] || 
	//	v2.pos[0] == v3.pos[0] && v2.pos[1] == v3.pos[1])
	//{
	//	return;
	//}


	float coordinates[6];
	coordinates[0] = v1.pos[0];
	coordinates[1] = v1.pos[1];
	coordinates[2] = v2.pos[0];
	coordinates[3] = v2.pos[1];
	coordinates[4] = v3.pos[0];
	coordinates[5] = v3.pos[1];

	v1 = vertexShader(v1, mvp, modelMatrix);
	v2 = vertexShader(v2, mvp, modelMatrix);
	v3 = vertexShader(v3, mvp, modelMatrix);

	if (v1.pos[1] < v2.pos[1])
	{
		Vertex temp = v1;
		v1 = v2;
		v2 = temp;
	}

	if (v2.pos[1] < v3.pos[1])
	{
		Vertex temp = v2;
		v2 = v3;
		v3 = temp;
	}

	if (v1.pos[1] < v2.pos[1])
	{
		Vertex temp = v1;
		v1 = v2;
		v2 = temp;
	}

	//Convert to "pixelspace"
	v1.pos[0] = std::roundf(v1.pos[0] * (width / 2) + (width / 2));
	v1.pos[1] = std::roundf(-v1.pos[1] * (height / 2) + (height / 2));
	v2.pos[0] = std::roundf(v2.pos[0] * (width / 2) + (width / 2));
	v2.pos[1] = std::roundf(-v2.pos[1] * (height / 2) + (height / 2));
	v3.pos[0] = std::roundf(v3.pos[0] * (width / 2) + (width / 2));
	v3.pos[1] = std::roundf(-v3.pos[1] * (height / 2) + (height / 2));

	Line line1 = createLine(v1, v3);
	Line line2 = createLine(v1, v2);
	Line line3 = createLine(v2, v3);



	Line linearray[] = { line2, line3 };

	int vY1 = v1.pos[1]+1;

	int u = 0;
	int k = 0;

	bool haveBeenSwapped = false;
	
	for (int i = 0; i < line1.size; i++)
	{
		if (!haveBeenSwapped && linearray[k].size <= u)
		{
			u = 1;
			k = 1;
			//i -= 1;
			haveBeenSwapped = true;
		}
		scanLine(line1.pixels[i], linearray[k].pixels[u], vY1 + i, v1, v2, v3, coordinates);
		//std::cout << &line1 << " " << &linearray[k] << std::endl;
		u++;
	}
	
	delete[] line1.pixels;
	delete[] line2.pixels;
	delete[] line3.pixels;
}

void SoftwareRenderer::rasterize()
{
	for (int i = 0; i < indexBuffer.size(); i += 3)
	{
		rasterizeTriangle(vertexBuffer[indexBuffer[i]], vertexBuffer[indexBuffer[i + 1]], vertexBuffer[indexBuffer[i + 2]]);
	}
}

Line SoftwareRenderer::createLine(Vertex v1, Vertex v2)
{
	Vertex Vertex1 = v1;
	Vertex Vertex2 = v2;

	if (v1.pos[1] > v2.pos[1])
	{
		Vertex1 = v2;
		Vertex2 = v1;
	}

	int VertexX1 = Vertex1.pos[0];
	int VertexY1 = Vertex1.pos[1];
	int VertexX2 = Vertex2.pos[0];
	int VertexY2 = Vertex2.pos[1];

	int deltaX = std::abs(VertexX2 - VertexX1);
	int deltaY = std::abs(VertexY2 - VertexY1);
	int F, H2, W, var, direction, y;

	if (VertexX1 < VertexX2)
	{
		direction = 1;
	}
	else
	{
		direction = -1;
	}

	Line line;
	line.pixels = new int[deltaY +1];
	line.size = deltaY +1;

	if (deltaX > deltaY)
	{
		H2 = 2 * deltaY;
		W = deltaX;
		F = H2 - W;
		var = -2 * (deltaX - deltaY);
		y = 0;

		while (VertexX1 != VertexX2)
		{
			line.pixels[y] = VertexX1;
				if (F < 0)
				{
					F += H2;
				}
				else
				{
					y++;
					F += var;
				}
			VertexX1 += direction;
		}
		line.pixels[y] = VertexX1;
	}

	if (deltaX <= deltaY)
	{
		H2 = 2 * deltaX;
		W = deltaY;
		F = H2 - W;
		var = -2 * (deltaY - deltaX);
		y = 0;

		while (y < deltaY)
		{
			line.pixels[y] = VertexX1;
			if (F < 0)
			{
				F += H2;
			}
			else
			{
				VertexX1 += direction;
				F += var;
			}
			y++;
		}
		line.pixels[y] = VertexX1;
	}
	return line;
}

void SoftwareRenderer::scanLine(int x1, int x2, int y, Vertex & v1, Vertex & v2, Vertex & v3, float* coordinates)
{
	if (x1 > x2)
	{
		int temp = x1;
		x1 = x2;
		x2 = temp;
	}

	if (x1 <= 0)
	{
		x1 = 0;
	}

	if (x2 >= width)
	{
		x2 = width;
	}

	if (y <= 0 || y >= height)
	{
		return;
	}
	Vertex temp;
	int index = x1 + width * y + 1;
	float u, v, w;
 	for (int x = x1; x < x2; x++)
	{
		getBarycentric(Vector4D(x, y, 0, 1), Vector4D(v1.pos[0], v1.pos[1], 1.0, 1), Vector4D(v2.pos[0], v2.pos[1], 1.0, 1), Vector4D(v3.pos[0], v3.pos[1], 1.0, 1), u, v, w);
		float pixelZ = (v1.pos[2] * u) + (v2.pos[2] * v) + (v3.pos[2] * w);

		//Position
		temp.pos[0] = (u * coordinates[0] + v * coordinates[2] + w * coordinates[4]);
		temp.pos[1] = (u * coordinates[1] + v * coordinates[3] + w * coordinates[5]);
		temp.pos[2] = (u * v1.pos[2] + v * v2.pos[2] + w * v3.pos[2]);

		//Texture
		temp.uv[0] = (u * v1.uv[0] + v * v2.uv[0] + w * v3.uv[0]);
		temp.uv[1] = (u * v1.uv[1] + v * v2.uv[1] + w * v3.uv[1]);

		//Normals
		temp.normal[0] = (u * v1.normal[0] + v * v2.normal[0] + w * v3.normal[0]);
		temp.normal[1] = (u * v1.normal[1] + v * v2.normal[1] + w * v3.normal[1]);
		temp.normal[2] = (u * v1.normal[2] + v * v2.normal[2] + w * v3.normal[2]);

		if (zBuffer[index] < pixelZ) //temp.pos[2]
		{
			//setPixel(index, Vector4D(255, 0, 0, 0));
			setPixel(index, pixelShader(temp, cameraPosition, texture, textureHeight, textureWidth));
			zBuffer[index] = pixelZ; //temp.pos[2];
		
		}
		index++;
	}
}

/*Vector4D SoftwareRenderer::getBarycentric(int x, int y, Vertex & v1, Vertex & v2, Vertex & v3)
{
	vertexArea = calculateVertexTriangleArea(v1, v2, v3);
	Vector4D barycentric;
	barycentric[0] = calculateTriangleArea(x, y, v2, v3)/vertexArea;
	barycentric[1] = calculateTriangleArea(x, y, v1, v3)/vertexArea;
	barycentric[2] = calculateTriangleArea(x, y, v2, v1)/vertexArea;
	return barycentric;
}*/
void SoftwareRenderer::getBarycentric(Vector4D point, Vector4D vec1, Vector4D vec2, Vector4D vec3, float& p1, float& p2, float& p3)
{
	Vector4D temp1 = vec2 - vec1, temp2 = vec3 - vec1, temp3 = point - vec1;
	float daa = temp1.dotProduct(temp1);
	float dab = temp1.dotProduct(temp2);
	float dbb = temp2.dotProduct(temp2);
	float dca = temp3.dotProduct(temp1);
	float dcb = temp3.dotProduct(temp2);
	float denominator = (daa * dbb) - (dab * dab);
	p2 = (dbb * dca - dab * dcb) / denominator;
	p3 = (daa * dcb - dab * dca) / denominator;
	if (denominator == 0)
	{
		printf("");
	}
	p1 = 1.0f - p2 - p3;
}

float SoftwareRenderer::calculateTriangleArea(int x, int y, Vertex & v2, Vertex & v3)
{
	return std::abs(((x * (v2.pos[1] - v3.pos[1]) + v2.pos[0] * (v3.pos[1] - y) + v3.pos[0] * (y - v2.pos[1])) / 2));
}

float SoftwareRenderer::calculateVertexTriangleArea(Vertex & v1, Vertex & v2, Vertex & v3)
{
	return std::abs(((v1.pos[0] * (v2.pos[1] - v3.pos[1]) + v2.pos[0] * (v3.pos[1] - v1.pos[1]) + v3.pos[0] * (v1.pos[1] - v2.pos[1])) / 2));
}

void SoftwareRenderer::setPixel(int position, Vector4D color)
{
	frameBuffer[position].red = color[0];
	frameBuffer[position].green = color[1];
	frameBuffer[position].blue = color[2];
}
