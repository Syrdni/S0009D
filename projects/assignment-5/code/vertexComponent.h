#pragma once
enum VertexType
{
    Position = 0,
    Normal = 1,
    TexCoord1 = 2,
    TexCoord2 = 3,
    Color = 4,
    Tangent = 5,
    Binormal = 6,
    SkinWeights = 7,
    SkinJIndices = 8,
};

enum Format
{
    Float2,
    Float3,
    Float4,
    Short2,
    Byte4N,
    UByte4,
    UByte4N,
};

class VertexComponent
{
    public:
        VertexComponent(VertexType vt, Format f, int size, int isN);
        VertexComponent();
        ~VertexComponent();
        VertexType vt;
        Format f;
        int sizeInBytes;
        int isNormalized;
    private:
};