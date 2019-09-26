#include "animationCurve.h"

AnimationCurve::AnimationCurve()
{

}

AnimationCurve::~AnimationCurve()
{

}

void AnimationCurve::setFirstKeyIndex(int keyIndex)
{
    this->firstKeyIndex = keyIndex;
}

void AnimationCurve::setActive(bool var)
{
    this->isActive = var;
}

void AnimationCurve::setStatic(bool var)
{
    this->isStatic = var;
}

void AnimationCurve::setStaticKey(float x, float y, float z, float w)
{
    this->staticKeyX = x;
    this->staticKeyY = y;
    this->staticKeyZ = z;
    this->staticKeyW = w;
}