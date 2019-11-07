#include "debugManager.h"

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
}

void DebugManager::setViewMatrix(Matrix4D mat)
{
    viewMatrix = mat;
}

void DebugManager::createLine(Vector4D p1, Vector4D p2)
{
    if (!createShapes)
        return;
    
    addShape(new DebugLine(p1, p2));
}

void DebugManager::addShape(DebugShape* shape)
{
    debugShapeVector.push_back(std::shared_ptr<DebugShape>(shape));
}

void DebugManager::clear()
{
    debugShapeVector.clear();
}

bool* DebugManager::getRenderBool()
{
    return &render;
}

bool* DebugManager::getCreateShapes()
{
    return &createShapes;
}