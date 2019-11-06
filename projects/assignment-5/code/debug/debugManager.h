#pragma once
#include <vector>
#include "debugShape.h"

class DebugManager
{
    public:
        DebugManager();
        ~DebugManager();
        void drawDebugShapes();
        void setViewMatrix(Matrix4D view);
        void addShape(DebugShape* shape);
    private:
        std::vector<DebugShape*> debugShapeVector;
        Matrix4D viewMatrix;
};