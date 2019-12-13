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

struct SupportPoint
{
    Vector4D point, sup_a, sup_b;
    SupportPoint(Vector4D p, Vector4D sa, Vector4D sb) : point(p), sup_a(sa), sup_b(sb) {};
};

struct Triangle
{
    Vector4D face[4];
    Triangle(Vector4D ai, Vector4D bi, Vector4D ci, Vector4D n) : face({ai, bi, ci ,n}) {};
};

struct EPAResult
{
    float distance;
    Vector4D normal, PosOfObject1, PosOfObject2;
    EPAResult(float d, Vector4D n, Vector4D Pos1, Vector4D Pos2) : distance(d) , normal(n), PosOfObject1(Pos1), PosOfObject2(Pos2) {};
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
        bool GJK(objectPair op, EPAResult &result);
        EPAResult EPA(std::vector<SupportPoint> points, objectPair op);
        ClosestResult findClosestTriangle(std::vector<std::vector<Vector4D>> &edges);
        void extendPolytope(std::vector<std::vector<Vector4D>> &edges, Vector4D p);

        std::vector<Object*> objectVector;
        std::vector<pointAndOwner> x_axisPoints;
        SupportPoint support(objectPair op, Vector4D d);
        Vector4D sum(float a[3], float b[3]);
        Vector4D DoSimplexLine(std::vector<SupportPoint>& points);
        Vector4D DoSimplexTriangle(std::vector<SupportPoint>& points);
        Vector4D DoSimplexTetrahedron(std::vector<SupportPoint>& points, bool& oof);
        void getBarycentric(Vector4D point, Vector4D vec1, Vector4D vec2, Vector4D vec3, float& p1, float& p2, float& p3);
        void response(EPAResult results, objectPair op);
        void Euler();
        void MidPoint();
        void Runge_Kutta();
};