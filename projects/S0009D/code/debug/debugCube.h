#pragma once
#include "debugShape.h"
#include "../Matrix4D.h"
#include "core/app.h"
#include "string.h"

class DebugCube : public DebugShape
{
    public:
        DebugCube();
        DebugCube(Vector4D pos, float width, float height, float length);
        ~DebugCube();
        void draw(Matrix4D viewMatrix);
    
    private:
        void setup();
};