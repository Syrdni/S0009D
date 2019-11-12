#include "object.h"

Object::Object(){}

Object::Object(MeshResource* mr, ShaderObject* so, TextureResource* tr, LightingNode* ln, Vector4D& cameraPos, std::string texturePath)
{
    setupGraphicsNode(mr, so, tr, ln, cameraPos, texturePath);
}

Object::~Object(){}

void Object::setupGraphicsNode(MeshResource* mr, ShaderObject* so, TextureResource* tr, LightingNode* ln, Vector4D& cameraPos, std::string texturePath)
{
    graphicsNode.setMeshResource(mr);
    graphicsNode.setShaderObject(so);
    graphicsNode.setTextureResource(tr);
    graphicsNode.setlightingNode(ln);
    graphicsNode.setCameraPosition(cameraPos);
    graphicsNode.loadTexture(texturePath.c_str());
    graphicsNode.preDrawSetup();
    setupFirstAABB(graphicsNode.getMeshResource()->getVertexBuffer());
}

void Object::setViewMatrix(Matrix4D viewmatrix)
{
    this->viewmatrix = viewmatrix;
}

void Object::setupFirstAABB(std::vector<Vertex> vertices)
{
    for (int i = 0; i < vertices.size(); i++)
    {
        if (vertices[i].pos[0] > originalAABB.maxPoint[0])
            originalAABB.maxPoint[0] = vertices[i].pos[0];

        else if (vertices[i].pos[0] < originalAABB.minPoint[0])
            originalAABB.minPoint[0] = vertices[i].pos[0];
        
        if (vertices[i].pos[1] > originalAABB.maxPoint[1])
            originalAABB.maxPoint[1] = vertices[i].pos[1];

        else if (vertices[i].pos[1] < originalAABB.minPoint[1])
            originalAABB.minPoint[1] = vertices[i].pos[1];

        if (vertices[i].pos[2] > originalAABB.maxPoint[2])
            originalAABB.maxPoint[2] = vertices[i].pos[2];

        else if (vertices[i].pos[2] < originalAABB.minPoint[2])
            originalAABB.minPoint[2] = vertices[i].pos[2];
    }

    originalAABB.minPoint[0] += position[0];
    originalAABB.minPoint[1] += position[1];
    originalAABB.minPoint[2] += position[2];

    originalAABB.maxPoint[0] += position[0];
    originalAABB.maxPoint[1] += position[1];
    originalAABB.maxPoint[2] += position[2];

    Vector4D dimentions = Vector4D(originalAABB.maxPoint[0]-originalAABB.minPoint[0], originalAABB.maxPoint[1]-originalAABB.minPoint[1], originalAABB.maxPoint[2]-originalAABB.minPoint[2], 1);
    Vector4D position = Vector4D(originalAABB.minPoint[0] + (dimentions[0]/2),
                                originalAABB.minPoint[1] + (dimentions[1]/2),
                                originalAABB.minPoint[2] + (dimentions[2]/2),
                                1);
    DebugManager::getInstance()->createSingleFrameCube(position, dimentions[0], dimentions[1], dimentions[2], Vector4D(0, 0, 1, 1), true);
    currentAABB = originalAABB;
    
}

