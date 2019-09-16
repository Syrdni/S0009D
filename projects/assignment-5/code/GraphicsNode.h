#pragma once
#include "core/app.h"
#include <memory>
#include "ShaderObject.h"
#include "TextureResource.h"
#include "MeshResource.h"
#include "LightingNode.h"

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
		void setTransform(Matrix4D transform);
		LightingNode* getLightingNode();
		void setCameraPosition(Vector4D position);
		Vector4D getCameraPosition();
		void setlightingNode(LightingNode *node);
		void setPosition(Matrix4D position);
		void draw();
		void loadTexture(std::string textureFile);
		void preDrawSetup();

	private:
		std::shared_ptr<ShaderObject> shaderObject;
		std::shared_ptr<TextureResource> textureResource;
		std::shared_ptr<MeshResource> meshResource;
		std::shared_ptr<LightingNode> lightingNode;
		Vector4D cameraPosition;
		Matrix4D transform;
		Matrix4D position;
};