#include "rigidbody.h"
#include "debug/debugManager.h"

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
    inverseInertiaTensor = Matrix4D::inverse(inertiaTensorBody);
    position = pos;
    q = Quaternion::createQuaternion(rot);
}

Rigidbody::~Rigidbody(){}

void Rigidbody::update()
{
    //Position
    momentum = momentum + forceToAdd;
    momentum[3] = 0;

    velocity = momentum * (1/mass);
    position = position + velocity * 0.1;
    position[3] = 1;   

    this->spin = this->spin + inverseInertiaTensor * angularMomentum;
    this->spin[3] = 0;
    
    Quaternion spins = Quaternion(spin[0], spin[1], spin[2], spin[3]);
    Quaternion Qrot = (q * spins) * 0.5;

    q = q + Qrot;
    q.normalize();
    rotation = q.createMatrix();

    inverseInertiaTensor = rotation * (Matrix4D::inverse(inertiaTensorBody) * Matrix4D::transpose(rotation));

   
    worldTransform = rotation * Matrix4D::getPositionMatrix(-aabb.getCenter());
    worldTransform = Matrix4D::getPositionMatrix(position + aabb.getCenter()) * worldTransform;
    
    if(forceToAdd.length() > 0){
        this->lastPosOfForce = Matrix4D::inverse(worldTransform) * posOfForce;
        this->lastForceToAdd = posOfForce + (Matrix4D::inverse(rotation) * forceToAdd * 100.0f);
    }

    auto cunt = worldTransform * this->lastPosOfForce;
    auto cunt2 = worldTransform * this->lastForceToAdd;

    DebugManager::getInstance()->createSingleFrameCube(cunt, 0.1f, 0.1f, 0.1f, {0,1,0,1});
    DebugManager::getInstance()->createSingleFrameLine(cunt, cunt2, {0,1,1,1});

    DebugManager::getInstance()->createSingleFrameLine({0,0,0,1}, Vector4D(spin[0], spin[1], spin[2], 1) * 100000.0f, {1,1,1,1});

    posOfForce = Vector4D(0, 0, 0, 1);
    forceToAdd = Vector4D(0, 0, 0, 1);
    angularMomentum = {0,0,0,0};

    ImGui::InputFloat4("Position", position.getVector());
    ImGui::InputFloat4("momentum", momentum.getVector());
    ImGui::InputFloat4("torque", torque.getVector());
    ImGui::InputFloat4("angularMomentum", angularMomentum.getReference());
    ImGui::InputFloat4("spin", spin.getReference());
    ImGui::InputFloat4("Center", aabb.getCenter().getVector());

}

void Rigidbody::applyForce(Vector4D pos, Vector4D forceDirection)
{
    posOfForce =  pos;
    forceToAdd =  forceDirection;

    torque = (posOfForce - (position + aabb.getCenter())).crossProduct(forceToAdd);
    DebugManager::getInstance()->createSingleFrameCube((position + aabb.getCenter()), 0.2, 0.2, 0.2, {1, 0, 0, 1});
    torque[3] = 0;

    angularMomentum = angularMomentum + torque;
    angularMomentum[3] = 0;
}

Matrix4D Rigidbody::getRotation()
{
    return rotation;
}

Vector4D Rigidbody::getPosition()
{
    return position;
}

Vector4D Rigidbody::getCenterPoint()
{
    return aabb.getCenter();
}