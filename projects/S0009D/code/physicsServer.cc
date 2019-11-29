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
            temp.erase(temp.begin + pos);
        }
        //Otherwise, create a objectPair if possible and add the object to the temp list
        else
        {
            for (int i = 0; i < temp.size(); i++)
            {
                objectPairVector.push_back(objectPair(temp[i], x_axisPoints[i].owner));
            }
            temp.push_back(x_axisPoints[i].owner);
        }
    }
    

    x_axisPoints.clear();
}