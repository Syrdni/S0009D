#include "animationCurve.h"

std::string CurveType::toString(Type type)
{
    switch(type)
    {
        case Translation:   return "Translation";
        case Scale:         return "Scale";
        case Rotation:      return "Rotation";
        case Color:         return "Color";
        case Velocity:      return "Velocity";
        case Float4:        return "Float4";
        default:
            return "";
    }
}

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

void AnimationCurve::setCurveType(CurveType::Type curveType)
{
    this->curveType = curveType;
}

void AnimationCurve::setStaticKey(float x, float y, float z, float w)
{
    this->staticKeyX = x;
    this->staticKeyY = y;
    this->staticKeyZ = z;
    this->staticKeyW = w;
}