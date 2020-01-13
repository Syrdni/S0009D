#include "rigidbody.h"
#include "debug/debugManager.h"

Rigidbody::Rigidbody(){};

Rigidbody::Rigidbody(AABB aabb, float m, Matrix4D& rot, Vector4D& pos, bool unmovable)
{

    start = std::chrono::steady_clock::now();

    this->aabb = aabb;
    mass = m;

    //Calculate the inertalTensorBody from the aabb
    float h2 = (aabb.maxPoint[1] - aabb.minPoint[1])*(aabb.maxPoint[1] - aabb.minPoint[1]);
    float d2 = (aabb.maxPoint[2] - aabb.minPoint[2])*(aabb.maxPoint[2] - aabb.minPoint[2]);
    float w2 = (aabb.maxPoint[0] - aabb.minPoint[0])*(aabb.maxPoint[0] - aabb.minPoint[0]);

    inertiaTensorBody = Matrix4D((mass/12.0)*(h2+d2), 0, 0, 0,
                                  0, (mass/12.0)*(w2+d2), 0, 0,
                                  0, 0, (mass/12.0)*(w2+h2), 0,
                                  0, 0, 0, 1);
    inverseInertiaTensor = Matrix4D::inverse(inertiaTensorBody);
    position = pos;

    //Convert the rotation to a quaternion
    q = Quaternion::createQuaternion(rot);

    this->unmovable = unmovable;
    if (unmovable)
    {
        inverseInertiaTensor = Matrix4D(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        mass = 0;
    }
}

Rigidbody::~Rigidbody(){}

void Rigidbody::update()
{
    auto end = std::chrono::steady_clock::now();
    float elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0;
    this->start = end;    

    //Check if we have a gravitational pull
    if (Rigidbody::gravitationDirection.length() > 0 && !unmovable)
    {
        applyForce(Matrix4D::getPositionMatrix(position) * aabb.getCenter(), Rigidbody::gravitationDirection * Rigidbody::gravidationPower);
    }

    momentum = momentum + forceToAdd;
    momentum[3] = 0;
    

    //Calcuclate the velocity
    if (!unmovable)    
        velocity = momentum * (1/mass);

    //Calculate the position
    position = position + (velocity * elapsed); //elapsed
    position[3] = 1;   

    //Calculate the spin vector
    this->spin = this->spin + inverseInertiaTensor * (angularMomentum * elapsed); //elapsed
    this->spin[3] = 0;
    
    //Convert it to a quaternion so you can do quaternion multiplication
    Quaternion spins = Quaternion(spin[0], spin[1], spin[2], spin[3]);

    //Calculate DQ
    Quaternion DQ = (spins * q) * 0.5;

    //Calculate the new rotation
    q = q + DQ;

    //Normalize the new quaternion
    q.normalize();

    //Convert to a rotation matrix
    rotation = q.createMatrix();

    //Calculate a new inverse inertia tensor
    inverseInertiaTensor = rotation * (Matrix4D::inverse(inertiaTensorBody) * Matrix4D::transpose(rotation));

    //Calculate the world transform
    worldTransform = rotation * Matrix4D::getPositionMatrix(-aabb.getCenter());
    worldTransform = Matrix4D::getPositionMatrix(position + aabb.getCenter()) * worldTransform * scale;

    //Reset the forces and the angularMomentum
    forceToAdd = Vector4D(0, 0, 0, 1);
    angularMomentum = {0,0,0,0};

    intptr_t id = reinterpret_cast<intptr_t>(this);
    ImGui::PushID(id);
    ImGui::InputFloat4("Position", position.getReference());
    ImGui::InputFloat4("Momentum", momentum.getReference());
    ImGui::InputFloat4("Spin", spin.getReference());
    ImGui::PopID();
    ImGui::Spacing();

}

void Rigidbody::applyForce(Vector4D pos, Vector4D forceDirection)
{
    if (unmovable)
    {
        return;
    }
    

    //Add the force to the total this frame
    forceToAdd = forceToAdd + forceDirection;

    //Calculate torque
    torque = (pos - (worldTransform * aabb.getCenter())).crossProduct(forceDirection);
    torque[3] = 0;
    //DebugManager::getInstance()->createLine(position, position + forceDirection, {1,0.4,0.2,1});

    //Calculate angularMomentum
    angularMomentum = angularMomentum + inverseInertiaTensor * torque;
    angularMomentum[3] = 0;
}

Matrix4D Rigidbody::getRotation()
{
    return rotation;
}

Vector4D& Rigidbody::getPosition()
{
    return position;
}

Vector4D Rigidbody::getCenterPoint()
{
    return aabb.getCenter();
}

void Rigidbody::setPosition(Vector4D pos)
{
    position = pos;
}

Matrix4D Rigidbody::getIITW()
{
    return inverseInertiaTensor;
}

void Rigidbody::Impulse(Vector4D J, Vector4D torque)
{
    forceToAdd = J;

    torque[3] = 0;
    //angularMomentum = angularMomentum + torque;
}