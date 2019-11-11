#pragma once
#include "graphics/GraphicsNode.h"

struct AABB
{
    Vector4D minPoint;
    Vector4D maxPoint;
};

class Object
{
    public:
        Object();
        Object(MeshResource* mr, ShaderObject* so, TextureResource* tr, LightingNode* ln, Vector4D& cameraPos, std::string texturePath);
        ~Object();
        void setupGraphicsNode(MeshResource* mr, ShaderObject* so, TextureResource* tr, LightingNode* ln, Vector4D& cameraPos, std::string texturePath);
        void setViewMatrix(Matrix4D viewmatrix);
        void draw();
    private:
        GraphicsNode graphicsNode;
        Vector4D position;
        Matrix4D viewmatrix;
};