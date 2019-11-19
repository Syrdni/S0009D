#include "rigidbody.h"

Rigidbody::Rigidbody(){};

Rigidbody::Rigidbody(AABB aabb, float m, Matrix4D& rot, Vector4D& pos)
{
    this->aabb = aabb;
    mass = m;
    //float side = aabb.maxPoint[2] - aabb.minPoint[2];
    aabb.maxPoint = Matrix4D::inverse(Matrix4D::getPositionMatrix(pos)) * aabb.maxPoint;
    Vector4D center = aabb.maxPoint * 0.5;
    //inertiaTensor = Matrix4D()*(m*((side*side)/6));
    inertiaTensor = Matrix4D(center[0], 0, 0, 0,
                             0, center[1], 0, 0,
                             0, 0, center[2], 0,
                             0, 0, 0, 1);
    inverseInertiaTensor = Matrix4D::inverse(inertiaTensor);
    rotation = rot;
    position = pos;
}

Rigidbody::~Rigidbody(){}

void Rigidbody::update()
{
    momentum = momentum + forceToAdd;
    momentum[3] = 1;
    forceToAdd = Vector4D(0, 0, 0, 1);
    Vector4D velocity = momentum * (1/mass);
    position = position + velocity;
    //rotation = rotation + Drot;
}

void Rigidbody::applyForce(Vector4D pos, Vector4D forceDirection)
{
    forceToAdd = forceDirection;
    posOfForce = pos;
    /*inverseInertiaTensor = rotation * inverseInertiaTensor * Matrix4D::transpose(rotation);
    forceDirection[3] = 1;
    torque = (pos - aabb.getCenter()).crossProduct(forceDirection);
    angularMomentum = angularMomentum + torque;
    angularMomentum[3] = 1;
    spin = inverseInertiaTensor * angularMomentum;
    spin[3] = 1;
    Vector4D X, Y, Z;
    X = Vector4D(rotation[0], rotation[1], rotation[2], 1).crossProduct(spin);
    Y = Vector4D(rotation[4], rotation[5], rotation[6], 1).crossProduct(spin);
    Z = Vector4D(rotation[8], rotation[9], rotation[10], 1).crossProduct(spin);
    Drot = Matrix4D(X[0], X[1], X[2], 0,
                    Y[0], Y[1], Y[2], 0,
                    Z[0], Z[1], Z[2], 0,
                    0, 0, 0, 1);
    rotation = rotation * Drot;
    int a = 0;*/
}

void Rigidbody::calcRotation(Vector4D pos)
{
    //Vector4D spinVector = Vector4D(0, 1, 0 ,1);
    //Vector4D rt = pos - centerOfMass;
    //Vector4D R = spinVector.crossProduct(rt);
    //Vector4D X, Y, Z;
    //X = spinVector.crossProduct(Vector4D(rotation[0], rotation[1], rotation[2], 1));
    //Y = spinVector.crossProduct(Vector4D(rotation[4], rotation[5], rotation[6], 1));
    //Z = spinVector.crossProduct(Vector4D(rotation[8], rotation[9], rotation[10], 1));
    //Drot = Matrix4D(X[0], X[1], X[2], 0,
    //                Y[0], Y[1], Y[2], 0,
    //                Z[0], Z[1], Z[2], 0,
    //                0, 0, 0, 1);
}

Matrix4D Rigidbody::getRotation()
{
    return rotation;
}

Vector4D Rigidbody::getPosition()
{
    return position;
}