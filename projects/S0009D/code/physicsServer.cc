#include "physicsServer.h"

bool comp(Object* o1, Object* o2)
{
    return o1->getReferenceToAABB().minPoint[0] < o2->getReferenceToAABB().minPoint[0];
}

PhysicsServer::PhysicsServer()
{

}

PhysicsServer::~PhysicsServer()
{

}

void PhysicsServer::update()
{
    sweep();
}

void PhysicsServer::addObject(Object* rb)
{
    objectVector.push_back(rb);
}

void PhysicsServer::sweep()
{
    for (int i = 0; i < objectVector.size(); i++)
    {
        x_axisPoints.push_back(pointAndOwner(objectVector[i], objectVector[i]->getReferenceToAABB().minPoint[0]));
        x_axisPoints.push_back(pointAndOwner(objectVector[i], objectVector[i]->getReferenceToAABB().maxPoint[0]));
    };

    std::sort(x_axisPoints.begin(), x_axisPoints.end(), [](pointAndOwner lhs, pointAndOwner rhs)
    {
        return lhs.point < rhs.point;
    });

    //Points that we are currently checking
    std::vector<Object*> temp;

    //All objects that have a intersecting aabb in x-Axis
    std::vector<objectPair> objectPairVector;

    for (int i = 0; i < x_axisPoints.size(); i++)
    {
        //Check if the point is in the temp list
        if (std::find(temp.begin(), temp.end(), x_axisPoints[i].owner) != temp.end())
        {
            //If it is, then we have hit the end so remove it from the vector
            ptrdiff_t pos = find(temp.begin(), temp.end(), x_axisPoints[i].owner) - temp.begin();
            temp.erase(temp.begin() + pos);
        }
        //Otherwise, create a objectPair if possible and add the object to the temp list
        else
        {
            for (int j = 0; j < temp.size(); j++)
            {
                objectPairVector.push_back(objectPair(temp[j], x_axisPoints[i].owner));
            }
            temp.push_back(x_axisPoints[i].owner);
        }
    }

    //Check the objects that intersected in the x-axis that they intersect in the other axis too
    //Y-axis
    for (int i = 0; i < objectPairVector.size(); i++)
    {
        if ((objectPairVector[i].object1->getReferenceToAABB().minPoint[1] <= objectPairVector[i].object2->getReferenceToAABB().maxPoint[1] &&
            objectPairVector[i].object1->getReferenceToAABB().minPoint[1] >= objectPairVector[i].object2->getReferenceToAABB().minPoint[1] ||
            objectPairVector[i].object1->getReferenceToAABB().maxPoint[1] <= objectPairVector[i].object2->getReferenceToAABB().maxPoint[1] &&
            objectPairVector[i].object1->getReferenceToAABB().maxPoint[1] >= objectPairVector[i].object2->getReferenceToAABB().minPoint[1]) ||

           (objectPairVector[i].object2->getReferenceToAABB().minPoint[1] <= objectPairVector[i].object1->getReferenceToAABB().maxPoint[1] &&
            objectPairVector[i].object2->getReferenceToAABB().minPoint[1] >= objectPairVector[i].object1->getReferenceToAABB().minPoint[1] ||
            objectPairVector[i].object2->getReferenceToAABB().maxPoint[1] <= objectPairVector[i].object1->getReferenceToAABB().maxPoint[1] &&
            objectPairVector[i].object2->getReferenceToAABB().maxPoint[1] >= objectPairVector[i].object1->getReferenceToAABB().minPoint[1]))
        {
            continue;
        }
        else
        {
            objectPairVector.erase(objectPairVector.begin() + i);
        }
    }

    //Z-axis
    for (int i = 0; i < objectPairVector.size(); i++)
    {
        if (objectPairVector[i].object1->getReferenceToAABB().minPoint[2] <= objectPairVector[i].object2->getReferenceToAABB().maxPoint[2] &&
            objectPairVector[i].object1->getReferenceToAABB().minPoint[2] >= objectPairVector[i].object2->getReferenceToAABB().minPoint[2] ||
            objectPairVector[i].object1->getReferenceToAABB().maxPoint[2] <= objectPairVector[i].object2->getReferenceToAABB().maxPoint[2] &&
            objectPairVector[i].object1->getReferenceToAABB().maxPoint[2] >= objectPairVector[i].object2->getReferenceToAABB().minPoint[2] ||

           (objectPairVector[i].object2->getReferenceToAABB().minPoint[2] <= objectPairVector[i].object1->getReferenceToAABB().maxPoint[2] &&
            objectPairVector[i].object2->getReferenceToAABB().minPoint[2] >= objectPairVector[i].object1->getReferenceToAABB().minPoint[2] ||
            objectPairVector[i].object2->getReferenceToAABB().maxPoint[2] <= objectPairVector[i].object1->getReferenceToAABB().maxPoint[2] &&
            objectPairVector[i].object2->getReferenceToAABB().maxPoint[2] >= objectPairVector[i].object1->getReferenceToAABB().minPoint[2]))
        {
            continue;
        }
        else
        {
            objectPairVector.erase(objectPairVector.begin() + i);
        }
    }
    
    //Set the color of the aadd to a red color
    for (int i = 0; i < objectPairVector.size(); i++)
    {
        objectPairVector[i].object1->colorOnAABB = Vector4D(1, 0, 0, 1);
        objectPairVector[i].object2->colorOnAABB = Vector4D(1, 0, 0, 1);
    }
    
    //GJK
    if (objectPairVector.size() > 0)
    {
       GJK(objectPairVector[0]);
    }
    

    x_axisPoints.clear();
}

