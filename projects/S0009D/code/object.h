#pragma once
#include "graphics/GraphicsNode.h"
#include "debug/debugManager.h"
#include "mPlane.h"

struct AABB
{
    Vector4D minPoint = Vector4D(99999, 99999, 99999, 1);
    Vector4D maxPoint = Vector4D(-99999, -99999, -99999, 1);;
};

class Object
{
    public:
        Object();
        Object(MeshResource* mr, ShaderObject* so, TextureResource* tr, LightingNode* ln, Vector4D& cameraPos, std::string texturePath);
        ~Object();
        void setupGraphicsNode(MeshResource* mr, ShaderObject* so, TextureResource* tr, LightingNode* ln, Vector4D& cameraPos, std::string texturePath);
        void setViewMatrix(Matrix4D viewmatrix);
        void setupFirstAABB(std::vector<Vertex> vertices);
        void draw();
        mPlane* getPlanesFromAABB();
        AABB getAABB();
    private:
        GraphicsNode graphicsNode;
        Vector4D position = Vector4D(3, 0, 0, 1);
        Matrix4D viewmatrix;
        AABB originalAABB;
        AABB currentAABB;
};