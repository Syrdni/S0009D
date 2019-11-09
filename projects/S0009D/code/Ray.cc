#include "Ray.h"

Ray::Ray(){}

Ray::Ray(Vector4D pos, Vector4D dir)
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

PointAndDistance Ray::intersect(mPlane plane)
{
    //Check if the ray is parallel to the plane. If that is the case the ray will never hit the plane
    float d = Vector4D::dotProduct(plane.getNormal(), this->direction);
    if (d == 0)
        return PointAndDistance(Vector4D(0,0,0,-1), -1);
    
    //Calculate t
    float t = Vector4D::dotProduct(plane.getPosition()-this->origin, plane.getNormal()) / d;

    if (t < 0)
    {
        return PointAndDistance(Vector4D(0,0,0,-1), -1);
    }
    
    //With t calculated we now can find where the ray is intersected with the plane
    return PointAndDistance((this->origin + this->direction * t), t);
}

Vector4D Ray::getPoint(int i)
{
    if (i == 0)
        return origin;
    else
        return origin+(direction*maxDist);
}