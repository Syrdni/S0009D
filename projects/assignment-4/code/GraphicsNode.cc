#include "GraphicsNode.h"

GraphicsNode::GraphicsNode()
{
	transform = Matrix4D();
}

GraphicsNode::~GraphicsNode()
{
}

MeshResource* GraphicsNode::getMeshResource()
{
	return meshResource.get();
}

void GraphicsNode::setMeshResource(MeshResource *meshResource)
{
	this->meshResource.reset(meshResource);
}

TextureResource* GraphicsNode::getTextureResource()
{
	return textureResource.get();
}

void GraphicsNode::setTextureResource(TextureResource *textureResource)
{
	this->textureResource.reset(textureResource);
}

ShaderObject* GraphicsNode::getShaderObject()
{
	return shaderObject.get();
}

void GraphicsNode::setShaderObject(ShaderObject *shaderObject)
{
	this->shaderObject.reset(shaderObject);
}

Matrix4D GraphicsNode::getTransform()
{
	return transform;
}

void GraphicsNode::setTransform(Matrix4D inMatrix)
{
	transform = inMatrix;
}

void GraphicsNode::draw(std::string fragmentFile, std::string vertexFile, std::string textureFile)
{
	//shaderObject.get()->loadFragmentShader(fragmentFile.c_str());
	//shaderObject.get()->loadVertexShader(vertexFile.c_str());
	//meshResource.get()->setup();
	textureResource.get()->bind(0);
	//meshResource.get()->bindVertexBuffer();
	//meshResource.get()->bindIndexBuffer();
	meshResource.get()->setupVertexAttributePointers();
	shaderObject.get()->useProgram();
	shaderObject.get()->modifyMatrix4fvUniform("transform", transform.getMatrix());

	meshResource.get()->bindVAO();
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);
	meshResource.get()->unbindVAO();
}

void GraphicsNode::preDrawSetup()
{
	meshResource.get()->setup();
	shaderObject.get()->loadFragmentShader("fragmentShader.txt");
	shaderObject.get()->loadVertexShader("vertexShader.txt");
	meshResource.get()->bufferIndexBuffer();
	meshResource.get()->bufferVertexBuffer();
	shaderObject->compileShader();
}

void GraphicsNode::loadTexture(std::string textureFile)
{
	textureResource.get()->loadFromFile(textureFile.c_str());
}
