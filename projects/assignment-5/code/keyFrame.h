#pragma once
#include <map>
#include "jointTransform.h"

class KeyFrame
{
    public:
        KeyFrame(std::map<int, JointTransform> modelPose, float timeStamp);
        KeyFrame();
        ~KeyFrame();
        float getTimeStamp();
        std::map<int, JointTransform> getModelPose();

    private:
        std::map<int, JointTransform> modelPose;    //The pose the model will do at that keyframe
        float timeStamp;                            //At which time the keyframe will occur
};