#include "Matrix4D.h"

class Quaternion
{
    public:
        Quaternion();
        ~Quaternion();
        static Matrix4D createMatrix(float x, float y, float z, float w);
        //Quaternion operator *(const Quaternion &q);

    private: 
        float x,y,z,w;
};