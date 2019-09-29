#include "jointTransform.h"

JointTransform::JointTransform()
{
    transform = Matrix4D();
    rotation = Quaternion();
}

JointTransform::JointTransform(Matrix4D mat, Quaternion rot)
{
    transform = mat;
    rotation = rot;
}

JointTransform::~JointTransform()
{

}

void JointTransform::setRotation(Quaternion rot)
{
    rotation = rot;
}

Quaternion JointTransform::getRotation()
{
    return rotation;
}

void JointTransform::setTransform(Matrix4D mat)
{
    transform = mat;
}

Matrix4D JointTransform::getTransform()
{
    return transform;
}