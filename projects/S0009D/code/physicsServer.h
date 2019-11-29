#pragma once
#include "object.h"
#include <algorithm>

struct objectPair
{
    Object* object1;
    Object* object2;
    objectPair(Object* rb1, Object* rb2) : object1(rb1), object2(rb2) {};
};

struct pointAndOwner
{
    Object* owner;
    float point;
    pointAndOwner(Object* o, float val) : owner(o), point(val) {};
};

class PhysicsServer
{
    public:
        PhysicsServer();
        ~PhysicsServer();
        void update();
        void addAABB(AABB& aabb);
        void addObject(Object* rb);
        void sweep();

    private:
        std::vector<Object*> objectVector;
        std::vector<pointAndOwner> x_axisPoints;
};