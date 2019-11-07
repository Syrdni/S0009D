#pragma once
#include <vector>
#include "debugShape.h"
#include <memory>
#include "debugLine.h"

class DebugManager
{
    public:
        DebugManager();
        ~DebugManager();
        void drawDebugShapes();
        void setViewMatrix(Matrix4D view);
        void createLine(Vector4D p1, Vector4D p2);
        void addShape(DebugShape* shape);
        void clear();

        bool* getRenderBool();
        bool* getCreateShapes();
    private:
        std::vector<std::shared_ptr<DebugShape>> debugShapeVector;
        Matrix4D viewMatrix;
        bool render;
        bool createShapes;
};