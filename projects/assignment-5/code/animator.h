#pragma once

#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include "nax3Structs.h"
#include "animationResource.h"
#include "Vector4D.h"
#include "animation.h"
#include "animatedModel.h"

class Animator
{
    public:
        Animator(AnimatedModel* guy);
        Animator();
        ~Animator();
        bool readNax3File(std::string fileLocation);
        void loadAnimation(int index);
        void setAnimationModel(AnimatedModel* entity);
        void update();

    private:
        AnimationResource* animResource;
        Animation currentAnimation;
        float animationTime = 0;
        AnimatedModel* entity;
        void* memoryBlock;
        void increaseAnimationTime();
        std::map<int, Matrix4D> calculateCurrentAnimationPose();
        std::vector<KeyFrame> getReleventKeyFrames();
        float calculateProgression(KeyFrame previousFrame, KeyFrame nextFrame);
        std::map<int, Matrix4D> interpolatePoses(KeyFrame previousFrame, KeyFrame nextFrame, float progression);
        void applyPose(std::map<int, Matrix4D> currentPose, Joint* joint, Matrix4D parentTransform);
        void resetTimer();
};