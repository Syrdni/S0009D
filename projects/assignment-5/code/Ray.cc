#include "Ray.h"

const GLchar* vs2 =
"#version 430\n"
"layout(location=0) in vec3 pos;\n"
"layout(location=0) out vec4 Color;\n"
"uniform mat4 aModelMatrix;\n"
"uniform mat4 aPerspectiveTransform;\n"
"uniform vec4 aColor;\n"
"void main()\n"
"{\n"
"	gl_Position = aPerspectiveTransform * aModelMatrix * vec4(pos, 1);\n"
"	Color = aColor;\n"
"}\n";

const GLchar* ps2 =
"#version 430\n"
"layout(location=0) in vec4 color;\n"
"out vec4 Color;\n"
"void main()\n"
"{\n"
"	Color = color;\n"
"}\n";

Ray::Ray(){}

Ray::Ray(Vector4D pos, Vector4D dir, bool render)
{
    this->origin = pos;
    this->direction = dir;
}

Ray::~Ray(){};

void Ray::setOrigin(Vector4D pos)
{
    this->origin = pos;
}

Vector4D Ray::getOrigin()
{
    return this->origin;
}

void Ray::setDirection(Vector4D dir)
{
    this->direction = dir;
}

Vector4D Ray::getDirection()
{
    return this->direction;
}

Vector4D Ray::intersect(mPlane plane)
{
    //Check if the ray is parallel to the plane. If that is the case the ray will never hit the plane
    float d = Vector4D::dotProduct(plane.getNormal(), this->direction);
    if (d == 0)
        return Vector4D(0,0,0,1);
    
    //Calculate t
    float t = Vector4D::dotProduct(plane.getPosition()-this->origin, plane.getNormal()) / d;

    if (t < 0)
    {
        std::cout << "Miss" << std::endl;
        return Vector4D(0,0,0,1);
    }
    

    //With t calculated we now can find where the ray is intersected with the plane
    std::cout << "Hit" << std::endl;
    return (this->origin + this->direction * t);
}

Vector4D Ray::getPoint(int i)
{
    if (i == 0)
        return origin;
    else
        return origin+(direction*maxDist);
}