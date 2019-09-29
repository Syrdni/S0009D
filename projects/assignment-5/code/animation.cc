#include "animation.h"

Animation::Animation(float animationLength, KeyFrame* keyFrames)
{
    this->length = animationLength;
    this->keyFrames = keyFrames;
}

Animation::~Animation()
{

}

float Animation::getAnimationLength()
{
    return this->length;
}

KeyFrame* Animation::getKeyFrames()
{
    return this->keyFrames;
}