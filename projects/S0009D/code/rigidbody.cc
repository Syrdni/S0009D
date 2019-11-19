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

    float h2 = (aabb.maxPoint[1] - aabb.minPoint[1])*(aabb.maxPoint[1] - aabb.minPoint[1]);
    float d2 = (aabb.maxPoint[2] - aabb.minPoint[2])*(aabb.maxPoint[2] - aabb.minPoint[2]);
    float w2 = (aabb.maxPoint[0] - aabb.minPoint[0])*(aabb.maxPoint[0] - aabb.minPoint[0]);

    inertiaTensor = Matrix4D((1.0/12)*mass*(h2+d2), 0, 0, 0,
                             0, (1.0/12)*mass*(w2+d2), 0, 0,
                             0, 0, (1.0/12)*mass*(w2+h2), 0,
                             0, 0, 0, 1);
    inverseInertiaTensor = Matrix4D::inverse(inertiaTensor);
    position = pos;
    rotation = rot;
}

Rigidbody::~Rigidbody(){}

void Rigidbody::update()
{
    //Position
    velocity = momentum * (1/mass);
    position = position + velocity;
    position[3] = 1;   

    rotation = rotation * Drot;
}

void Rigidbody::applyForce(Vector4D pos, Vector4D forceDirection)
{
    momentum = momentum + forceDirection;
    momentum[3] = 1;
    torque = (pos - (aabb.getCenter() + position)).crossProduct(forceDirection);
    angularMomentum = angularMomentum + torque;
    inverseInertiaTensor = rotation*Matrix4D::inverse(inertiaTensor)*Matrix4D::transpose(rotation);

    spin = inverseInertiaTensor * angularMomentum;
    //Vector4D X, Y, Z;
    //X = Vector4D(rotation[0], rotation[1], rotation[2], 1).crossProduct(spin);
    //Y = Vector4D(rotation[4], rotation[5], rotation[6], 1).crossProduct(spin);
    //Z = Vector4D(rotation[8], rotation[9], rotation[10], 1).crossProduct(spin);
    //Drot = Matrix4D(X[0], Y[0], Z[0], 0,
    //                X[1], Y[1], Z[1], 0,
    //                X[2], Y[2], Z[2], 0,
    //                0, 0, 0, 1);
    Matrix4D spinMatrix = Matrix4D(1, -spin[2], spin[1], 0,
                                   spin[2], 1, -spin[0], 0,
                                   -spin[1], spin[0], 1, 0,
                                   0, 0, 0, 1);
    Drot = spinMatrix * rotation;
    int i = 0;

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