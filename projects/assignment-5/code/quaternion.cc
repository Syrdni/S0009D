#include "quaternion.h"

Quaternion::Quaternion()
{

}

Quaternion::~Quaternion()
{

}

Matrix4D Quaternion::createMatrix(float x, float y, float z, float w)
{
    Matrix4D returnMatrix;

    returnMatrix[0] = 1.0f - 2.0f * ( y * y + z * z );
    returnMatrix[1] = 2.0f * ( x * y - w * z );
    returnMatrix[2] = 2.0f * ( x * z + w * y );
    returnMatrix[3] = 0.0f;

    returnMatrix[4] = 2.0f * ( x * y + w * z );
    returnMatrix[5] = 1.0f - 2.0f * ( x * x + z * z );
    returnMatrix[6] = 2.0f * ( y * z - w * x );
    returnMatrix[7] = 0.0f;

    returnMatrix[8] = 2.0f * ( x * z - w * y );
    returnMatrix[9] = 2.0f * ( y * z + w * x );
    returnMatrix[10] = 1.0f - 2.0f * ( x * x + y * y );
    returnMatrix[11] = 0.0f;

    returnMatrix[12] = 0;
    returnMatrix[13] = 0;
    returnMatrix[14] = 0;
    returnMatrix[15] = 1.0f;

    return returnMatrix;


}