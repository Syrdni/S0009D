#pragma once
#include "core/app.h"
#include "Matrix4D.h"
#include "mPlane.h"
#include <vector>
#include "string.h"
#include "debug/debugManager.h"

struct AABB
{
    Vector4D minPoint = Vector4D(99999, 99999, 99999, 1);
    Vector4D maxPoint = Vector4D(-99999, -99999, -99999, 1);
    Vector4D getCenter(){return maxPoint - ((maxPoint - minPoint)*0.5);};
};

struct PointAndDistance
{
    Vector4D point;
    float distance;
    PointAndDistance(Vector4D p, float d){point = p; distance = d;};
};

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
        PointAndDistance intersect(mPlane plane);
        PointAndDistance intersect(AABB aabb);
        Vector4D getPoint(int i);

    private:;
        Vector4D origin;
        Vector4D direction;
        float maxDist = 1000;
};