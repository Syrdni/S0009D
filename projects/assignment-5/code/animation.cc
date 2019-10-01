#include "animation.h"

Animation::Animation(float animationLength, std::vector<KeyFrame> keyFrames, int index)
{
    this->length = animationLength;
    this->keyFrames = keyFrames;
    this->clipIndex = index;
}

Animation::Animation()
{
    this->clipIndex = -1;
}

Animation::~Animation()
{

}

float Animation::getAnimationLength()
{
    return this->length;
}

std::vector<KeyFrame> Animation::getKeyFrames()
{
    return this->keyFrames;
}

int Animation::getClipIndex()
{
    return this->clipIndex;
}