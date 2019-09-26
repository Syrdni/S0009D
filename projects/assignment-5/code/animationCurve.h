#pragma once

class AnimationCurve
{
    public:
        AnimationCurve();
        ~AnimationCurve();
        void setFirstKeyIndex(int keyIndex);
        void setActive(bool var);
        void setStatic(bool var);
        //void setCurveType();
        void setStaticKey(float x, float y, float z, float w);


    private:
        int firstKeyIndex;
        bool isActive;
        bool isStatic;
        //CurveType
        //padding;
        float staticKeyX;
        float staticKeyY;
        float staticKeyZ;
        float staticKeyW;
};