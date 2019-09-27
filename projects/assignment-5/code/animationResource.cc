#include "animationResource.h"

AnimationResource::AnimationResource()
{

}

AnimationResource::~AnimationResource()
{

}

void AnimationResource::setupClips(int numberOfClips)
{
    this->clips.clear();
    for (int i = 0; i < numberOfClips; i++)
    {
        this->clips.push_back(AnimationClip());
    }
    
}

AnimationClip& AnimationResource::getClip(int index)
{
    return this->clips[index];
}