void Object::updateAABB()
{
    std::vector<Vertex> vertices = graphicsNode.getMeshResource()->getVertexBuffer();
    currentAABB.maxPoint = Vector4D(-99999, -99999, -99999, 1);
    currentAABB.minPoint = Vector4D(99999, 99999, 99999, 1);
    std::vector<Vector4D> pointVector;

    pointVector.push_back(totalRotation * Vector4D(originalAABB.maxPoint[0], originalAABB.maxPoint[1], originalAABB.maxPoint[2], 1));
    pointVector.push_back(totalRotation * Vector4D(originalAABB.maxPoint[0], originalAABB.minPoint[1], originalAABB.maxPoint[2], 1));
    pointVector.push_back(totalRotation * Vector4D(originalAABB.minPoint[0], originalAABB.minPoint[1], originalAABB.maxPoint[2], 1));
    pointVector.push_back(totalRotation * Vector4D(originalAABB.minPoint[0], originalAABB.maxPoint[1], originalAABB.maxPoint[2], 1));

    pointVector.push_back(totalRotation * Vector4D(originalAABB.maxPoint[0], originalAABB.maxPoint[1], originalAABB.minPoint[2], 1));
    pointVector.push_back(totalRotation * Vector4D(originalAABB.maxPoint[0], originalAABB.minPoint[1], originalAABB.minPoint[2], 1));
    pointVector.push_back(totalRotation * Vector4D(originalAABB.minPoint[0], originalAABB.minPoint[1], originalAABB.minPoint[2], 1));
    pointVector.push_back(totalRotation * Vector4D(originalAABB.minPoint[0], originalAABB.maxPoint[1], originalAABB.minPoint[2], 1));

    for (int i = 0; i < pointVector.size(); i++)
    {
        if (pointVector[i][0] >= currentAABB.maxPoint[0]) currentAABB.maxPoint[0] = pointVector[i][0];
        if (pointVector[i][1] >= currentAABB.maxPoint[1]) currentAABB.maxPoint[1] = pointVector[i][1];
        if (pointVector[i][2] >= currentAABB.maxPoint[2]) currentAABB.maxPoint[2] = pointVector[i][2];
        if (pointVector[i][0] <= currentAABB.minPoint[0]) currentAABB.minPoint[0] = pointVector[i][0];
        if (pointVector[i][1] <= currentAABB.minPoint[1]) currentAABB.minPoint[1] = pointVector[i][1];
        if (pointVector[i][2] <= currentAABB.minPoint[2]) currentAABB.minPoint[2] = pointVector[i][2];
    }

    currentAABB.minPoint = Matrix4D::getPositionMatrix(position) * currentAABB.minPoint;
    currentAABB.maxPoint = Matrix4D::getPositionMatrix(position) * currentAABB.maxPoint;

    Vector4D dimentions = Vector4D(currentAABB.maxPoint[0]-currentAABB.minPoint[0], currentAABB.maxPoint[1]-currentAABB.minPoint[1], currentAABB.maxPoint[2]-currentAABB.minPoint[2], 1);
    Vector4D position = Vector4D(currentAABB.minPoint[0] + (dimentions[0]/2),
                                currentAABB.minPoint[1] + (dimentions[1]/2),
                                currentAABB.minPoint[2] + (dimentions[2]/2),
                                1);
    DebugManager::getInstance()->createSingleFrameCube(position, dimentions[0], dimentions[1], dimentions[2], Vector4D(0, 0, 1, 1), true);
}

void Object::draw()
{
    Vector4D dimentions = Vector4D(currentAABB.maxPoint[0]-currentAABB.minPoint[0], currentAABB.maxPoint[1]-currentAABB.minPoint[1], currentAABB.maxPoint[2]-currentAABB.minPoint[2], 1);
    Vector4D pos = Vector4D(currentAABB.minPoint[0] + (dimentions[0]/2),
                                currentAABB.minPoint[1] + (dimentions[1]/2),
                                currentAABB.minPoint[2] + (dimentions[2]/2),
                                1);
    DebugManager::getInstance()->createSingleFrameCube(pos, dimentions[0], dimentions[1], dimentions[2], Vector4D(0, 0, 1, 1), true);
    Matrix4D rotationX = Matrix4D::rotX(rotation[0]);
    Matrix4D rotationY = Matrix4D::rotY(rotation[1]);
    Matrix4D rotationZ = Matrix4D::rotZ(rotation[2]);
    totalRotation = rotationX * rotationY * rotationZ;
    graphicsNode.setTransform(viewmatrix * Matrix4D::getPositionMatrix(position) * totalRotation);
    graphicsNode.setPosition(Matrix4D::getPositionMatrix(position) * totalRotation);
    graphicsNode.draw();
}

void Object::update()
{
    updateAABB();
    draw();
}

AABB Object::getAABB()
{
    return currentAABB;
}

