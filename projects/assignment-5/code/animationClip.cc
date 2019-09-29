#include "animationClip.h"

AnimationClip::AnimationClip()
{

}

AnimationClip::~AnimationClip()
{

}

void AnimationClip::setNumberOfCurves(short numberOfCurves)
{
    this->curves.clear();

    for (int i = 0; i < numberOfCurves; i++)
    {
        this->curves.push_back(AnimationCurve());
    }
}

void AnimationClip::setStartKeyIndex(short index)
{
    this->startKeyIndex = index;
}

void AnimationClip::setNumberOfKeys(short keys)
{
    this->numberOfKeys = keys;
}

void AnimationClip::setKeyStride(short stride)
{
    this->keyStride = stride;
}

void AnimationClip::setKeyDuration(short duration)
{
    this->keyDuration = duration;
}

void AnimationClip::setName(char name[50])
{
    this->name = name;
}

AnimationCurve& AnimationClip::curveByIndex(int index)
{
    return this->curves[index];
}

short AnimationClip::getNumberOfKeys()
{
    return this->numberOfKeys;
}

short AnimationClip::getKeyStride()
{
    return this->keyStride;
}
