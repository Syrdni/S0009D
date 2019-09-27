#pragma once
#include <string>

class CurveType
{
    public:
        enum Type
        {
            Translation,
            Scale,
            Rotation,
            Color,
            Velocity,
            Float4,

            NumCurveTypes,
            InvalidCurveType,
        };

    static std::string toString(Type type);
};

class AnimationCurve
{
    public:
        AnimationCurve();
        ~AnimationCurve();
        void setFirstKeyIndex(int keyIndex);
        void setActive(bool var);
        void setStatic(bool var);
        void setCurveType(CurveType::Type curveType);
        void setStaticKey(float x, float y, float z, float w);


    private:
        int firstKeyIndex;
        bool isActive;
        bool isStatic;
        CurveType::Type curveType;
        //padding;
        float staticKeyX;
        float staticKeyY;
        float staticKeyZ;
        float staticKeyW;
};