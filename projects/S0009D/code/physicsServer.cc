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
    std::vector<SupportPoint> points;
    Vector4D start = Vector4D(0, -1, 0, 1);
    SupportPoint S = support(op, start);

    if (S.point.dotProduct(start) < 0)
        return;
    

    points.push_back(S);
    Vector4D D = -S.point;

    bool oof = false;
    int iterationDepth = 20;
    for (int i = 0; i < iterationDepth; i++)
    {
        SupportPoint A = support(op, D);
        if (A.point.dotProduct(D) < 0)
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
                EPAResult r = EPA(points, op);
                Vector4D l1 = op.object1->indexOfFurthestPoint(r.normal);
                Vector4D l2 = op.object2->indexOfFurthestPoint(-r.normal);
                //DebugManager::getInstance()->createSingleFrameCube(l1 - (r.normal * r.distance), 0.2, 0.2, 0.2, Vector4D(1, 1, 0, 1));
                //DebugManager::getInstance()->createSingleFrameCube(l2 - (r.normal * r.distance), 0.2, 0.2, 0.2, Vector4D(0, 1, 1, 1));
                break;
            }
        }  
    }
    
    DebugManager::getInstance()->createSingleFrameCube(Vector4D(0, 0, 0, 1), 0.1, 0.1, 0.1, Vector4D(1, 0, 1,1));
    DebugManager::getInstance()->createSingleFrameCube(points[0].point, 0.2, 0.2, 0.2, Vector4D(1, 0, 0, 1));
    DebugManager::getInstance()->createSingleFrameCube(points[1].point, 0.2, 0.2, 0.2, Vector4D(0, 1, 0, 1));
    DebugManager::getInstance()->createSingleFrameCube(points[2].point, 0.2, 0.2, 0.2, Vector4D(0, 0, 1, 1));
    DebugManager::getInstance()->createSingleFrameCube(points[3].point, 0.2, 0.2, 0.2, Vector4D(1, 1, 1, 1));
    DebugManager::getInstance()->createSingleFrameLine(points[0].point, points[1].point, Vector4D(0, 1, 0, 1));
    DebugManager::getInstance()->createSingleFrameLine(points[0].point, points[2].point, Vector4D(0, 1, 0, 1));
    DebugManager::getInstance()->createSingleFrameLine(points[0].point, points[3].point, Vector4D(0, 1, 0, 1));
    DebugManager::getInstance()->createSingleFrameLine(points[1].point, points[2].point, Vector4D(0, 1, 0, 1));
    DebugManager::getInstance()->createSingleFrameLine(points[1].point, points[3].point, Vector4D(0, 1, 0, 1));
    DebugManager::getInstance()->createSingleFrameLine(points[2].point, points[3].point, Vector4D(0, 1, 0, 1));

    int t = 0;
}

