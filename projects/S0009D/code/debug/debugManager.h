#pragma once
#include <vector>
#include "debugShape.h"
#include <memory>
#include "debugLine.h"
#include "debugCube.h"

class DebugManager
{
    public:
        DebugManager();
        ~DebugManager();
        void drawDebugShapes();
        void setViewMatrix(Matrix4D view);
        void createLine(Vector4D p1, Vector4D p2);
        void createCube(Vector4D pos, float width, float height, float length);
        void clear();

        bool* getRenderBool();
        bool* getCreateShapes();
    private:
        void addShape(DebugShape* shape);
        std::vector<std::shared_ptr<DebugShape>> debugShapeVector;
        Matrix4D viewMatrix;
        bool render = true;
        bool createShapes = true;
};