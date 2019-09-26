#include "animationResource.h"

AnimationResource::AnimationResource()
{

}

AnimationResource::~AnimationResource()
{

}

void AnimationResource::setupClips(int numberOfClips)
{
    clips.clear();
    for (int i = 0; i < numberOfClips; i++)
    {
        clips.push_back(AnimationClip());
    }
    
}

AnimationClip& AnimationResource::getClip(int index)
{
    return clips[index];
}