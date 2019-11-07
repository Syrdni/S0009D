#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "../Matrix4D.h"
#include "stb_image.h"
#include <fstream>
#include <iostream>
#include <sstream>

class DebugShape
{
    public:
        DebugShape();
        virtual void draw(Matrix4D viewMatrix);
    
    protected:
        bool debugMessageSent;
        virtual void setup();
        std::string setupFragmentShader();
        std::string setupVertexShader();
        void linkVertexShader();
        void linkFragmentShader();
        std::vector<float> vertices;
        std::vector<int> indices;
        unsigned int VAO, EBO, VBO, vertexShader, pixelShader, program;
        const char* vs;
        const char* fs; 
};