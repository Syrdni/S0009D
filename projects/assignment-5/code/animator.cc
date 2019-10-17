#include "animator.h"

Animator::Animator(AnimatedModel* guy)
{
    this->entity = guy;
}

Animator::Animator()
{

}

Animator::~Animator()
{

}

bool Animator::readNax3File(std::string fileLocation)
{
    //Open the Nax3 File
    std::ifstream infile(fileLocation.c_str(), std::ifstream::binary);

    //Check if the file exists
    if (!infile)
        return false;
    
    //Get the pointer to the buffer object
    std::filebuf* pbuf = infile.rdbuf();

    //Get the file size
    std::size_t size = pbuf->pubseekoff(0, infile.end, infile.in);
    pbuf->pubseekpos(0, infile.in);

    //Allocate memory to contain the data
    char* ptr = new char[size];

    //Get the file data
    pbuf->sgetn(ptr,size);
    infile.close();

    //Cast to the Nax3Header struct
    Nax3Header* naxHeader = (Nax3Header*) ptr;

    //Move pointer forward
    ptr += sizeof(Nax3Header);

    AnimationResource* anim = new AnimationResource();

    //Check if the file we just read was a Nax3 file
    if (naxHeader->magic != NAX3_MAGICNUMBER)
        return false;

    //Check if the file contains any clips
    if (naxHeader->numClips > 0)
    {
        //Create x amounts of empty clips
        anim->setupClips(naxHeader->numClips);
        int clipIndex;
        int numClips = naxHeader->numClips;
        for (clipIndex = 0; clipIndex < numClips; clipIndex++)
        {
            //Cast to Nax3Clip
            Nax3Clip* naxClip = (Nax3Clip*)ptr;
            //Move pointer forward
            ptr += sizeof(Nax3Clip);

            //Setup animation clip object
            AnimationClip& clip = anim->getClip(clipIndex);
            clip.setNumberOfCurves(naxClip->numCurves);
            clip.setStartKeyIndex(naxClip->startKeyIndex);
            clip.setNumberOfKeys(naxClip->numKeys);
            clip.setKeyStride(naxClip->keyStride);
            clip.setKeyDuration(naxClip->keyDuration);
            clip.setName(naxClip->name);

            //We dont use these, skip them
            int eventIndex;
            for (int eventIndex = 0; eventIndex < naxClip->numEvents; eventIndex++)
            {
                Nax3AnimEvent* naxEvent = (Nax3AnimEvent*)ptr;
                ptr += sizeof(Nax3AnimEvent);
            }

            int curveIndex;
            for (int curveIndex = 0; curveIndex < naxClip->numCurves; curveIndex++)
            {
                //Cast to Nax3Curve
                Nax3Curve* naxCurve = (Nax3Curve*)ptr;

                //Move pointer forward
                ptr += sizeof(Nax3Curve);                

                //Setup animation curve
                AnimationCurve& animCurve = clip.curveByIndex(curveIndex);
                animCurve.setFirstKeyIndex(naxCurve->firstKeyIndex);
                animCurve.setActive(naxCurve->isActive != 0);
                animCurve.setStatic(naxCurve->isStatic != 0);
                animCurve.setCurveType((CurveType::Type)naxCurve->curveType);
                animCurve.setStaticKey(naxCurve->staticKeyX, naxCurve->staticKeyY, naxCurve->staticKeyZ, naxCurve->staticKeyW);
            }    
        }

        //Save the rest of the data
        void* keyPtr[naxHeader->numKeys * sizeof(Vector4D)];
        memcpy(keyPtr, ptr, (naxHeader->numKeys * sizeof(Vector4D)));
        memoryBlock = keyPtr;

        //Save the animationResource for later use
        animResource = anim;
        return true;
    }
}

