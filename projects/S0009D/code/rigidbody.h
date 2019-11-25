#pragma once
#include "Vector4D.h"
#include "Ray.h"
#include "imgui.h"
#include "animations/quaternion.h"

class Rigidbody
{
    public:
        Rigidbody();
        Rigidbody(AABB aabb, float m, Matrix4D& rot, Vector4D& pos);
        ~Rigidbody();
        void applyForce(Vector4D pos, Vector4D forceDirection);
        void update();
        Matrix4D getRotation();
        Vector4D getPosition();
        Vector4D getCenterPoint();
    
        Matrix4D worldTransform;
    private:
        AABB aabb;
        float mass;

        Matrix4D rotation;
        Vector4D position;

        Matrix4D Drot = Matrix4D(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);

        Matrix4D inertiaTensorBody;
        Matrix4D inverseInertiaTensor;

        Vector4D velocity;
        Vector4D momentum;
        Vector4D dMomentum;

        Vector4D torque;
        Vector4D angularMomentum;
        Vector4D spin;
        Matrix4D spinMatrix;

        Vector4D forceToAdd;
        Vector4D posOfForce;

        Vector4D lastPosOfForce;
        Vector4D lastForceToAdd;
        
        

        Quaternion q;
        Quaternion QRot = Quaternion(0, 0, 0, 1);
};