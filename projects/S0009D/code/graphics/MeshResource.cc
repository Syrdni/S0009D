#include "MeshResource.h"

//The class constructor. Takes in an index buffer and a vertex buffer and each buffers length.
MeshResource::MeshResource(int *indexBuffer, float *vertexBuffer, int indexlength, int vertexlength)
{
	this->indexBuffer = indexBuffer;
	this->vertexBuffer = vertexBuffer;
	this->indexLength = indexlength;
	this->vertexLength = vertexlength;
}

MeshResource::MeshResource()
{
}

MeshResource::~MeshResource()
{
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &TEX);
	delete[] vertexBuffer;
	delete[] indexBuffer;
}

//Binds the IndexBuffer.
void MeshResource::bindIndexBuffer()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
}

void MeshResource::bufferIndexBuffer()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexes.size() * sizeof(int), &indexes[0], GL_STATIC_DRAW);
}

//Binds the VertexBuffer.
void MeshResource::bindVertexBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
}

void MeshResource::bufferVertexBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, combinedBuffer.size() * sizeof(Vertex), &combinedBuffer[0], GL_STATIC_DRAW);

}

//Setups the vertex attribute pointers.
void MeshResource::setupVertexAttributePointers()
{
	bindVAO();
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float)*3));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 5));
	unbindVAO();
	
}

void MeshResource::bindVAO()
{
	glBindVertexArray(VAO);
}

void MeshResource::unbindVAO()
{
	glBindVertexArray(0);
}

//Setups the different handels and binds the vertex array object
void MeshResource::setup()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &TEX);
	glBindVertexArray(VAO);
}

//Destroys the instance of the class
void MeshResource::destroy()
{
	delete this;
}

int MeshResource::getIndexLength()
{
	return indexLength;
}

int MeshResource::getVertexLength()
{
	return combinedBuffer.size();
}

bool MeshResource::loadFromOBJFile(const char * fileName)
{
	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<Vector4D> temp_vertices;
	std::vector<Vector4D> temp_uvs;
	std::vector<Vector4D> temp_normals;

	FILE * file = fopen(fileName, "r");
	if (file == NULL)
	{
		printf("Error file not found! \n");
		return false;
	}
	while (true)
	{
		char lineHeader[128];
		//read the frist word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
		{
			break; //EOF = End Of File. Quit the loop
		}

		if (strcmp(lineHeader, "v") == 0)
		{
			Vector4D vertex;
			fscanf(file, "%f %f %f\n", &vertex[0], &vertex[1], &vertex[2]);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			Vector4D uv;
			fscanf(file, "%f %f\n", &uv[0], &uv[1]);
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			Vector4D normal;
			fscanf(file, "%f %f %f\n", &normal[0], &normal[1], &normal[2]);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by this simple parser. Try exporting with other options\n");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
	}

	int indexCount = 0;
	std::map<std::string, int> checkUp;

	vertices = temp_vertices;
	textureCordinates = temp_uvs;
	normals = temp_normals;

	Vertex temp;
	float lol[3];

	for (int i = 0; i < vertexIndices.size(); i++)
	{
		std::string check = std::to_string(vertexIndices[i]) + "/" + std::to_string(uvIndices[i]) + "/" + std::to_string(normalIndices[i]);
		if (checkUp.count(check) <= 0)
		{
			indexes.push_back(indexCount);
			checkUp.insert(std::pair<std::string, int>(check, indexCount));

			temp.pos[0] = vertices[vertexIndices[i] - 1].getFloat(0);
			temp.pos[1] = vertices[vertexIndices[i] - 1].getFloat(1);
			temp.pos[2] = vertices[vertexIndices[i] - 1].getFloat(2);

			temp.uv[0] = textureCordinates[uvIndices[i] - 1].getFloat(0);
			temp.uv[1] = textureCordinates[uvIndices[i] - 1].getFloat(1);

			temp.normal[0] = normals[normalIndices[i] - 1].getFloat(0);
			temp.normal[1] = normals[normalIndices[i] - 1].getFloat(1);
			temp.normal[2] = normals[normalIndices[i] - 1].getFloat(2);

			combinedBuffer.push_back(temp);

			indexCount++;
		}
		else
		{
			indexes.push_back(checkUp.at(check));
		}
	}
}

void MeshResource::bindVBO()
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
}

void MeshResource::unbindVBO()
{
	glBindBuffer(0, 0);
}
