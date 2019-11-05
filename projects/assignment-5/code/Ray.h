#pragma once
#include "Vector4D.h"
#include "mPlane.h"

class Ray
{
    public:
    Ray();
    Ray(Vector4D pos, Vector4D dir);
    ~Ray();
    void setOrigin(Vector4D position);
    Vector4D getOrigin();
    void setDirection(Vector4D direction);
    Vector4D getDirection();
    Vector4D intersect(mPlane plane);
    private:
        Vector4D origin;
        Vector4D direction;
};