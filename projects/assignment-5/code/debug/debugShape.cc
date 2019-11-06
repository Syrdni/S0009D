#include "debugShape.h"

DebugShape::DebugShape()
{
    debugMessageSent = false;
}

void DebugShape::draw(Matrix4D viewMatrix)
{
    if (!debugMessageSent)
    {
        std::cout << "Draw function missing" << std::endl;
        debugMessageSent = true;
    }
}

void DebugShape::setup(){std::cout << "Setup function missing" << std::endl;}