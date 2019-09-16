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

void GraphicsNode::setTransform(Matrix4D transform)
{
	this->transform = transform;
}

LightingNode* GraphicsNode::getLightingNode()
{
	return lightingNode.get();
}

void GraphicsNode::setCameraPosition(Vector4D position)
{
	cameraPosition = position;
}

Vector4D GraphicsNode::getCameraPosition()
{
	return cameraPosition;
}

void GraphicsNode::setlightingNode(LightingNode *node)
{
	this->lightingNode.reset(node);
}

void GraphicsNode::setPosition(Matrix4D position)
{
	this->position = position;
}

void GraphicsNode::draw()
{
	textureResource.get()->bind(0);
	meshResource->setupVertexAttributePointers();
	shaderObject->useProgram();
	shaderObject->modifyMatrix4fvUniform("transform", transform);
	shaderObject->modifyMatrix4fvUniform("modelMatrix", position);
	shaderObject->modifyVector4fvUniform("lightPosition", lightingNode->getPosition());
	shaderObject->modifyVector4fvUniform("lightColor", lightingNode->getColorWithIntensity());
	shaderObject->modifyVector4fvUniform("cameraPosition", cameraPosition);
	meshResource->bindVAO();
	glDrawElements(GL_TRIANGLES, meshResource->getVertexLength() * sizeof(Vertex), GL_UNSIGNED_INT, NULL);
	//glDrawArrays(GL_TRIANGLES, 0, meshResource->getVertexLength());
	meshResource->unbindVAO();
}

void GraphicsNode::loadTexture(std::string textureFile)
{
	textureResource.get()->loadFromFile(textureFile.c_str());
}

void GraphicsNode::preDrawSetup()
{
	meshResource.get()->setup();
	meshResource.get()->bufferVertexBuffer();
	meshResource.get()->bufferIndexBuffer();
	shaderObject.get()->loadFragmentShader("fragmentShader2.txt");
	shaderObject.get()->loadVertexShader("vertexShader.txt");
	shaderObject->compileShader();
}
