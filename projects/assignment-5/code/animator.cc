#include "animator.h"

Animator::Animator()
{

}

Animator::~Animator()
{

}

bool Animator::readNax3File(std::string fileLocation)
{
    std::ifstream infile(fileLocation.c_str(), std::ifstream::binary);

    //Check if the file exists
    if (!infile)
        return false;
    
    std::filebuf* pbuf = infile.rdbuf();
    std::size_t size = pbuf->pubseekoff(0, infile.end, infile.in);
    pbuf->pubseekpos(0, infile.in);
    char* ptr = new char[size];
    pbuf->sgetn(ptr,size);
    infile.close();

    //Cast to naxHeader
    Nax3Header* naxHeader = (Nax3Header*) ptr;
    //Move pointer forward
    ptr += sizeof(Nax3Header);

    AnimationResource* anim = new AnimationResource();

    if (naxHeader->magic != NAX3_MAGICNUMBER)
        return false;

    if (naxHeader->numClips > 0)
    {
        //setup animation clips
        anim->setupClips(naxHeader->numClips);
        int clipIndex;
        int numClips = naxHeader->numClips;
        for (clipIndex = 0; clipIndex < numClips; clipIndex++)
        {
            Nax3Clip* naxClip = (Nax3Clip*)ptr;
            ptr += sizeof(Nax3Clip);

            //Setup animation clip object
            AnimationClip& clip = anim->getClip(clipIndex);
            clip.setNumberOfCurves(naxClip->numCurves);
            clip.setStartKeyIndex(naxClip->startKeyIndex);
            clip.setNumberOfKeys(naxClip->numKeys);
            clip.setKeyStride(naxClip->keyStride);
            clip.setKeyDuration(naxClip->keyDuration);
            clip.setName(naxClip->name);

            //Skip these
            int eventIndex;
            for (int eventIndex = 0; eventIndex < naxClip->numEvents; eventIndex++)
            {
                Nax3AnimEvent* naxEvent = (Nax3AnimEvent*)ptr;
                ptr += sizeof(Nax3AnimEvent);
            }

            int curveIndex;
            for (int curveIndex = 0; curveIndex < naxClip->numCurves; curveIndex++)
            {
                Nax3Curve* naxCurve = (Nax3Curve*)ptr;
                ptr += sizeof(Nax3Curve);

                AnimationCurve& animCurve = clip.curveByIndex(curveIndex);
                animCurve.setFirstKeyIndex(naxCurve->firstKeyIndex);
                animCurve.setActive(naxCurve->isActive != 0);
                animCurve.setStatic(naxCurve->isStatic != 0);
                animCurve.setCurveType((CurveType::Type)naxCurve->curveType);
                animCurve.setStaticKey(naxCurve->staticKeyX, naxCurve->staticKeyY, naxCurve->staticKeyZ, naxCurve->staticKeyW);
                int lul = 0;
            }    
        }

        void* keyPtr[naxHeader->numKeys * sizeof(Vector4D)];
        memcpy(keyPtr, ptr, (naxHeader->numKeys * sizeof(Vector4D)));
        memoryBlock = keyPtr;
        animResource = anim;
    }
}