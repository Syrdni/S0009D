#include "animationClip.h"

AnimationClip::AnimationClip()
{

}

AnimationClip::~AnimationClip()
{

}

void AnimationClip::setNumberOfCurves(short numberOfCurves)
{
    curves.clear();

    for (int i = 0; i < numberOfCurves; i++)
    {
        curves.push_back(AnimationCurve());
    }
}

void AnimationClip::setStartKeyIndex(short startkeyIndex)
{
    this->startKeyIndex = startKeyIndex;
}

void AnimationClip::setNumberOfKeys(short numberOfKeys)
{
    this->numberOfKeys = numberOfKeys;
}

void AnimationClip::setKeyStride(short keyStride)
{
    this->keyStride = keyStride;
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
    return curves[index];
}
