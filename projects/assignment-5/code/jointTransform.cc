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

Quaternion JointTransform::interpolate(Quaternion v0, Quaternion v1, double t)
{
    v0.normalize();
    v1.normalize();

    // Compute the cosine of the angle between the two vectors.
    double dot = Quaternion::dot_product(v1, v0);

    // If the dot product is negative, slerp won't take
    // the shorter path. Note that v1 and -v1 are equivalent when
    // the negation is applied to all four components. Fix by 
    // reversing one quaternion.
    if (dot < 0.0f) {
        v1 = Quaternion(-v1.getX(), -v1.getY(), -v1.getZ(), -v1.getW());
        dot = -dot;
    }

    const double DOT_THRESHOLD = 0.9995;
    if (dot > DOT_THRESHOLD) {
        // If the inputs are too close for comfort, linearly interpolate
        // and normalize the result.

        Quaternion result = v0 + (v1 - v0)*t;
        result.normalize();
        return result;
    }

    // Since dot is in range [0, DOT_THRESHOLD], acos is safe
    double theta_0 = acos(dot);        // theta_0 = angle between input vectors
    double theta = theta_0*t;          // theta = angle between v0 and result
    double sin_theta = sin(theta);     // compute this value only once
    double sin_theta_0 = sin(theta_0); // compute this value only once

    double s0 = cos(theta) - dot * sin_theta / sin_theta_0;  // == sin(theta_0 - theta) / sin(theta_0)
    double s1 = sin_theta / sin_theta_0;
    Quaternion temp = ((v0 *s0) + (v1 * s1));
    temp.normalize();
    return temp;
}

Matrix4D JointTransform::getCombinedMatrix()
{
    return transform*rotation.createMatrix();
}