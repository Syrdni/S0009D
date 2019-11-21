#pragma once
#include "../Matrix4D.h"
#include <cmath>

class Quaternion
{
    public:
        Quaternion();
        Quaternion(float x, float y, float z, float w);
        ~Quaternion();

        //Creates a rotation matrix
        static Matrix4D createMatrix(float x, float y, float z, float w);

        static Quaternion createQuaternion(Matrix4D mat);

        //Creates a rotation matrix with the values stored in the class
        Matrix4D createMatrix();

        Quaternion operator *(const Quaternion &q);
        Quaternion operator *(const float q);
        Quaternion operator *(const double q);
        Quaternion operator +(const Quaternion &q);
        Quaternion operator -(const Quaternion &q);
        Quaternion operator -=(const Quaternion &q);


        //Normalizes the values in the quaternion
        void normalize();
        float getX();
        float getY();
        float getZ();
        float getW();
        void setX(float x);
        void setY(float y);
        void setZ(float z);
        void setW(float w);
        Vector4D getVector();

        static float dot_product(Quaternion a, Quaternion b);

    private: 
        float x,y,z,w;
};