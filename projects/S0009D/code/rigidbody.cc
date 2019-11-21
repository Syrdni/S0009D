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

    inertiaTensorBody = Matrix4D((mass/12.0)*(h2+d2), 0, 0, 0,
                                  0, (mass/12.0)*(w2+d2), 0, 0,
                                  0, 0, (mass/12.0)*(w2+h2), 0,
                                  0, 0, 0, 1);
    //inertiaTensorBody = Matrix4D();
    inverseInertiaTensor = Matrix4D::inverse(inertiaTensorBody);
    position = pos;
    q = Quaternion::createQuaternion(rot);;
}

Rigidbody::~Rigidbody(){}

void Rigidbody::update()
{
    //Position
    momentum = momentum + forceToAdd;
    momentum[3] = 0;

    velocity = momentum * (1/mass);
    position = position + velocity;
    position[3] = 1;   



    torque = (posOfForce - aabb.getCenter()).crossProduct(forceToAdd);
    torque[3] = 1;

    angularMomentum = angularMomentum + torque;
    angularMomentum[3] = 1;

    q.normalize();
    rotation = q.createMatrix();

    inverseInertiaTensor = rotation*Matrix4D::inverse(inertiaTensorBody)*Matrix4D::transpose(rotation);
    spin = inverseInertiaTensor * angularMomentum;
    spin[3] = 1;
    Quaternion Qrot = Quaternion(spin[0], spin[1], spin[2], spin[3]) * q * 0.5;
    //Matrix4D spinMatrix = Matrix4D(1, -spin[2], spin[1], 0,
    //                               spin[2], 1, -spin[0], 0,
    //                               -spin[1], spin[0], 1, 0,
    //                               0, 0, 0, 1);
    //Drot = spinMatrix * rotation;
    Qrot.normalize();
    q = Qrot;
    rotation = q.createMatrix();

    posOfForce = Vector4D(0, 0, 0, 1);
    forceToAdd = Vector4D(0, 0, 0, 1);

    ImGui::InputFloat4("Position", position.getVector());
    ImGui::InputFloat4("momentum", momentum.getVector());
    ImGui::InputFloat4("torque", torque.getVector());
    ImGui::InputFloat4("angularMomentum", angularMomentum.getVector());
    ImGui::InputFloat4("spin", spin.getVector());
    ImGui::InputFloat4("Center", aabb.getCenter().getVector());
    //ImGui::InputFloat4("QROT", QRot.getVector().getVector());
    //ImGui::InputFloat4("QDOT", QDot.getVector().getVector());
}

void Rigidbody::applyForce(Vector4D pos, Vector4D forceDirection)
{
    posOfForce =  pos;
    forceToAdd =  forceDirection;
    int i = 0;
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