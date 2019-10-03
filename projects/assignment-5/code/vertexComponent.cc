#include "vertexComponent.h"

VertexComponent::VertexComponent(VertexType vt, Format f, int size, int isN)
{
    this->vt = vt;
    this->f = f;
    this->sizeInBytes = size;
    this->isNormalized = isN;
}

VertexComponent::VertexComponent()
{

}

VertexComponent::~VertexComponent()
{

}