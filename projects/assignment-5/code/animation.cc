#include "animation.h"

Animation::Animation(float animationLength, std::vector<KeyFrame> keyFrames)
{
    this->length = animationLength;
    this->keyFrames = keyFrames;
}

Animation::Animation()
{
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