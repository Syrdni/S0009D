#pragma once
#include "keyFrame.h"
#include <vector>

class Animation
{
    public:
        Animation(float animationLength, std::vector<KeyFrame> keyFrames, int index);
        Animation();
        ~Animation();
        float getAnimationLength();
        std::vector<KeyFrame> getKeyFrames();
        int getClipIndex();

    private:
        int clipIndex;
        float length;                       //Length of the animation
        std::vector<KeyFrame> keyFrames;    //All the keyframes in the animation
};