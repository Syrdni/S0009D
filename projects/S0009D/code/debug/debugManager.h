#pragma once
#include <vector>
#include "debugShape.h"
#include <memory>
#include "debugLine.h"
#include "debugCube.h"

class DebugManager
{
    public:
        static DebugManager* getInstance();
        ~DebugManager();
        void drawDebugShapes();
        void setViewMatrix(Matrix4D view);
        void createLine(Vector4D p1, Vector4D p2, Vector4D color);
        void createCube(Vector4D pos, float width, float height, float length, Vector4D color, bool wireframe = false);
        void createSingleFrameLine(Vector4D p1, Vector4D p2, Vector4D color);
        void createSingleFrameCube(Vector4D pos, float width, float height, float length, Vector4D color, bool wireframe = false);
        void clear();
        void clearSingleFrameVector();

        bool* getRenderBool();
        bool* getCreateShapes();
    private:
        static DebugManager* instance;
        DebugManager();
        void addShape(DebugShape* shape);
        void addSingleFrameShape(DebugShape* shape);
        std::vector<std::shared_ptr<DebugShape>> debugShapeVector;
        std::vector<std::shared_ptr<DebugShape>> debugSingleFrameVector;
        Matrix4D viewMatrix;
        bool render = true;
        bool createShapes = true;
};