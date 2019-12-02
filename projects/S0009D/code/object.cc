#include "object.h"

Object::Object(){}

Object::Object(MeshResource* mr, ShaderObject* so, TextureResource* tr, LightingNode* ln, Vector4D& cameraPos, std::string texturePath)
{
    setupGraphicsNode(mr, so, tr, ln, cameraPos, texturePath);
    rb = Rigidbody(originalAABB, 10, totalRotation, getReferenceToPosition());
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
    //Create the original AABB with mesh data
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

    //Convert to worldspace
    originalAABB.minPoint[0] += position[0];
    originalAABB.minPoint[1] += position[1];
    originalAABB.minPoint[2] += position[2];

    originalAABB.maxPoint[0] += position[0];
    originalAABB.maxPoint[1] += position[1];
    originalAABB.maxPoint[2] += position[2];

    //Set current AABB to the original
    currentAABB = originalAABB;

    //Debug things
    Vector4D dimentions = Vector4D(originalAABB.maxPoint[0]-originalAABB.minPoint[0], originalAABB.maxPoint[1]-originalAABB.minPoint[1], originalAABB.maxPoint[2]-originalAABB.minPoint[2], 1);
    Vector4D pos = Vector4D(originalAABB.minPoint[0] + (dimentions[0]/2),
                                originalAABB.minPoint[1] + (dimentions[1]/2),
                                originalAABB.minPoint[2] + (dimentions[2]/2),
                                1);
    //DebugManager::getInstance()->createSingleFrameCube(pos, dimentions[0], dimentions[1], dimentions[2], colorOnAABB, true);
    
}

void Object::updateAABB()
{
    //Reset AABB
    currentAABB.maxPoint = Vector4D(-99999, -99999, -99999, 1);
    currentAABB.minPoint = Vector4D(99999, 99999, 99999, 1);

    //Create a vector that will contain all the points for the AABB
    std::vector<Vector4D> pointVector;

    //Create the combinedMatrix with rotation and scale
    Matrix4D combinedMatrix = totalRotation * Matrix4D::getScaleMatrix(scale);

    //Apply the matrix to the original AABB and add the new points to the vector
    pointVector.push_back(combinedMatrix * Vector4D(originalAABB.maxPoint[0], originalAABB.maxPoint[1], originalAABB.maxPoint[2], 1));
    pointVector.push_back(combinedMatrix * Vector4D(originalAABB.maxPoint[0], originalAABB.minPoint[1], originalAABB.maxPoint[2], 1));
    pointVector.push_back(combinedMatrix * Vector4D(originalAABB.minPoint[0], originalAABB.minPoint[1], originalAABB.maxPoint[2], 1));
    pointVector.push_back(combinedMatrix * Vector4D(originalAABB.minPoint[0], originalAABB.maxPoint[1], originalAABB.maxPoint[2], 1));

    pointVector.push_back(combinedMatrix * Vector4D(originalAABB.maxPoint[0], originalAABB.maxPoint[1], originalAABB.minPoint[2], 1));
    pointVector.push_back(combinedMatrix * Vector4D(originalAABB.maxPoint[0], originalAABB.minPoint[1], originalAABB.minPoint[2], 1));
    pointVector.push_back(combinedMatrix * Vector4D(originalAABB.minPoint[0], originalAABB.minPoint[1], originalAABB.minPoint[2], 1));
    pointVector.push_back(combinedMatrix * Vector4D(originalAABB.minPoint[0], originalAABB.maxPoint[1], originalAABB.minPoint[2], 1));

    //Find the min and max
    for (int i = 0; i < pointVector.size(); i++)
    {
        if (pointVector[i][0] >= currentAABB.maxPoint[0]) currentAABB.maxPoint[0] = pointVector[i][0];
        if (pointVector[i][1] >= currentAABB.maxPoint[1]) currentAABB.maxPoint[1] = pointVector[i][1];
        if (pointVector[i][2] >= currentAABB.maxPoint[2]) currentAABB.maxPoint[2] = pointVector[i][2];
        if (pointVector[i][0] <= currentAABB.minPoint[0]) currentAABB.minPoint[0] = pointVector[i][0];
        if (pointVector[i][1] <= currentAABB.minPoint[1]) currentAABB.minPoint[1] = pointVector[i][1];
        if (pointVector[i][2] <= currentAABB.minPoint[2]) currentAABB.minPoint[2] = pointVector[i][2];
    }

    //Apply position matrix to the new AABB
    currentAABB.minPoint = Matrix4D::getPositionMatrix(position) * currentAABB.minPoint;
    currentAABB.maxPoint = Matrix4D::getPositionMatrix(position) * currentAABB.maxPoint;


    //Debug thingy
    Vector4D dimentions = Vector4D(currentAABB.maxPoint[0]-currentAABB.minPoint[0], currentAABB.maxPoint[1]-currentAABB.minPoint[1], currentAABB.maxPoint[2]-currentAABB.minPoint[2], 1);
    Vector4D pos = Vector4D(currentAABB.minPoint[0] + (dimentions[0]/2),
                                currentAABB.minPoint[1] + (dimentions[1]/2),
                                currentAABB.minPoint[2] + (dimentions[2]/2),
                                1);
    //DebugManager::getInstance()->createSingleFrameCube(pos, dimentions[0], dimentions[1], dimentions[2], colorOnAABB, true);
}

