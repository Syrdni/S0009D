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
        Vector4D& getReferenceToRotation();
    private:
        float findTheSmallesCoordinate(float p1, float p2);
        float findTheBiggestCoordinate(float p1, float p2);
        GraphicsNode graphicsNode;

        Vector4D rotation;
        Vector4D position;

        Matrix4D viewmatrix;
        Matrix4D totalRotation;

        AABB originalAABB;
        AABB currentAABB;
};