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
        static JointTransform interpolateJointTransform(JointTransform JointA, JointTransform JointB, float progression);
        Matrix4D getCombinedMatrix();

    private:
        Quaternion rotation;                //A joints rotation at a certain keyframe
        Matrix4D transform;                 //A joints transform at a certain keyframe
        static Matrix4D interpolate(Matrix4D start, Matrix4D end, float progression);
        static Quaternion interpolate(Quaternion start, Quaternion end, float progression);
};