void Object::draw()
{
    //Debug thiny for AABB
    Vector4D dimentions = Vector4D(currentAABB.maxPoint[0]-currentAABB.minPoint[0], currentAABB.maxPoint[1]-currentAABB.minPoint[1], currentAABB.maxPoint[2]-currentAABB.minPoint[2], 1);
    Vector4D pos = Vector4D(currentAABB.minPoint[0] + (dimentions[0]/2),
                                currentAABB.minPoint[1] + (dimentions[1]/2),
                                currentAABB.minPoint[2] + (dimentions[2]/2),
                                1);
    DebugManager::getInstance()->createSingleFrameCube(pos, dimentions[0], dimentions[1], dimentions[2], colorOnAABB, true);
    colorOnAABB = Vector4D(0, 0, 1, 0);


    Matrix4D rotationX = Matrix4D::rotX(rotation[0]);
    Matrix4D rotationY = Matrix4D::rotY(rotation[1]);
    Matrix4D rotationZ = Matrix4D::rotZ(rotation[2]);
    totalRotation = rb.getRotation();
    Vector4D center = rb.getCenterPoint();
    Vector4D mcenter = center * -1;
    totalRotation = Matrix4D::getPositionMatrix(center) * totalRotation * Matrix4D::getPositionMatrix(mcenter);
    // graphicsNode.setTransform(viewmatrix * Matrix4D::getPositionMatrix(position) * totalRotation * Matrix4D::getScaleMatrix(scale));
    // graphicsNode.setPosition(Matrix4D::getPositionMatrix(position) * totalRotation * Matrix4D::getScaleMatrix(scale));

    graphicsNode.setTransform(viewmatrix * rb.worldTransform);
    graphicsNode.setPosition(rb.worldTransform);

    graphicsNode.draw();
}

