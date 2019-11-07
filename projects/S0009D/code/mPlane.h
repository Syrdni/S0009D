#pragma once
#include "Vector4D.h"
class mPlane
{
    public:
        mPlane();
        mPlane(Vector4D pos, Vector4D dir);
        ~mPlane();
        void setPosition(Vector4D pos);
        Vector4D getPosition();
        void setNormal(Vector4D dir);
        Vector4D getNormal();
        Vector4D& getReferenceToNormal();
    private:
        Vector4D position;
        Vector4D normal;
};