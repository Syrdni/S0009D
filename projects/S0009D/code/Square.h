#pragma once
#include "mPlane.h"
#include "core/app.h"
#include "string.h"
#include "Matrix4D.h"

class Square
{
    public:
        Square();
        Square(Vector4D pos, float w, float h, Vector4D rgb);
        ~Square();
        void drawSquare(Matrix4D modelMatrix, Matrix4D viewMatrix);
    private:
        void setupSquare();
        Vector4D position, color;
        float height, width;
        unsigned int VAO, EBO, VBO, vertexShader, pixelShader, program;        
};