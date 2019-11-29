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
        if ((objectPairVector[i].object1->getReferenceToAABB().minPoint[1] < objectPairVector[i].object2->getReferenceToAABB().maxPoint[1] &&
            objectPairVector[i].object1->getReferenceToAABB().minPoint[1] > objectPairVector[i].object2->getReferenceToAABB().minPoint[1] ||
            objectPairVector[i].object1->getReferenceToAABB().maxPoint[1] < objectPairVector[i].object2->getReferenceToAABB().maxPoint[1] &&
            objectPairVector[i].object1->getReferenceToAABB().maxPoint[1] > objectPairVector[i].object2->getReferenceToAABB().minPoint[1]) ||

           (objectPairVector[i].object2->getReferenceToAABB().minPoint[1] < objectPairVector[i].object1->getReferenceToAABB().maxPoint[1] &&
            objectPairVector[i].object2->getReferenceToAABB().minPoint[1] > objectPairVector[i].object1->getReferenceToAABB().minPoint[1] ||
            objectPairVector[i].object2->getReferenceToAABB().maxPoint[1] < objectPairVector[i].object1->getReferenceToAABB().maxPoint[1] &&
            objectPairVector[i].object2->getReferenceToAABB().maxPoint[1] > objectPairVector[i].object1->getReferenceToAABB().minPoint[1]))
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
        if (objectPairVector[i].object1->getReferenceToAABB().minPoint[2] < objectPairVector[i].object2->getReferenceToAABB().maxPoint[2] &&
            objectPairVector[i].object1->getReferenceToAABB().minPoint[2] > objectPairVector[i].object2->getReferenceToAABB().minPoint[2] ||
            objectPairVector[i].object1->getReferenceToAABB().maxPoint[2] < objectPairVector[i].object2->getReferenceToAABB().maxPoint[2] &&
            objectPairVector[i].object1->getReferenceToAABB().maxPoint[2] > objectPairVector[i].object2->getReferenceToAABB().minPoint[2] ||

           (objectPairVector[i].object2->getReferenceToAABB().minPoint[2] < objectPairVector[i].object1->getReferenceToAABB().maxPoint[2] &&
            objectPairVector[i].object2->getReferenceToAABB().minPoint[2] > objectPairVector[i].object1->getReferenceToAABB().minPoint[2] ||
            objectPairVector[i].object2->getReferenceToAABB().maxPoint[2] < objectPairVector[i].object1->getReferenceToAABB().maxPoint[2] &&
            objectPairVector[i].object2->getReferenceToAABB().maxPoint[2] > objectPairVector[i].object1->getReferenceToAABB().minPoint[2]))
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
    //    GJK(objectPairVector[0]);
    }
    

    x_axisPoints.clear();
}

void PhysicsServer::GJK(objectPair op)
{
    std::vector<Vector4D> points;
    Vector4D S = support(op, Vector4D(0, 1, 0, 1));
    points.push_back(S);
    Vector4D D = -S;
    while (true)
    {
        Vector4D A = support(op, D);
        if (A.dotProduct(D) < 0)
        {
            break;
        }
        points.push_back(A);
        
    }
    

    int t = 0;
}

Vector4D PhysicsServer::support(objectPair op, Vector4D d)
{
    int i = op.object1->indexOfFurthestPoint(d);
    int j = op.object2->indexOfFurthestPoint(-d);

    return sum(op.object1->getGraphicsNode().getMeshResource()->getVertexBuffer()[i].pos, op.object1->getGraphicsNode().getMeshResource()->getVertexBuffer()[j].pos);
}

Vector4D PhysicsServer::sum(float a[3], float b[3])
{
    return Vector4D(a[0] - b[0], a[1] - b[1], a[2] - b[2], 1);
}