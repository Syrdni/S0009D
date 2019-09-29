#pragma once
#include "quaternion.h"
#include "Matrix4D.h"

class JointTransform
{
    public:
        JointTransform();
        JointTransform(Matrix4D mat, Quaternion rot);
        ~JointTransform();
        void setRotation(Quaternion rot);
        Quaternion getRotation();
        void setTransform(Matrix4D mat);
        Matrix4D getTransform();

    private:
        Quaternion rotation;                //A joints rotation at a certain keyframe
        Matrix4D transform;                 //A joints transform at a certain keyframe
};