#pragma once

#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include "nax3Structs.h"
#include "animationResource.h"
#include "../Vector4D.h"
#include "animation.h"
#include "animatedModel.h"
#include <chrono>

class Animator
{
    public:
        //Constructor
        Animator(AnimatedModel* model);

        //Default Constructor.
        Animator();

        //Deconstructor
        ~Animator();

        //Reads a Nax3File and creates a animationResource and saves the data for the joints
        bool readNax3File(std::string fileLocation);

        //Loads an animation to play
        void loadAnimation(int index);

        //Set the model you want to animate
        void setAnimationModel(AnimatedModel* entity);

        //Updates and moves the model each call
        void update();

    private:
        //Contains the information to load an animation from the memoryBlock
        AnimationResource* animResource;

        //The current animation playing
        Animation currentAnimation;

        //The current time of the animation. Measured in frames
        float animationTime = 0;

        //The model that is going to be animated
        AnimatedModel* entity;

        //Data that contains all the information of the animation in binary format
        void* memoryBlock;

        //Increases the animationTime and loops the animation if it ends
        void increaseAnimationTime();

        //Calculates which pose the entity is going to be in
        std::map<int, Matrix4D> calculateCurrentAnimationPose();

        //Returns the previous and the next keyframe depending on where the animationTime
        std::vector<KeyFrame> getReleventKeyFrames();

        //Calculates how close to the next keyframe we are on a scale on 0 to 1
        float calculateProgression(KeyFrame previousFrame, KeyFrame nextFrame);

        //Interpolates between two poses
        std::map<int, Matrix4D> interpolatePoses(KeyFrame previousFrame, KeyFrame nextFrame, float progression);
        
        //Applys the pose to then animated model
        void applyPose(std::map<int, Matrix4D> currentPose, Joint* joint, Matrix4D parentTransform);

        //Resets the animation timer to 0
        void resetTimer();

        int numberOfFramesPerSec = 24;

        float msBetweenFrames = 1000/numberOfFramesPerSec;

        int deltaTimeStamp = 0;

        //Used to calculate the time since the last frame
       std::chrono::time_point<std::chrono::steady_clock> start;
};