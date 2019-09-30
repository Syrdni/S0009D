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

JointTransform JointTransform::interpolateJointTransform(JointTransform JointA, JointTransform JointB, float progression)
{
    Matrix4D pos = interpolate(JointA.getTransform(), JointB.getTransform(), progression);
    Quaternion rot = interpolate(JointA.getRotation(), JointB.getRotation(), progression);
    return JointTransform(pos, rot);
}

Matrix4D JointTransform::interpolate(Matrix4D start, Matrix4D end, float progression)
{
    float x = start[3] + (end[3] - start[3])*progression;
    float y = start[7] + (end[7] - start[7])*progression;
    float z = start[11] + (end[11] - start[11])*progression;
    Vector4D temp = Vector4D(x, y, z, 1);
    return Matrix4D::getPositionMatrix(temp);
}

Quaternion JointTransform::interpolate(Quaternion a, Quaternion b, float blend)
{
    Quaternion result = Quaternion(0, 0, 0, 1);
    float dot = a.getW() * b.getW() + a.getX() * b.getX() + a.getY() * b.getY() + a.getZ() * b.getZ();
    float blendI = 1.0f - blend;
    if (dot < 0)
    {
        result.setW(blendI * a.getW() + blend * -b.getW());
        result.setX(blendI * a.getX() + blend * -b.getX());
        result.setY(blendI * a.getY() + blend * -b.getY());
        result.setZ(blendI * a.getZ() + blend * -b.getZ());
    }
    else
    {
        result.setW(blendI * a.getW() + blend * b.getW());
        result.setX(blendI * a.getX() + blend * b.getX());
        result.setY(blendI * a.getY() + blend * b.getY());
        result.setZ(blendI * a.getZ() + blend * b.getZ());
    }
    result.normalize();
    return result;  
}

Matrix4D JointTransform::getCombinedMatrix()
{
    return transform*rotation.createMatrix();
}