EPAResult PhysicsServer::EPA(std::vector<SupportPoint> points, objectPair op)
{
    //points is the simplex from GJK
    std::vector<std::vector<Vector4D>> faces;
    faces.push_back({points[0].point, points[1].point, points[2].point, (points[1].point - points[0].point).crossProduct(points[2].point - points[0].point).normalize()});
    faces.push_back({points[0].point, points[2].point, points[3].point, (points[2].point - points[0].point).crossProduct(points[3].point - points[0].point).normalize()});
    faces.push_back({points[0].point, points[3].point, points[1].point, (points[3].point - points[0].point).crossProduct(points[1].point - points[0].point).normalize()});
    faces.push_back({points[1].point, points[3].point, points[2].point, (points[3].point - points[1].point).crossProduct(points[2].point - points[1].point).normalize()});
    int i = 0;
    while (true)
    {
        i++;
        ClosestResult e = findClosestTriangle(faces);
        SupportPoint p = support(op, e.normal);
        points.push_back(p);
        float d = p.point.dotProduct(e.normal);
        if (d - e.distance < 0.0001)
        {
            std::vector<SupportPoint> sp;
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < points.size(); j++)
                {
                    if (faces[e.index][i] == points[j].point)
                    {
                        sp.push_back(points[j]);
                        break;
                    }
                }
            }

            float u, v, w;
            float x, y, z;
            getBarycentric(e.normal, faces[e.index][0], faces[e.index][1], faces[e.index][2], u, v, w);

            Vector4D colpointA = (sp[0].sup_a * u) + (sp[1].sup_a * v) + (sp[2].sup_a * w);
            colpointA[3] = 1;
            Vector4D colpointB = (sp[0].sup_b * u) + (sp[1].sup_b * v) + (sp[2].sup_b * w);
            colpointB[3] = 1;

            //DebugManager::getInstance()->createSingleFrameCube(e.normal * e.distance, 0.2, 0.2, 0.2, Vector4D(1, 1, 0, 1));
            DebugManager::getInstance()->createSingleFrameCube(colpointA, 0.2, 0.2, 0.2, Vector4D(1, 1, 0, 1));
            DebugManager::getInstance()->createSingleFrameCube(colpointB, 0.2, 0.2, 0.2, Vector4D(0, 1, 1, 1));

            //DebugManager::getInstance()->createSingleFrameCube(sp[0].sup_a, 0.2, 0.2, 0.2, Vector4D(1, 0, 0, 1));
            //DebugManager::getInstance()->createSingleFrameCube(sp[1].sup_a, 0.2, 0.2, 0.2, Vector4D(1, 0, 0, 1));
            //DebugManager::getInstance()->createSingleFrameCube(sp[2].sup_a, 0.2, 0.2, 0.2, Vector4D(1, 0, 0, 1));
            //DebugManager::getInstance()->createSingleFrameCube(sp[0].sup_b, 0.2, 0.2, 0.2, Vector4D(1, 0, 0, 1));
            //DebugManager::getInstance()->createSingleFrameCube(sp[1].sup_b, 0.2, 0.2, 0.2, Vector4D(1, 0, 0, 1));
            //DebugManager::getInstance()->createSingleFrameCube(sp[2].sup_b, 0.2, 0.2, 0.2, Vector4D(1, 0, 0, 1));



            return EPAResult(d, e.normal);
        }
        else
        {
            //rebuild the tetrahedron
            //points.insert(points.begin() + e.index, p);
            extendPolytope(faces, p.point);
        }
    }
}

ClosestResult PhysicsServer::findClosestTriangle(std::vector<std::vector<Vector4D>> &faces)
{
    //faces = {faces[0], faces[1], faces[2], faces[3]};
    ClosestResult closest = ClosestResult();
    closest.distance = FLT_MAX;
    closest.distance = faces[0][0].dotProduct(faces[0][3]);
    closest.index = 0;
    closest.normal = faces[0][3];
    
    for (int i = 1; i < faces.size(); i++)
    {
        float distance = faces[i][0].dotProduct(faces[i][3]);
        if (distance < closest.distance)
        {
            closest.distance = distance;
            closest.index = i;
            closest.normal = faces[i][3];
        }        
    }

    return closest;
}

