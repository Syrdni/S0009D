#pragma once
#include <vector>
#include "debugShape.h"
#include <memory>

class DebugManager
{
    public:
        DebugManager();
        ~DebugManager();
        void drawDebugShapes();
        void setViewMatrix(Matrix4D view);
        void addShape(DebugShape* shape);
        void clear();
    private:
        std::vector<std::shared_ptr<DebugShape>> debugShapeVector;
        Matrix4D viewMatrix;
};