#pragma once
#include "Vector4D.h"
#include "Ray.h"

class Rigidbody
{
    public:
        Rigidbody();
        Rigidbody(AABB aabb, float m, Matrix4D& rot, Vector4D& pos);
        ~Rigidbody();
        void applyForce(Vector4D pos, Vector4D forceDirection);
        void calcRotation(Vector4D pos);
        void update();
        Matrix4D getRotation();
        Vector4D getPosition();
    private:
        AABB aabb;
        float mass;

        Matrix4D rotation;
        Vector4D position;

        Matrix4D Drot = Matrix4D(0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0);

        Matrix4D inertiaTensor;
        Matrix4D inverseInertiaTensor;

        Vector4D forceToAdd;
        Vector4D posOfForce;

        Vector4D momentum;
        Vector4D dMomentum;

        Vector4D torque;
        Vector4D angularMomentum;
        Vector4D spin;
};