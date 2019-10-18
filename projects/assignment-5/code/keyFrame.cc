#include "keyFrame.h"

KeyFrame::KeyFrame(std::map<int, JointTransform> modelPose, float timeStamp)
{
    this->modelPose = modelPose;
    this->timeStamp = timeStamp;
}

KeyFrame::KeyFrame()
{
    
}

KeyFrame::~KeyFrame()
{

}

std::map<int, JointTransform> KeyFrame::getModelPose()
{
    return modelPose;
}

float KeyFrame::getTimeStamp()
{
    return timeStamp;
}

void KeyFrame::setTimeStamp(int time)
{
    timeStamp = time;
}