#pragma once
#include "graphics/GraphicsNode.h"
#include "debug/debugManager.h"
#include "mPlane.h"
#include "Ray.h"
#include "rigidbody.h"

class Object
{
    public:
        Object();
        Object(MeshResource* mr, ShaderObject* so, TextureResource* tr, LightingNode* ln, Vector4D& cameraPos, std::string texturePath, Vector4D scale, float mass, bool unmovable);
        ~Object();
        void setupGraphicsNode(MeshResource* mr, ShaderObject* so, TextureResource* tr, LightingNode* ln, Vector4D& cameraPos, std::string texturePath);
        void setViewMatrix(Matrix4D viewmatrix);
        void setupFirstAABB(std::vector<Vertex> vertices);
        void updateAABB();
        void draw();
        void update();
        AABB getAABB();
        PointAndDistance checkIfRayIntersects(Ray ray);
        Vector4D& getReferenceToPosition();
        Vector4D& getReferenceToRotation();
        Vector4D& getReferenceToScale();
        Rigidbody& getReferenceToRigidbody();
        AABB& getReferenceToAABB();
        Matrix4D getRotation();
        Vector4D colorOnAABB = Vector4D(0, 0, 1, 1);
        Vector4D indexOfFurthestPoint(Vector4D direction);
        GraphicsNode getGraphicsNode();
        void setScaleMatrix(Matrix4D scale);
        Matrix4D scaleMatrix;

    private:
        float findTheSmallesCoordinate(float p1, float p2);
        float findTheBiggestCoordinate(float p1, float p2);
        GraphicsNode graphicsNode;

        Vector4D rotation;
        Vector4D scale = Vector4D(1, 1, 1, 1);
        Vector4D position;

        Matrix4D viewmatrix;
        Matrix4D totalRotation;

        AABB originalAABB;
        AABB currentAABB;

        Rigidbody rb;

};