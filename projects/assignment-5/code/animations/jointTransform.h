#pragma once
#include "quaternion.h"
#include "Matrix4D.h"

class JointTransform
{
    public:
        JointTransform();
        JointTransform(Matrix4D mat, Quaternion rot);
        ~JointTransform();

        //Set the rotation of the JointTransform
        void setRotation(Quaternion rot);

        //Return the rotation of the JointTransform
        Quaternion getRotation();

        //Set the transform of the JointTransform
        void setTransform(Matrix4D mat);

        //Return the tranform of the JointTransform
        Matrix4D getTransform();

        //Return a JointTransform that is interpolated between JointA and JointB
        static JointTransform interpolateJointTransform(JointTransform JointA, JointTransform JointB, float progression);

        //Returns a matrix4D that is the combined matrix of rotation and transform
        Matrix4D getCombinedMatrix();

    private:
        //A joints rotation at a certain keyframe
        Quaternion rotation;

         //A joints transform at a certain keyframe
        Matrix4D transform;

        //Return a matrix4D that is interpolated between the start and end (Transform)
        static Matrix4D interpolate(Matrix4D start, Matrix4D end, float progression);

        //Returns a quaternion that is interpolated between the start and end (Quatenion)
        static Quaternion interpolate(Quaternion start, Quaternion end, double progression);
};