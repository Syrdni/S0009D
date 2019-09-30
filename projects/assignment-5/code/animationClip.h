#pragma once
#include <string>
#include <vector>
#include "animationCurve.h"

class AnimationClip
{
    public:
        AnimationClip();
        ~AnimationClip();
        void setNumberOfCurves(short numberOfCurves);
        void setStartKeyIndex(short startKeyIndex);
        void setNumberOfKeys(short numberOfKeys);
        void setKeyStride(short keyStride);
        void setKeyDuration(short duration);
        void setName(char name[50]);
        AnimationCurve& curveByIndex(int curveIndex);
        short getNumberOfKeys();
        short getKeyStride();
        short getKeyDuration();

    private:
        std::string name;
        short startKeyIndex;
        short numberOfKeys;
        short keyStride;
        short keyDuration;
        //preInfinityType
        //postInfinityType
        std::vector<AnimationCurve> curves;
};