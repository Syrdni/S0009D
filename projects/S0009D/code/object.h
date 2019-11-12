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
        void updateAABB();
        void draw();
        void update();
        AABB getAABB();
        bool checkIfRayIntersects(Ray ray);
        Vector4D& getReferenceToPosition();
    private:
        GraphicsNode graphicsNode;
        Vector4D position;
        Matrix4D viewmatrix;
        AABB originalAABB;
        AABB currentAABB;
};