void PhysicsServer::extendPolytope(std::vector<std::vector<Vector4D>> &faces, Vector4D p)
{
    std::vector<Edge> loose_edges;
    for (int i = 0; i < faces.size(); i++)
    {
        if (faces[i][3].dotProduct(p-faces[i][0]) > 0)
        {
            //std::vector<Vector4D> face = {faces[i][0], faces[i][1], faces[i][2]};
            //faces.erase(faces.begin() + i);

            for (int j = 0; j < 3; j++)
            {

                std::vector<Vector4D> current_edge = {faces[i][j], faces[i][(j+1)%3]};
                bool found_edge = false;

                for (int k = 0; k < loose_edges.size(); k++)
                {
                    if (current_edge[0] == loose_edges[k].point2 &&
                        current_edge[1] == loose_edges[k].point1)
                    {
                        loose_edges.erase(loose_edges.begin() + k);
                        found_edge = true;
                        k = loose_edges.size();
                    }
                }

                if (!found_edge)
                {
                    Edge e;
                    e.point1 = current_edge[0];
                    e.point2 = current_edge[1];
                    loose_edges.push_back(e);
                }
            }
            faces.erase(faces.begin() + i);
            i--;
        }
    }

    for (int i = 0; i < loose_edges.size(); i++)
    {
        faces.push_back({loose_edges[i].point1, loose_edges[i].point2, p, (loose_edges[i].point1 - loose_edges[i].point2).crossProduct(loose_edges[i].point1 - p).normalize()});

        if (faces[faces.size()-1][0].dotProduct(faces[faces.size()-1][3]) + 0.00001 < 0)
        {
            Vector4D temp = faces[faces.size()-1][0];
            faces[faces.size()-1][0] = faces[faces.size()-1][1];
            faces[faces.size()-1][1] = temp;
            faces[faces.size()-1][3] = -faces[faces.size()-1][3];
        }
    }
}

SupportPoint PhysicsServer::support(objectPair op, Vector4D d)
{   
    Vector4D sup_a = op.object1->indexOfFurthestPoint(d);
    Vector4D sup_b = op.object2->indexOfFurthestPoint(-d);
    Vector4D ret = (sup_a - sup_b);
    ret[3] = 1;
    return SupportPoint(ret, sup_a, sup_b);
}

Vector4D PhysicsServer::sum(float a[3], float b[3])
{
    return Vector4D(a[0] - b[0], a[1] - b[1], a[2] - b[2], 1);
}

Vector4D PhysicsServer::DoSimplexLine(std::vector<SupportPoint>& points)
{
    Vector4D A0 = -points[0].point;
    Vector4D AB = points[1].point - points[0].point;
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

Vector4D PhysicsServer::DoSimplexTriangle(std::vector<SupportPoint>& points)
{
    Vector4D A0 = -points[0].point;
    Vector4D AB = points[1].point - points[0].point; //were 2-0
    Vector4D AC = points[2].point - points[0].point; //were 1-0
    Vector4D ABC = (AB).crossProduct(AC);

    Vector4D n = (AB).crossProduct(AC);

    if (AB.crossProduct(n).dotProduct(A0) > 0)
    {
        points = {points[1], points[0]};
        return AB.crossProduct(A0).crossProduct(AB);
    }

    if (n.crossProduct(AC).dotProduct(A0) > 0)
    {
        points = {points[0], points[2]};
        return AC.crossProduct(A0).crossProduct(AC);
    }

    if (n.dotProduct(A0)>0)
    {
        return n;
    }

    points = {points[0], points[2], points[1]};
    return -n;
}

Vector4D PhysicsServer::DoSimplexTetrahedron(std::vector<SupportPoint>& points, bool& oof)
{
    Vector4D ABC = (points[1].point - points[0].point).crossProduct((points[2].point - points[0].point));
    Vector4D ACD = (points[2].point - points[0].point).crossProduct((points[3].point - points[0].point));
    Vector4D ADB = (points[3].point - points[0].point).crossProduct((points[1].point - points[0].point));
    Vector4D A0 = -points[0].point;

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

void PhysicsServer::getBarycentric(Vector4D point, Vector4D vec1, Vector4D vec2, Vector4D vec3, float& p1, float& p2, float& p3)
{
	Vector4D v0 = vec2 - vec1, v1 = vec3 - vec1, v2 = point - vec1;
     float d00 = v0.dotProduct(v0);
     float d01 = v0.dotProduct(v1);
     float d11 = v1.dotProduct(v1);
     float d20 = v2.dotProduct(v0);
     float d21 = v2.dotProduct(v1);
     float denom = d00 * d11 - d01 * d01;
     p2 = (d11 * d20 - d01 * d21) / denom;
     p3 = (d00 * d21 - d01 * d20) / denom;
     p1 = 1.0f - p2 - p3;
}