#pragma once
#include <vector>
#include "animationClip.h"

class AnimationResource
{
    public:
        AnimationResource();
        ~AnimationResource();
        void setupClips(int numberOfClips);
        AnimationClip& getClip(int index);
        int getAmountOfClips();

    private:
        std::vector<AnimationClip> clips;
        int amountOfClips;

};