PointAndDistance Object::checkIfRayIntersects(Ray ray)
{
    std::vector<PointAndDistance> intersectionPoints;
    Vector4D normal1, normal2, normal3;
    //Gett the Vertex and index buffer
    std::vector<Vertex> vertBuffer = graphicsNode.getMeshResource()->getVertexBuffer();
    std::vector<int> indBuffer = graphicsNode.getMeshResource()->getIndexBuffer();

    //Convert the ray into localspace for the model
    //Set 4th coord to 1
    ray.setOrigin(Vector4D(ray.getOrigin()[0], ray.getOrigin()[1], ray.getOrigin()[2], 1));
    ray.setOrigin(Matrix4D::inverse(Matrix4D::getPositionMatrix(position) * totalRotation) * ray.getOrigin());

    ray.setDirection(Matrix4D::inverse(Matrix4D::getPositionMatrix(position) * totalRotation) * ray.getDirection());
    ray.setDirection(Vector4D(ray.getDirection()[0], ray.getDirection()[1], ray.getDirection()[2], 1));
    //Loop through all the triangles in the vector
    for (int i = 0; i < indBuffer.size(); i += 3)
    {
        normal1[0] = vertBuffer[indBuffer[i]].normal[0];   normal1[1] = vertBuffer[indBuffer[i]].normal[1];   normal1[2] = vertBuffer[indBuffer[i]].normal[2];
        normal2[0] = vertBuffer[indBuffer[i+1]].normal[0]; normal2[1] = vertBuffer[indBuffer[i+1]].normal[1]; normal2[2] = vertBuffer[indBuffer[i+1]].normal[2];
        normal3[0] = vertBuffer[indBuffer[i+2]].normal[0]; normal3[1] = vertBuffer[indBuffer[i+2]].normal[1]; normal3[2] = vertBuffer[indBuffer[i+2]].normal[2];

        //Calculate the normals for each triangle
        Vector4D normal = (normal1 + normal2 + normal3) * (1.0/3.0);
        normal[4] = 1;
        normal = normal.normalize();

        //Cehck if we hit the triangle
        if (Vector4D::dotProduct(normal, ray.getDirection()) < 0)
        {
            Vector4D pos1, pos2, pos3;

            //Find the point for our mPlane
            pos1[0] = vertBuffer[indBuffer[i]].pos[0];   pos1[1] = vertBuffer[indBuffer[i]].pos[1];   pos1[2] = vertBuffer[indBuffer[i]].pos[2];
            pos2[0] = vertBuffer[indBuffer[i+1]].pos[0]; pos2[1] = vertBuffer[indBuffer[i+1]].pos[1]; pos2[2] = vertBuffer[indBuffer[i+1]].pos[2];
            pos3[0] = vertBuffer[indBuffer[i+2]].pos[0]; pos3[1] = vertBuffer[indBuffer[i+2]].pos[1]; pos3[2] = vertBuffer[indBuffer[i+2]].pos[2];

            //Construct the vectors we need to check if our point is inside the plane
            Vector4D v2v1 = pos2-pos1; Vector4D v3v2 = pos3-pos2; Vector4D v1v3 = pos1-pos3; 

            //Find the point where we intersected with the plane
            PointAndDistance temp = ray.intersect(mPlane((pos1 + pos2 + pos3)*(1.0/3.0), normal));
            temp.point[3] = 1;

            //Calculate vetors towards the point
            Vector4D PV0 = temp.point - pos1; Vector4D PV1 = temp.point - pos2; Vector4D PV2 = temp.point - pos3;

            //True if we hit the triangle
            if (Vector4D::dotProduct(normal, v2v1.crossProduct(PV0)) > 0 &&
            Vector4D::dotProduct(normal, v3v2.crossProduct(PV1)) > 0 &&
            Vector4D::dotProduct(normal, v1v3.crossProduct(PV2)) > 0)
            {
                DebugManager::getInstance()->createCube((Matrix4D::getPositionMatrix(position) * totalRotation * temp.point), 0.5, 0.5, 0.5, Vector4D(1, 0, 0, 1));
                //DebugManager::getInstance()->createCube((pos1 + pos2 + pos3)*(1.0/3.0), 0.3, 0.3, 0.3, Vector4D(0, 0, 1, 1));
                //DebugManager::getInstance()->createCube(pos1, 0.3, 0.3, 0.3, Vector4D(1, 0, 0, 1));
                //DebugManager::getInstance()->createCube(pos2, 0.3, 0.3, 0.3, Vector4D(1, 0, 0, 1));
                //DebugManager::getInstance()->createCube(pos3, 0.3, 0.3, 0.3, Vector4D(1, 0, 0, 1));
                //DebugManager::getInstance()->createLine((pos1 + pos2 + pos3)*(1.0/3.0), (pos1 + pos2 + pos3)*(1.0/3.0) + normal * 2, Vector4D(1, 0, 0, 1));
                intersectionPoints.push_back(PointAndDistance(Matrix4D::getPositionMatrix(position) * totalRotation * temp.point, temp.distance));
            }
            //DebugManager::getInstance()->createCube((pos1 + pos2 + pos3)*(1.0/3.0), 0.3, 0.3, 0.3, Vector4D(1, 1, 1, 1));
        }  
    }
    if (intersectionPoints.size() <= 0)
        return PointAndDistance(Vector4D(0, 0, 0, -1), -1);

    PointAndDistance closest = PointAndDistance(Vector4D(0, 0, 0, -1), 999999);
    for (int i = 0; i < intersectionPoints.size(); i++)
    {
        if (closest.distance > intersectionPoints[i].distance)
            closest = intersectionPoints[i];
    }

    return closest; 
}

Vector4D& Object::getReferenceToPosition()
{
    return position;
}

Vector4D& Object::getReferenceToRotation()
{
    return rotation;
}

//Origin med allt
//Direction med bara rotation