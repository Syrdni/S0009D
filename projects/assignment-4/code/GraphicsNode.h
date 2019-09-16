#pragma once
#include "core/app.h"
#include <memory>
#include "ShaderObject.h"
#include "TextureResource.h"
#include "MeshResource.h"
#include "Matrix4D.h"

class GraphicsNode {
public:
	GraphicsNode();
	~GraphicsNode();
	MeshResource* getMeshResource();
	void setMeshResource(MeshResource *meshResource);
	TextureResource* getTextureResource();
	void setTextureResource(TextureResource *textureResource);
	ShaderObject* getShaderObject();
	void setShaderObject(ShaderObject *shaderObject);
	Matrix4D getTransform();
	void setTransform(Matrix4D inMatrix);
	void draw(std::string fragmentFile, std::string vertexFile, std::string textureFile);
	void preDrawSetup();
	void loadOBJFile(char * fileName);
	void loadTexture(std::string textureFile);

private:
	Matrix4D transform;
	std::shared_ptr<ShaderObject> shaderObject;
	std::shared_ptr<TextureResource> textureResource;
	std::shared_ptr<MeshResource> meshResource;
};
