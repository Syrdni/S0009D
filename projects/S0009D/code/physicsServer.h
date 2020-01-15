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

struct CollisionStruct
{
    Rigidbody RA, RB;
    Vector4D PosA, PosB;
    Vector4D VelA, VelB;
    Quaternion RotationA, RotationB;
    Vector4D SpinA, SpinB;
    Vector4D colPointA, colPointB;
    Matrix4D worldTransformA, worldTransformB;
    bool unmovabaleA, unmovabaleB;

    CollisionStruct operator/(const float& other){
        CollisionStruct cs;
        cs.PosA = PosA * (1/other);
        cs.PosB = PosB * (1/other);
        cs.VelA = VelA * (1/other);
        cs.VelB = VelB * (1/other);
        cs.SpinA = SpinA;
        cs.SpinB = SpinB;
        cs.RotationA = RotationA * (1/other);
        cs.RotationB = RotationB * (1/other);
        cs.colPointA = colPointA * (1/other);
        cs.colPointB = colPointB * (1/other);
        cs.worldTransformA = Matrix4D::getPositionMatrix(cs.PosA) * cs.RotationA.createMatrix() * cs.RA.scale;
        cs.worldTransformB = Matrix4D::getPositionMatrix(cs.PosB) * cs.RotationB.createMatrix() * cs.RB.scale;
        cs.PosA[3] = 1;
        cs.PosB[3] = 1;
        cs.VelA[3] = 0;
        cs.VelB[3] = 0;
        return cs;
    }

    CollisionStruct operator*(const float& other){
        CollisionStruct cs;
        cs.PosA = PosA * other;
        cs.PosB = PosB * other;
        cs.VelA = VelA * other;
        cs.VelB = VelB * other;
        cs.SpinA = SpinA;
        cs.SpinB = SpinB;
        cs.RotationA = RotationA * other;
        cs.RotationB = RotationB * other;
        cs.colPointA = colPointA * other;
        cs.colPointB = colPointB * other;
        cs.worldTransformA = Matrix4D::getPositionMatrix(cs.PosA) * cs.RotationA.createMatrix() * cs.RA.scale;
        cs.worldTransformB = Matrix4D::getPositionMatrix(cs.PosB) * cs.RotationB.createMatrix() * cs.RB.scale;
        cs.PosA[3] = 1;
        cs.PosB[3] = 1;
        cs.VelA[3] = 0;
        cs.VelB[3] = 0;
        return cs;
    }

        CollisionStruct operator+(const CollisionStruct& other){
        CollisionStruct cs;
        cs.PosA = PosA + other.PosA;
        cs.PosB = PosB + other.PosB;
        cs.VelA = VelA + other.VelA;
        cs.VelB = VelB + other.VelB;
        cs.SpinA = other.SpinA;
        cs.SpinB = other.SpinB;
        cs.RotationA = RotationA + other.RotationA;
        cs.RotationB = RotationB + other.RotationB;
        cs.colPointA = colPointA + other.colPointA;
        cs.colPointB = colPointB + other.colPointB;
        cs.worldTransformA = Matrix4D::getPositionMatrix(cs.PosA) * cs.RotationA.createMatrix() * cs.RA.scale;
        cs.worldTransformB = Matrix4D::getPositionMatrix(cs.PosB) * cs.RotationB.createMatrix() * cs.RB.scale;
        cs.PosA[3] = 1;
        cs.PosB[3] = 1;
        cs.VelA[3] = 0;
        cs.VelB[3] = 0;
        return cs;
    }

    CollisionStruct operator-(const CollisionStruct& other){
        CollisionStruct cs;
        cs.PosA = PosA - other.PosA;
        cs.PosB = PosB - other.PosB;
        cs.VelA = VelA - other.VelA;
        cs.VelB = VelB - other.VelB;
        cs.SpinA = other.SpinA;
        cs.SpinB = other.SpinB;
        cs.RotationA = RotationA - other.RotationA;
        cs.RotationB = RotationB - other.RotationB;
        cs.colPointA = colPointA - other.colPointA;
        cs.colPointB = colPointB - other.colPointB;
        cs.worldTransformA = Matrix4D::getPositionMatrix(cs.PosA) * cs.RotationA.createMatrix() * cs.RA.scale;
        cs.worldTransformB = Matrix4D::getPositionMatrix(cs.PosB) * cs.RotationB.createMatrix() * cs.RB.scale;
        cs.PosA[3] = 1;
        cs.PosB[3] = 1;
        cs.VelA[3] = 0;
        cs.VelB[3] = 0;
        return cs;
    }
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

        std::chrono::time_point<std::chrono::steady_clock> start;
        std::chrono::time_point<std::chrono::steady_clock> end;

        std::vector<Object*> objectVector;
        std::vector<pointAndOwner> x_axisPoints;
        SupportPoint support(objectPair op, Vector4D d);
        Vector4D sum(float a[3], float b[3]);
        Vector4D DoSimplexLine(std::vector<SupportPoint>& points);
        Vector4D DoSimplexTriangle(std::vector<SupportPoint>& points);
        Vector4D DoSimplexTetrahedron(std::vector<SupportPoint>& points, bool& oof);
        void getBarycentric(Vector4D point, Vector4D vec1, Vector4D vec2, Vector4D vec3, float& p1, float& p2, float& p3);
        void response(EPAResult& results, objectPair op);
        CollisionStruct Euler(CollisionStruct cs, float depth, Vector4D colpoint1, Vector4D colpoint2, float epsilon);
        CollisionStruct MidPoint(CollisionStruct cs, float depth, Vector4D colpoint1, Vector4D colpoint2, float epsilon);
        CollisionStruct Runge_Kutta(CollisionStruct cs, float depth, Vector4D colpoint1, Vector4D colpoint2, float epsilon);
};