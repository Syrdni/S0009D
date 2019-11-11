#pragma once
#include "graphics/GraphicsNode.h"
#include "debug/debugManager.h"
#include "mPlane.h"
#include "Ray.h"

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
        AABB getAABB();
        bool checkIfRayIntersects(Ray ray);
    private:
        GraphicsNode graphicsNode;
        Vector4D position = Vector4D(0, 0, 0, 1);
        Matrix4D viewmatrix;
        AABB originalAABB;
        AABB currentAABB;
};