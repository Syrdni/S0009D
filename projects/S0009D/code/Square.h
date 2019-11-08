#pragma once
#include "mPlane.h"
#include "core/app.h"
#include "string.h"
#include "Matrix4D.h"

class Square
{
    public:
        Square();
        Square(Vector4D pos, Vector4D normal, float w, float h, Vector4D rgb);
        ~Square();
        void update(Matrix4D viewMatrix);
        void drawSquare(Matrix4D viewMatrix);
        mPlane getPlane();
        mPlane& getReferenceToPlane();
        Vector4D& getPosition();
        Vector4D& getNormal();
    private:
        void setupSquare();
        Vector4D position, color, normal;
        mPlane plane;
        float height, width;
        unsigned int VAO, EBO, VBO, vertexShader, pixelShader, program;        
};