void Object::update()
{
    position = rb.getPosition(); 
    updateAABB();
    totalRotation = rb.getRotation();
    rb.update();
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

    //Get the combined matrix of scale and rotation
    Matrix4D combinedMatrix = rb.worldTransform;// Matrix4D::getPositionMatrix(position) * totalRotation * Matrix4D::getScaleMatrix(scale);

    //Get the Vertex and index buffer
    std::vector<Vertex> vertBuffer = graphicsNode.getMeshResource()->getVertexBuffer();
    std::vector<int> indBuffer = graphicsNode.getMeshResource()->getIndexBuffer();

    //Save origin for so we can use it to calculate distance
    Vector4D originOriginal = ray.getOrigin();

    //Convert the ray into the localspace of the model
    ray.setOrigin(Vector4D(ray.getOrigin()[0], ray.getOrigin()[1], ray.getOrigin()[2], 1)); //Set 4 coord to 1 or else...
    ray.setOrigin(Matrix4D::inverse(combinedMatrix) * ray.getOrigin());

    ray.setDirection(Matrix4D::inverse(combinedMatrix) * ray.getDirection());
    ray.setDirection(Vector4D(ray.getDirection()[0], ray.getDirection()[1], ray.getDirection()[2], 0)); //Same here

    //Loop through all the triangles
    for (int i = 0; i < indBuffer.size(); i += 3)
    {
        //Calculate the normal for the triangle
        Vector4D pos1, pos2, pos3;
        pos1[0] = vertBuffer[indBuffer[i]].pos[0];   pos1[1] = vertBuffer[indBuffer[i]].pos[1];   pos1[2] = vertBuffer[indBuffer[i]].pos[2];
        pos2[0] = vertBuffer[indBuffer[i+1]].pos[0]; pos2[1] = vertBuffer[indBuffer[i+1]].pos[1]; pos2[2] = vertBuffer[indBuffer[i+1]].pos[2];
        pos3[0] = vertBuffer[indBuffer[i+2]].pos[0]; pos3[1] = vertBuffer[indBuffer[i+2]].pos[1]; pos3[2] = vertBuffer[indBuffer[i+2]].pos[2];

        Vector4D normal = (pos2 - pos1).crossProduct(pos3 - pos1);
        normal = normal.normalize();

        //Cehck if we hit the triangle
        if (Vector4D::dotProduct(normal, ray.getDirection()) < 0)
        {
            //Construct the vectors we need to check if our point is inside the plane
            Vector4D v2v1 = pos2-pos1; Vector4D v3v2 = pos3-pos2; Vector4D v1v3 = pos1-pos3;

            //Find the point where we intersected with the plane
            PointAndDistance temp = ray.intersect(mPlane((pos1 + pos2 + pos3)*(1.0/3.0), normal));
            if (temp.distance == -1)
                continue;
            
            temp.point[3] = 1;

            //Calculate vetors towards the point from all corners of the triangle
            Vector4D PV0 = temp.point - pos1; Vector4D PV1 = temp.point - pos2; Vector4D PV2 = temp.point - pos3;

            //Check if we are inside the triangle
            if (Vector4D::dotProduct(normal, v2v1.crossProduct(PV0)) > 0 &&
            Vector4D::dotProduct(normal, v3v2.crossProduct(PV1)) > 0 &&
            Vector4D::dotProduct(normal, v1v3.crossProduct(PV2)) > 0)
            {
                DebugManager::getInstance()->createCube((combinedMatrix * temp.point), 0.5, 0.5, 0.5, Vector4D(1, 0, 0, 1));
                
                //Add the intersection point to the vector
                intersectionPoints.push_back(PointAndDistance(combinedMatrix * temp.point, temp.distance, temp.normal));
            }
        }  
    }
    //If we didnt intersect with the mesh return with distance of -1
    if (intersectionPoints.size() <= 0)
        return PointAndDistance(Vector4D(0, 0, 0, -1), -1, {});

    //Else find the closest point of intersection and return it
    PointAndDistance closest = PointAndDistance(Vector4D(0, 0, 0, -1), 999999, {});
    for (int i = 0; i < intersectionPoints.size(); i++)
    {
        if (closest.distance > intersectionPoints[i].distance)
            closest = intersectionPoints[i];
    }

    return closest; 
}

Vector4D& Object::getReferenceToPosition()
{
    return rb.getPosition();
}

Vector4D& Object::getReferenceToRotation()
{
    return rotation;
}

Vector4D& Object::getReferenceToScale()
{
    return scale;
}

Rigidbody& Object::getReferenceToRigidbody()
{
    return rb;
}

AABB& Object::getReferenceToAABB()
{
    return currentAABB;
}

GraphicsNode Object::getGraphicsNode()
{
    return graphicsNode;
}

int Object::indexOfFurthestPoint(Vector4D direction)
{
    int index = 0;
    std::vector<Vertex> vertexBuffer = graphicsNode.getMeshResource()->getVertexBuffer();
    float maxProduct = direction.dotProduct(Vector4D(vertexBuffer[index].pos, 1));
    float product = 0;
    for (int i = 1; i < vertexBuffer.size(); i++)
    {
        product = direction.dotProduct(Vector4D(vertexBuffer[i].pos, 1));
        if (product > maxProduct)
        {
            maxProduct = product;
            index = i;
        }  
    }
    return index;
}