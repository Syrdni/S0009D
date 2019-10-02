#pragma once
#include <map>
#include "jointTransform.h"

class KeyFrame
{
    public:
        KeyFrame(std::map<int, JointTransform> modelPose, float timeStamp);
        KeyFrame();
        ~KeyFrame();

        //Get the keyframes timestamp
        float getTimeStamp();

        //Returns the map of jointTransforms that forms the pose the model will have at that keyframe
        std::map<int, JointTransform> getModelPose();

    private:
        //The pose the model will do at that keyframe
        std::map<int, JointTransform> modelPose;

         //At which time the keyframe will occur
        float timeStamp;
};