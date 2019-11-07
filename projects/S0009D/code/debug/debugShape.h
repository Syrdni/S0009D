#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "../Matrix4D.h"

class DebugShape
{
    public:
        DebugShape();
        virtual void draw(Matrix4D viewMatrix);
    
    protected:
        bool debugMessageSent;
        virtual void setup();
        std::vector<float> vertices;
        std::vector<int> indices;
        unsigned int VAO, EBO, VBO, vertexShader, pixelShader, program;     
};