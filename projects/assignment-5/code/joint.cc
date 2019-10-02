#include "joint.h"

Joint::Joint()
{

}

Joint::Joint(int id, Joint* parent, std::string name)
{
    this->ID = id;
    if (parent != nullptr)
        parent->addChild(this);
    this->name = name;
}

Joint::~Joint()
{
    for (int i = 0; i < children.size(); i++)
    {
        delete children[i];
    }
    
}

void Joint::addChild(Joint* child)
{
    children.push_back(child);
}

void Joint::addMatrixes(Matrix4D pos, Matrix4D rot, Matrix4D sca)
{
    rotation = rot;
    localPosition = pos;
    scale = sca;
    transform = pos*rot*sca;
}

void Joint::calcInverseLocalPosition(Matrix4D parentLocalTransform)
{
    Matrix4D temp = localPosition * parentLocalTransform;
    inverseLocalPosition = Matrix4D::inverse(temp);
    for (int i = 0; i < children.size(); i++)
    {
        children[i]->calcInverseLocalPosition(temp);
    }  
}

void Joint::calcWorldSpace(Matrix4D parentTransform)
{
    worldPosition =  parentTransform * transform;
    defaultTransform = worldPosition;

    for (int i = 0; i < children.size(); i++)
    {
        children[i]->calcWorldSpace(worldPosition);
    }

}

void Joint::splitStringIntoFloatVetor(const std::string &s, char delim, std::vector<float> &elems)
{
    std::stringstream ss(s);
    std::string item;
    float value;
    while(std::getline(ss, item, delim)) {
        value = stof(item);
        elems.push_back(value);
    }
}

void Joint::drawSetup(MeshResource* mr, TextureResource* tr, ShaderObject* so, LightingNode* ln, Vector4D cameraPos, std::string texture)
{
    JGN.setMeshResource(mr);
	JGN.setShaderObject(so);
	JGN.setTextureResource(tr);
	JGN.setlightingNode(ln);
	JGN.setCameraPosition(cameraPos);
	JGN.loadTexture("texture.jpg");
	JGN.preDrawSetup();

    for (int i = 0; i < children.size(); i++)
    {
        children[i]->drawSetup(mr, tr, so, ln, cameraPos, texture);
    }
    
}

void Joint::draw(Matrix4D mat)
{
    JGN.setTransform(mat*worldPosition*Matrix4D(0.01, 0, 0, 0,
                                                0, 0.01, 0, 0,
                                                0, 0, 0.01, 0,
                                                0, 0, 0, 1));
    JGN.setPosition(worldPosition);
    JGN.draw();
    for (int i = 0; i < children.size(); i++)
    {        
        children[i]->draw(mat);
    }
}

void Joint::drawLines(Matrix4D mat)
{
    for (int i = 0; i < children.size(); i++)
    {        
        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf((GLfloat*)&mat);

        glBegin(GL_LINES);
            glVertex3f(worldPosition[3], worldPosition[7], worldPosition[11]);
            glVertex3f(children[i]->worldPosition[3], children[i]->worldPosition[7], children[i]->worldPosition[11]);
        glEnd();

        children[i]->drawLines(mat);
    }
}

int Joint::movePosition(Matrix4D mat, std::string jointName)
{
    if (jointName == name)
    {
        localPosition = localPosition * mat;
        transform = localPosition*rotation*scale;
        return 1;
    }
    for (int i = 0; i < children.size(); i++)
    {
        if(children[i]->movePosition(mat, jointName) == 1)
        {
            return 1;
        }
    }
    return 0;    
}

Joint* Joint::getJoint(int id)
{
    if (this->ID == id)
    {
        return this;
    }
    else
    {
        for (int i = 0; i < children.size(); i++)
        {
            Joint* joint = children[i]->getJoint(id);
            if (!joint){}
            else
            {
                return joint;
            }
        }
    }  
    return NULL;
}

void Joint::setWorldPosition(Matrix4D mat)
{

}

void Joint::reset()
{
    worldPosition = defaultTransform;
}

std::vector<Joint*> Joint::getChildren()
{
    return this->children;
}

int Joint::getID()
{
    return ID;
}