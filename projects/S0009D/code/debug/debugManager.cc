#include "debugManager.h"

DebugManager* DebugManager::getInstance()
{
    if (!instance)
    {
        instance = new DebugManager;
    }
    return instance;
}

DebugManager::DebugManager(){}

DebugManager::~DebugManager(){}

void DebugManager::drawDebugShapes()
{
    if(!render)
        return;

    for (int i = 0; i < debugShapeVector.size(); i++)
    {
        debugShapeVector[i]->draw(viewMatrix);
    }
    for (int i = 0; i < debugSingleFrameVector.size(); i++)
    {
        debugSingleFrameVector[i]->draw(viewMatrix);
    }
    
}

void DebugManager::setViewMatrix(Matrix4D mat)
{
    viewMatrix = mat;
}

void DebugManager::createLine(Vector4D p1, Vector4D p2, Vector4D color)
{
    if (!createShapes)
        return;
    
    addShape(new DebugLine(p1, p2, color));
}


void DebugManager::createCube(Vector4D pos, float width, float height, float length, Vector4D color, bool wireframe)
{
    if(!createShapes)
        return;

    addShape(new DebugCube(pos, width, height, length, color, wireframe));
}

void DebugManager::createSingleFrameCube(Vector4D pos, float width, float height, float length, Vector4D color, bool wireframe)
{
    if(!createShapes)
        return;

    addSingleFrameShape(new DebugCube(pos, width, height, length, color, wireframe));
}

void DebugManager::createSingleFrameLine(Vector4D p1, Vector4D p2, Vector4D color)
{
    if (!createShapes)
        return;
    
    addSingleFrameShape(new DebugLine(p1, p2, color));
}

void DebugManager::addShape(DebugShape* shape)
{
    debugShapeVector.push_back(std::shared_ptr<DebugShape>(shape));
}

void DebugManager::addSingleFrameShape(DebugShape* shape)
{
    debugSingleFrameVector.push_back(std::shared_ptr<DebugShape>(shape));
}

void DebugManager::clear()
{
    debugShapeVector.clear();
}

void DebugManager::clearSingleFrameVector()
{
    debugSingleFrameVector.clear();
}

bool* DebugManager::getRenderBool()
{
    return &render;
}

bool* DebugManager::getCreateShapes()
{
    return &createShapes;
}