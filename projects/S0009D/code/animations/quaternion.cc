#include "quaternion.h"

Quaternion::Quaternion()
{

}

Quaternion::Quaternion(float x, float y, float z, float w)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
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

Matrix4D Quaternion::createMatrix()
{
    Matrix4D returnMatrix;

    //returnMatrix[0] = 1.0f - (2.0f * y * y) - (2.0f * z * z);
    //returnMatrix[1] = (2.0f * x * y) - (2.0f * w * z);
    //returnMatrix[2] = (2.0f * x * z) + (2.0f * w * y);
    //returnMatrix[3] = 0.0f;
//
    //returnMatrix[4] = (2.0f * x * y) + (2.0f * w * z);
    //returnMatrix[5] = 1.0f - (2.0f * x * x) - (2.0f * z * z);
    //returnMatrix[6] = (2.0f * y * z) - (2.0f * w * x);
    //returnMatrix[7] = 0.0f;
//
    //returnMatrix[8] = (2.0f * x * z) - (2.0f * w * y);
    //returnMatrix[9] = (2.0f * y * z) + (2.0f * w * x);
    //returnMatrix[10] = 1.0f - (2.0f * x * x) - (2.0f * y * y);
    //returnMatrix[11] = 0.0f;
//
    //returnMatrix[12] = 0;
    //returnMatrix[13] = 0;
    //returnMatrix[14] = 0;
    //returnMatrix[15] = 1.0f;


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

Quaternion Quaternion::createQuaternion(Matrix4D m)
{
    float i, j, k, r, s;
    float tr = m[0] + m[5] + m[10];
    if(tr >= 0)
    {
        s = sqrt(tr + 1);
        r = 0.5 * s;
        s = 0.5 / s;
        i = (m[9] - m[6]) * s;
        j = (m[2] - m[8]) * s;
        k = (m[4] - m[1]) * s;
    }
    else
    {
        int i = 0;
        if(m[5] > m[0])
            i = 1;
        if(m[10] > m[(i*4)+i])
            i = 2;
        switch (i)
        {
        case 0:
            s = sqrt((m[0] - (m[5] + m[10])) + 1);
            i = 0.5 * s;
            s = 0.5 / s;
            j = (m[1] + m[4]) * s;
            k = (m[8] + m[2]) * s;
            r = (m[9] - m[6]) * s;
            break;
        case 1:
            s = sqrt((m[5] - (m[10] + m[0])) + 1);
            j = 0.5 * s;
            s = 0.5 / s;
            k = (m[6] + m[9]) * s;
            i = (m[1] + m[4]) * s;
            r = (m[2] - m[8]) * s;
            break;
        case 2:
            s = sqrt((m[10] - (m[0] + m[5])) + 1);
            k = 0.5 * s;
            s = 0.5 / s;
            i = (m[8] + m[2]) * s;
            j = (m[6] + m[9]) * s;
            r = (m[4] - m[1]) * s;
        }
    }
    return Quaternion(i, j, k, r);
    //float w = sqrt(1.0 + mat[0] + mat[5] + mat[10])/2.0;
    //float x = sqrt(mat[9] - mat[6]) / (w*4.0);
    //float y = sqrt(mat[2] - mat[8]) / (w*4.0);
    //float z = sqrt(mat[4] - mat[1]) / (w*4.0);
    //return Quaternion(x, y, z, w);
}

Quaternion Quaternion::operator*(const Quaternion &q)
{
    Quaternion r;
    r.x =  x * q.w + y * q.z - z * q.y + w * q.x;
    r.y = -x * q.z + y * q.w + z * q.x + w * q.y;
    r.z =  x * q.y - y * q.x + z * q.w + w * q.z;
    r.w = -x * q.x - y * q.y - z * q.z + w * q.w;

    //r.x = w*q.x + x*q.w + y*q.z - z*q.y;
    //r.y = w*q.y + y*q.w + z*q.x - x*q.z;
    //r.z = w*q.z + z*q.w + x*q.y - y*q.x;
    //r.w = w*q.w - x*q.x - y*q.y - z*q.z;

    //r.x = w*q.x + x*q.w + y*q.z - z*q.y;
    //r.y = w*q.y + y*q.w + z*q.x - x*q.z;
    //r.z = w*q.z + z*q.w + x*q.y - y*q.x;
    //r.w = w*q.w - x*q.x - y*q.y - z*q.z;

    //r.w = w*q.w;
    //r.x = x*q.x;
    //r.y = y*q.y;
    //r.z = z*q.z;

    return r;
}

Quaternion Quaternion::operator*(const float q)
{
    Quaternion r;
	r.x = x*q;
	r.y = y*q;
	r.z = z*q;
    r.w = w*q;

    return r;
}

Quaternion Quaternion::operator*(const double q)
{
    Quaternion r;
	r.x = x*q;
	r.y = y*q;
	r.z = z*q;
    r.w = w*q;

    return r;
}

Quaternion Quaternion::operator+(const Quaternion &q)
{
    Quaternion r;
	r.x = x+q.x;
	r.y = y+q.y;
	r.z = z+q.z;
    r.w = w+q.w;

    return r;
}

Quaternion Quaternion::operator-(const Quaternion &q)
{
    Quaternion r;

	r.x = x-q.x;
	r.y = y-q.y;
	r.z = z-q.z;
	r.w = w-q.w;

    return r;
}

Quaternion Quaternion::operator-=(const Quaternion &q)
{
    w -= q.w;
    x -= q.x;
    y -= q.y;
    z -= q.z;

  return (*this);
}

void Quaternion::normalize()
{
    float mag = (float)sqrt(w*w+x*x+y*y+z*z);
    w /= mag;
    x /= mag;
    y /= mag;
    z /= mag;
}

float Quaternion::getX()
{
    return x;
}

float Quaternion::getY()
{
    return y;
}

float Quaternion::getZ()
{
    return z;
}

float Quaternion::getW()
{
    return w;
}

void Quaternion::setX(float x)
{
    this->x = x;
}

void Quaternion::setY(float y)
{
    this->y = y;
}

void Quaternion::setZ(float z)
{
    this->z = z;
}

void Quaternion::setW(float w)
{
    this->w = w;
}

float Quaternion::dot_product(Quaternion a, Quaternion b)
{
    return a.getX() * b.getX() + a.getY() * b.getY() + a.getZ() * b.getZ() + a.getW() * b.getW();
}

Vector4D Quaternion::getVector()
{
    return Vector4D(x, y, z, w);
}