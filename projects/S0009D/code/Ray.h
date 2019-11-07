#pragma once
#include "core/app.h"
#include "Matrix4D.h"
#include "mPlane.h"
#include <vector>
#include "string.h"

class Ray
{
    public:
        Ray();
        Ray(Vector4D pos, Vector4D dir, bool render);
        ~Ray();
        void setOrigin(Vector4D position);
        Vector4D getOrigin();
        void setDirection(Vector4D direction);
        Vector4D getDirection();
        Vector4D intersect(mPlane plane);
        Vector4D getPoint(int i);

    private:;
        Vector4D origin;
        Vector4D direction;
        float maxDist = 3;
};