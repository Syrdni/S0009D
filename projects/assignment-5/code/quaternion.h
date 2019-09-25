#include "Matrix4D.h"

class Quaternion
{
    public:
        Quaternion();
        Quaternion(float x, float y, float z, float w);
        ~Quaternion();
        static Matrix4D createMatrix(float x, float y, float z, float w);
        Matrix4D createMatrix();
        Quaternion operator *(const Quaternion &q);

    private: 
        float x,y,z,w;
};