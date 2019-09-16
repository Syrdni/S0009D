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
}

//Binds the IndexBuffer.
void MeshResource::bindIndexBuffer()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexLength * sizeof(int), this->indexBuffer, GL_STATIC_DRAW);
}

//Binds the VertexBuffer.
void MeshResource::bindVertexBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexLength * sizeof(float), this->vertexBuffer, GL_STATIC_DRAW);
}

//Setups the vertex attribute pointers.
void MeshResource::setupVertexAttributePointers()
{
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, NULL);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(6 * sizeof(float)));
	glBindVertexArray(0);
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
	glGenBuffers(1, &tex);
	glBindVertexArray(VAO);
}

//Destroys the instance of the class
void MeshResource::destroy()
{
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &tex);
	delete[] vertexBuffer;
	delete[] indexBuffer;
	delete this;
}