void Animator::loadAnimation(int clipIndex)
{
    if (clipIndex == -1)
    {
        entity->isPlaying(false);
        return;
    }

    //Check if the index is vaild
    if (clipIndex >= animResource->getAmountOfClips() || clipIndex < 0)
        return;

    //Reset animation timer even if it is the same animation
    resetTimer();

    //Check if the new animation is the same animation already playing
    if (clipIndex == currentAnimation.getClipIndex())
        return;

    entity->isPlaying(true);
    //Create a new animation
    std::vector<KeyFrame> animation;

    //Get the animation clip
    AnimationClip clip = animResource->getClip(clipIndex);

    //Block of memory
    char* ptr = (char*)memoryBlock;

    //Move pointer forward
    ptr += clip.curveByIndex(0).getFirstKeyIndex() * sizeof(Vector4D);


    for (int i = 0; i < clip.getNumberOfKeys(); i++)
    {
        //Create a new keyFrame
        std::map<int, JointTransform> modelPose;

        for (int j = 0; j < clip.getKeyStride()/4; j++)
        {
            JointTransform jointTrans;
            for (int k = 0; k < 4; k++)
            {            
                //Get the information and put it into a Vector4D
                Vector4D* vec4 = (Vector4D*)ptr;

                //Move the pointer forward
                ptr += sizeof(Vector4D);

                //Check which kind of curve it is
                switch(clip.curveByIndex((4*j)+k).getCurveType())
                {
                    case CurveType::Translation:
                        jointTrans.setTransform(Matrix4D(1,0,0, vec4->getFloat(0), 0,1,0, vec4->getFloat(1), 0,0,1, vec4->getFloat(2), 0,0,0,1));
                    case CurveType::Rotation:      
                        jointTrans.setRotation(Quaternion(vec4->getFloat(0), vec4->getFloat(1), vec4->getFloat(2), vec4->getFloat(3)));
                }
            }
            modelPose.insert(std::pair<int, JointTransform>(j, jointTrans));
        }
        //Insert keyFrame into the animation
        KeyFrame keyFrame = KeyFrame(modelPose, 40*i);
        animation.push_back(keyFrame);
    }
    currentAnimation = Animation((clip.getNumberOfKeys()-1)*clip.getKeyDuration(), animation, clipIndex);
    return;
}

void Animator::setAnimationModel(AnimatedModel* am)
{
    this->entity = am;
}

void Animator::update()
{
    //Check if an animation is running
    if (currentAnimation.getClipIndex() == -1){
        start = std::chrono::steady_clock::now();
        return;
    }
    
    increaseAnimationTime();
    std::map<int, Matrix4D> currentPos = calculateCurrentAnimationPose();
    applyPose(currentPos, entity->getRootJoint(), Matrix4D());
    //start = std::chrono::steady_clock::now();    
}

void Animator::increaseAnimationTime()
{    
    auto end = std::chrono::steady_clock::now();
    int elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    animationTime += (elapsed/msBetweenFrames)*10;
    if (animationTime > currentAnimation.getAnimationLength())
    {
        animationTime = 0;
    }
    start = end;
}

std::map<int, Matrix4D> Animator::calculateCurrentAnimationPose()
{
    std::vector<KeyFrame> frames = getReleventKeyFrames();
    float progression = calculateProgression(frames[0], frames[1]);
    return interpolatePoses(frames[0], frames[1], progression);
}

std::vector<KeyFrame> Animator::getReleventKeyFrames()
{
    std::vector<KeyFrame> frames = currentAnimation.getKeyFrames();
    KeyFrame previousFrame, nextFrame;

    for (int i = 0; i < frames.size(); i++)
    {
        nextFrame = frames[i];

        //If the keyframes timestamp is bigger than the current animation time then we have found our next frame
        if (nextFrame.getTimeStamp() > (int)animationTime)
            break;

        previousFrame = frames[i]; 
    }
       
    std::vector<KeyFrame> v = {previousFrame, nextFrame};
    return v;
}

float Animator::calculateProgression(KeyFrame previousFrame, KeyFrame nextFrame)
{
    float totalTime = nextFrame.getTimeStamp() - previousFrame.getTimeStamp();
    float currentTime = (int)animationTime - previousFrame.getTimeStamp();
    return currentTime/totalTime;
}

std::map<int, Matrix4D> Animator::interpolatePoses(KeyFrame previousFrame, KeyFrame nextFrame, float progression)
{
    std::map<int, Matrix4D> currentPose;
    for (int i = 0; i < previousFrame.getModelPose().size(); i++)
    {
        JointTransform previousTransfrom = previousFrame.getModelPose()[i];
        JointTransform nextTransfrom = nextFrame.getModelPose()[i];
        JointTransform currentTransform = JointTransform::interpolateJointTransform(previousTransfrom, nextTransfrom, progression);
        currentPose.insert(std::pair<int, Matrix4D>(i, currentTransform.getCombinedMatrix()));
    }
    return currentPose;
}

void Animator::applyPose(std::map<int, Matrix4D> currentPose, Joint* joint, Matrix4D parentTransform)
{
    Matrix4D currentLocalTransform = currentPose[joint->getID()];
    Matrix4D currentTransform = parentTransform * currentLocalTransform;
    for (int i = 0; i < joint->getChildren().size(); i++)
    {
        applyPose(currentPose, joint->getChildren()[i], currentTransform);
    }
    joint->worldPosition = currentTransform;
    joint->transform = currentTransform * joint->inverseBindTransform;   
}

void Animator::resetTimer()
{
    this->animationTime = 0;
}