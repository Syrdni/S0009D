#pragma once
#include "keyFrame.h"
#include <vector>

class Animation
{
    public:
        Animation(float animationLength, std::vector<KeyFrame> keyFrames);
        Animation();
        ~Animation();
        float getAnimationLength();
        std::vector<KeyFrame> getKeyFrames();

    private:
        float length;                       //Length of the animation
        std::vector<KeyFrame> keyFrames;    //All the keyframes in the animation
};