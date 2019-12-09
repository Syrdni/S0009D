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

        //Applies a force in a certain spot. The length of the force vector is the power of the force.
        void applyForce(Vector4D pos, Vector4D forceDirection);

        void update();

        //Returns the rotation of the Rigidbody
        Matrix4D getRotation();

        //Returns the position of the Rigidbody
        Vector4D& getPosition();

        //Returns the centerpoint of the Rigidbody in localspace
        Vector4D getCenterPoint();

        //Set the position
        void setPosition(Vector4D pos);

        //The worldTransform
        Matrix4D worldTransform;

        //The direction of the gravitational pull
        static Vector4D gravitationDirection;

        //The power of the gravitational pull
        static float gravidationPower;

        //Current rotation
        Matrix4D rotation;

    private:
        //The aabb taken from the object class
        AABB aabb;

        //The mass of the object
        float mass;


        //Current position
        Vector4D position;

        
        Matrix4D inertiaTensorBody;
        Matrix4D inverseInertiaTensor;

        Vector4D velocity;
        Vector4D momentum;

        Vector4D torque;
        Vector4D angularMomentum;
        Vector4D spin;

        Vector4D forceToAdd;
        Vector4D posOfForce;
        
        Quaternion q;
        Quaternion QRot = Quaternion(0, 0, 0, 1);

};