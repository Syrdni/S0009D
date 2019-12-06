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

struct ClosestResult
{
    float distance;
    Vector4D normal;
    int index;
};

struct Edge
{
    Vector4D point1, point2;
};

struct Triangle
{
    Vector4D face[4];
    Triangle(Vector4D ai, Vector4D bi, Vector4D ci, Vector4D n) : face({ai, bi, ci ,n}) {};
};

struct EPAResult
{
    float distance;
    Vector4D normal;
    EPAResult(float d, Vector4D n) : distance(d) , normal(n) {};
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
        void GJK(objectPair op);
        EPAResult EPA(std::vector<Vector4D> points, objectPair op);
        ClosestResult findClosestTriangle(std::vector<std::vector<Vector4D>> &edges);
        void extendPolytope(std::vector<std::vector<Vector4D>> &edges, Vector4D p);

        std::vector<Object*> objectVector;
        std::vector<pointAndOwner> x_axisPoints;
        Vector4D support(objectPair op, Vector4D d);
        Vector4D sum(float a[3], float b[3]);
        Vector4D DoSimplexLine(std::vector<Vector4D>& points);
        Vector4D DoSimplexTriangle(std::vector<Vector4D>& points);
        Vector4D DoSimplexTetrahedron(std::vector<Vector4D>& points, bool& oof);
};