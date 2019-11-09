#pragma once
#include "mPlane.h"
#include "core/app.h"
#include "string.h"
#include "Matrix4D.h"
#include "Ray.h"
#include "debug/debugManager.h"

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
        Vector4D& getColor();
        float checkIfHit(Ray ray);
    private:
        void setupSquare();
        Vector4D point1, point2, point3, point4;
        Vector4D position, color, normal;
        mPlane plane;
        float height, width;
        unsigned int VAO, EBO, VBO, vertexShader, pixelShader, program;
        float vertices[8] = {
            1.0f, 1.0f,
            1.0f, -1.0f,
            -1.0f, -1.0f,
            -1.0f, 1.0f
        };

        int indices[6] = {
            0, 1, 2,
            0, 3, 2
        };      
};