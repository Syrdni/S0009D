#include "object.h"

Object::Object(){}

Object::Object(MeshResource* mr, ShaderObject* so, TextureResource* tr, LightingNode* ln, Vector4D& cameraPos, std::string texturePath)
{
    setupGraphicsNode(mr, so, tr, ln, cameraPos, texturePath);
}

Object::~Object(){}

void Object::setupGraphicsNode(MeshResource* mr, ShaderObject* so, TextureResource* tr, LightingNode* ln, Vector4D& cameraPos, std::string texturePath)
{
    graphicsNode.setMeshResource(mr);
    graphicsNode.setShaderObject(so);
    graphicsNode.setTextureResource(tr);
    graphicsNode.setlightingNode(ln);
    graphicsNode.setCameraPosition(cameraPos);
    graphicsNode.loadTexture(texturePath.c_str());
    graphicsNode.preDrawSetup();
}

void Object::setViewMatrix(Matrix4D viewmatrix)
{
    this->viewmatrix = viewmatrix;
}

void Object::draw()
{
    graphicsNode.setTransform(viewmatrix*Matrix4D::getPositionMatrix(position));
    graphicsNode.setPosition(Matrix4D::getPositionMatrix(position));
    graphicsNode.draw();
}