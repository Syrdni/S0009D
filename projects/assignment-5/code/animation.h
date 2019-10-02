#pragma once
#include "keyFrame.h"
#include <vector>

class Animation
{
    public:
        Animation(float animationLength, std::vector<KeyFrame> keyFrames, int index);
        Animation();
        ~Animation();

        //Returns the length of the whole animation
        float getAnimationLength();

        //Returns the vector with all the keyframes
        std::vector<KeyFrame> getKeyFrames();

        //Returns which clip index the animation have
        int getClipIndex();

    private:
        //Which clipindex the animation have
        int clipIndex;

        //Length of the animation
        float length;

        //All the keyframes in the animation
        std::vector<KeyFrame> keyFrames;
};