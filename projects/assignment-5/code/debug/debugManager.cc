#include "debugManager.h"

DebugManager::DebugManager(){}

DebugManager::~DebugManager(){}

void DebugManager::drawDebugShapes()
{
    for (int i = 0; i < debugShapeVector.size(); i++)
    {
        debugShapeVector[i]->draw(viewMatrix);
    }
}

void DebugManager::setViewMatrix(Matrix4D mat)
{
    viewMatrix = mat;
}

void DebugManager::addShape(DebugShape* shape)
{
    debugShapeVector.push_back(shape);
}