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
    setupFirstAABB(graphicsNode.getMeshResource()->getVertexBuffer());
}

void Object::setViewMatrix(Matrix4D viewmatrix)
{
    this->viewmatrix = viewmatrix;
}

void Object::setupFirstAABB(std::vector<Vertex> vertices)
{
    for (int i = 0; i < vertices.size(); i++)
    {
        if (vertices[i].pos[0] > originalAABB.maxPoint[0])
            originalAABB.maxPoint[0] = vertices[i].pos[0];

        else if (vertices[i].pos[0] < originalAABB.minPoint[0])
            originalAABB.minPoint[0] = vertices[i].pos[0];
        
        if (vertices[i].pos[1] > originalAABB.maxPoint[1])
            originalAABB.maxPoint[1] = vertices[i].pos[1];

        else if (vertices[i].pos[1] < originalAABB.minPoint[1])
            originalAABB.minPoint[1] = vertices[i].pos[1];

        if (vertices[i].pos[2] > originalAABB.maxPoint[2])
            originalAABB.maxPoint[2] = vertices[i].pos[2];

        else if (vertices[i].pos[2] < originalAABB.minPoint[2])
            originalAABB.minPoint[2] = vertices[i].pos[2];
    }

    originalAABB.minPoint[0] += position[0];
    originalAABB.minPoint[1] += position[1];
    originalAABB.minPoint[2] += position[2];

    originalAABB.maxPoint[0] += position[0];
    originalAABB.maxPoint[1] += position[1];
    originalAABB.maxPoint[2] += position[2];

    Vector4D dimentions = Vector4D(originalAABB.maxPoint[0]-originalAABB.minPoint[0], originalAABB.maxPoint[1]-originalAABB.minPoint[1], originalAABB.maxPoint[2]-originalAABB.minPoint[2], 1);
    Vector4D position = Vector4D(originalAABB.minPoint[0] + (dimentions[0]/2),
                                originalAABB.minPoint[1] + (dimentions[1]/2),
                                originalAABB.minPoint[2] + (dimentions[2]/2),
                                1);
    DebugManager::getInstance()->createCube(position, dimentions[0], dimentions[1], dimentions[2], Vector4D(0, 0, 1, 1), true);
    currentAABB = originalAABB;
    
}

void Object::draw()
{
    Vector4D dimentions = Vector4D(currentAABB.maxPoint[0]-currentAABB.minPoint[0], currentAABB.maxPoint[1]-currentAABB.minPoint[1], currentAABB.maxPoint[2]-currentAABB.minPoint[2], 1);
    Vector4D pos = Vector4D(currentAABB.minPoint[0] + (dimentions[0]/2),
                                currentAABB.minPoint[1] + (dimentions[1]/2),
                                currentAABB.minPoint[2] + (dimentions[2]/2),
                                1);
    DebugManager::getInstance()->createSingleFrameCube(pos, dimentions[0], dimentions[1], dimentions[2], Vector4D(0, 0, 1, 1), true);
    graphicsNode.setTransform(viewmatrix*Matrix4D::getPositionMatrix(position));
    graphicsNode.setPosition(Matrix4D::getPositionMatrix(position));
    graphicsNode.draw();
}

mPlane* Object::getPlanesFromAABB()
{
}

AABB Object::getAABB()
{
    return currentAABB;
}