void PhysicsServer::GJK(objectPair op)
{
    std::vector<Vector4D> points;
    Vector4D start = Vector4D(0, -1, 0, 1);
    Vector4D S = support(op, start);

    if (S.dotProduct(start) < 0)
        return;
    

    points.push_back(S);
    Vector4D D = -S;

    bool oof = false;
    int iterationDepth = 20;
    for (int i = 0; i < iterationDepth; i++)
    {
        Vector4D A = support(op, D);
        if (A.dotProduct(D) < 0)
        {
           oof = false;
           return;
        }
        points.insert(points.begin(), A);

        if (points.size() == 2)
            D = DoSimplexLine(points);
        else if (points.size() == 3)
            D = DoSimplexTriangle(points);
        else if (points.size() == 4)
        {
            D = DoSimplexTetrahedron(points, oof);
            if (oof == true)
            {
                op.object1->colorOnAABB = Vector4D(0, 1, 0, 1);
                op.object2->colorOnAABB = Vector4D(0, 1, 0, 1);
                break;
            }
        }  
    }
    
    DebugManager::getInstance()->createSingleFrameCube(Vector4D(0, 0, 0, 1), 1, 1, 1, Vector4D(1, 0, 1,1));
    DebugManager::getInstance()->createSingleFrameCube(points[0], 2, 2, 2, Vector4D(1, 0, 0, 1));
    DebugManager::getInstance()->createSingleFrameCube(points[1], 2, 2, 2, Vector4D(0, 1, 0, 1));
    DebugManager::getInstance()->createSingleFrameCube(points[2], 2, 2, 2, Vector4D(0, 0, 1, 1));
    DebugManager::getInstance()->createSingleFrameCube(points[3], 2, 2, 2, Vector4D(1, 1, 1, 1));
    DebugManager::getInstance()->createSingleFrameLine(points[0], points[1], Vector4D(0, 1, 0, 1));
    DebugManager::getInstance()->createSingleFrameLine(points[0], points[2], Vector4D(0, 1, 0, 1));
    DebugManager::getInstance()->createSingleFrameLine(points[0], points[3], Vector4D(0, 1, 0, 1));
    DebugManager::getInstance()->createSingleFrameLine(points[1], points[2], Vector4D(0, 1, 0, 1));
    DebugManager::getInstance()->createSingleFrameLine(points[1], points[3], Vector4D(0, 1, 0, 1));
    DebugManager::getInstance()->createSingleFrameLine(points[2], points[3], Vector4D(0, 1, 0, 1));

    int t = 0;
}

Vector4D PhysicsServer::support(objectPair op, Vector4D d)
{    
    Vector4D ret = (op.object1->indexOfFurthestPoint(d) - op.object2->indexOfFurthestPoint(-d));
    //DebugManager::getInstance()->createSingleFrameCube(op.object1->indexOfFurthestPoint(d), 2, 2, 2, Vector4D(1, 0, 0, 1));
    //DebugManager::getInstance()->createSingleFrameCube(op.object2->indexOfFurthestPoint(-d), 2, 2, 2, Vector4D(0, 0, 1, 1));
    ret[3] = 1;
    return ret;
}

Vector4D PhysicsServer::sum(float a[3], float b[3])
{
    return Vector4D(a[0] - b[0], a[1] - b[1], a[2] - b[2], 1);
}

Vector4D PhysicsServer::DoSimplexLine(std::vector<Vector4D>& points)
{
    Vector4D A0 = -points[0];
    Vector4D AB = points[1] - points[0];
    if (AB.dotProduct(A0) > 0.0f)
    {
        return AB.crossProduct(A0).crossProduct(AB);
    }
    else
    {
        points = {points[0]};
        return A0;
    }
}

Vector4D PhysicsServer::DoSimplexTriangle(std::vector<Vector4D>& points)
{
    Vector4D A0 = -points[0];
    Vector4D AB = points[1] - points[0]; //were 2-0
    Vector4D AC = points[2] - points[0]; //were 1-0
    Vector4D ABC = (AB).crossProduct(AC);

    if ((ABC.crossProduct(AC)).dotProduct(A0) > 0.0f)
    {
        if (AC.dotProduct(A0) > 0.0f)
        {
            points = {points[0], points[2]};
            return AC.crossProduct(A0).crossProduct(AC);
        }
        else
        {
            //star
        } 
    }

    else
    {
        if ((AB.crossProduct(ABC)).dotProduct(A0) > 0.0f)
        {
            //star
        }
        else
        {
            if (ABC.dotProduct(A0) > 0.0f)
            {
                return ABC;
            }
            else
            {
                points = {points[0], points[2], points[1]};
                return -ABC;
            }
        }
    }
    
    //'Star function'
    if (AB.dotProduct(A0) > 0.0f)
    {
        points = {points[0], points[1]};
        return AB.crossProduct(A0).crossProduct(AB);
    }
    else
    {
        points = {points[0]};
        return A0;
    }
}

Vector4D PhysicsServer::DoSimplexTetrahedron(std::vector<Vector4D>& points, bool& oof)
{
    Vector4D ABC = (points[1] - points[0]).crossProduct((points[2] - points[0]));
    Vector4D ACD = (points[2] - points[0]).crossProduct((points[3] - points[0]));
    Vector4D ADB = (points[3] - points[0]).crossProduct((points[1] - points[0]));
    Vector4D A0 = -points[0];

    if (ABC.dotProduct(A0) > 0.0f)
    {
        points = {points[0], points[1], points[2]};
        oof = false;
        return ABC;
    }

    if (ACD.dotProduct(A0) > 0.0f)
    {
        points = {points[0], points[2], points[3]};
        oof = false;
        return ACD;
    }

    if (ADB.dotProduct(A0) > 0.0f)
    {
        points = {points[0], points[3], points[1]};
        oof = false;
        return ADB;
    }

    oof = true;
    return Vector4D();
}