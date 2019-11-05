#include "vertexComponent.h"

VertexComponent::VertexComponent(VertexType vt, Format f, int size, unsigned int isN, int sizeA)
{
    this->vt = vt;
    this->f = f;
    this->sizeInBytes = size;
    this->isNormalized = isN;
    this->sizeArray = sizeA;
}

VertexComponent::VertexComponent()
{

}

VertexComponent::~VertexComponent()
{

}

unsigned int VertexComponent::getType()
{
    if (this->f == Format::Float3 ||this->f == Format::Float2)
        return 0x1406;
    else if (this->f == Format::Byte4N)
        return 0x1400;
    else if (this->f == Format::UByte4 || this->f == Format::UByte4N)
        return 0x1401; 
}