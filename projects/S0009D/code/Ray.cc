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
    if (d >= 0)
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

PointAndDistance Ray::intersect(AABB aabb)
{
    float temp;

    // Calculate T for when we pass the X axis of the bounding box;
    float txMin = (aabb.minPoint[0] - this->origin[0]) / this->direction[0];
    float txMax = (aabb.maxPoint[0] - this->origin[0]) / this->direction[0];
    if(txMax < txMin)
    {
        temp = txMax; 
        txMax = txMin; 
        txMin = temp;
    }
    //DebugManager::getInstance()->createCube(this->origin + this->direction * txMin, 0.05, 0.05, 0.05, Vector4D(1, 0, 0, 1), false);
    //DebugManager::getInstance()->createCube(this->origin + this->direction * txMax, 0.05, 0.05, 0.05, Vector4D(1, 0, 0, 1), false);

    // Calculate T for when we pass the Y axis of the bounding box;
    float tyMin = (aabb.minPoint[1] - this->origin[1]) / this->direction[1];
    float tyMax = (aabb.maxPoint[1] - this->origin[1]) / this->direction[1];
    if(tyMax < tyMin)
    {
        temp = tyMax; 
        tyMax = tyMin; 
        tyMin = temp;
    }
    //DebugManager::getInstance()->createCube(this->origin + this->direction * tyMin, 0.05, 0.05, 0.05, Vector4D(0, 1, 0, 1), false);
    //DebugManager::getInstance()->createCube(this->origin + this->direction * tyMax, 0.05, 0.05, 0.05, Vector4D(0, 1, 0, 1), false);

    // Calculate T for when we pass the Z axis of the bounding box;
    float tzMin = (aabb.minPoint[2] - this->origin[2]) / this->direction[2];
    float tzMax = (aabb.maxPoint[2] - this->origin[2]) / this->direction[2];
    if(tzMax < tzMin)
    {
        temp = tzMax; 
        tzMax = tzMin; 
        tzMin = temp;
    }
    //DebugManager::getInstance()->createCube(this->origin + this->direction * tzMin, 0.05, 0.05, 0.05, Vector4D(0, 0, 1, 1), false);
    //DebugManager::getInstance()->createCube(this->origin + this->direction * tzMax, 0.05, 0.05, 0.05, Vector4D(0, 0, 1, 1), false);
    
    //Calculate the greatest min and smallest max
    float tMin = (txMin > tyMin)? txMin : tyMin;
    float tMax = (txMax < tyMax)? txMax : tyMax;

    //Did not intersect with the AABB
    if (txMin > tyMax || tyMin > txMax)
        return PointAndDistance(Vector4D(0, 0, 0, -1), -1);
    if (tMin > tzMax || tzMin > tMax)
        return PointAndDistance(Vector4D(0, 0, 0, -1), -1);

    //Did intersect with the AABB
    if (tzMin > tMin)
        tMin = tzMin;
    if (tzMax = tMax)
        tMax = tzMax;

    DebugManager::getInstance()->createCube(this->origin + this->direction * tMin, 0.02, 0.02, 0.02, Vector4D(0, 0, 0, 1), false);
    DebugManager::getInstance()->createCube(this->origin + this->direction * tMax, 0.02, 0.02, 0.02, Vector4D(0, 0, 0, 1), false);      

    //Return the point closest to the camera
    return PointAndDistance(this->origin + this->direction * tMin, tMin); 
}

Vector4D Ray::getPoint(int i)
{
    if (i == 0)
        return origin;
    else
        return origin+(direction*maxDist);
}