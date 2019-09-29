#pragma once
#include "keyFrame.h"

class Animation
{
    public:
        Animation(float animationLength, KeyFrame* keyFrames);
        ~Animation();
        float getAnimationLength();
        KeyFrame* getKeyFrames();

    private:
        float length;           //Length of the animation
        KeyFrame* keyFrames;    //All the keyframes in the animation
};