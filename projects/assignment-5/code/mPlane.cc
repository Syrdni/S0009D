#include "mPlane.h"

mPlane::mPlane(Vector4D pos, Vector4D dir)
{
    this->position = pos;
    this->normal = dir;
}

mPlane::~mPlane()
{

}

void mPlane::setPosition(Vector4D pos)
{
    this->position = pos;
}

Vector4D mPlane::getPosition()
{
    return this->position;
}

void mPlane::setNormal(Vector4D dir)
{
    this->normal = dir;
}

Vector4D mPlane::getNormal()
{
    return this->normal;
}