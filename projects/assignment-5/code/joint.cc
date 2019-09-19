#include "joint.h"

Joint::Joint(int id, Joint* parent, std::string name)
{
    this->ID = id;
    if (parent != nullptr)
        parent->addChild(this);
    this->name = name;
}

Joint::~Joint()
{

}

Joint::Joint()
{

}

void Joint::addChild(Joint* child)
{
    children.push_back(child);
}

void Joint::addMatrixes(Matrix4D pos, Matrix4D rot, Matrix4D sca)
{
    rotation = rot;
    localPosition = pos*rot;
    scale = sca;
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
    worldPosition =  parentTransform * localPosition;

    for (int i = 0; i < children.size(); i++)
    {
        children[i]->calcWorldSpace(worldPosition);
    }

}

/*void Joint::buildJointTreeWithXML(std::string filePath)
{
    TiXmlDocument doc;
    std::vector<Joint*> tempList = std::vector<Joint*>();
    const char *path = filePath.c_str();
    if (doc.LoadFile(path, TIXML_ENCODING_UTF8))
    {
        TiXmlElement *nebula3, *model, *characterNodes, *characterNode, *skinList, *joint;
        nebula3 = doc.FirstChildElement("Nebula3");
        if (nebula3)
        {
            model = nebula3->FirstChildElement("Model");
            if (model)
            {
                characterNodes = model->FirstChildElement("CharacterNodes");
                if (characterNodes)
                {
                    characterNode = characterNodes->FirstChildElement("CharacterNode");

                    if (characterNode)
                    {
                        skinList = characterNode->FirstChildElement("Skinlist");
                        joint = skinList->NextSiblingElement("Joint");
                        bool rootjoint = true;
                        while (joint)
                        {
                            std::cout << "index: " << joint->Attribute("index") << " parent: " << joint->Attribute("parent") << std::endl;

                            if (rootjoint)
                            {
                                tempList.push_back(new Joint(atoi(joint->Attribute("index")), nullptr, joint->Attribute("name")));
                                rootjoint = false;
                            }
                            else
                            {
                                tempList.push_back(new Joint(atoi(joint->Attribute("index")), tempList[atoi(joint->Attribute("parent"))], joint->Attribute("name")));
                            }
                                std::string positionString, rotationString, scaleString;
                                Vector4D positionVector, rotationVector, scaleVector;
                                
                                std::vector<float> floatVector = std::vector<float>();   

                                //Parse out the position from the xml file    
                                positionString = joint->Attribute("position");
                                splitStringIntoFloatVetor(positionString, ',', floatVector);
                                positionVector = Vector4D(floatVector[0],floatVector[1],floatVector[2],floatVector[3]);

                                //Parse out the position from the xml file    
                                rotationString = joint->Attribute("rotation");
                                splitStringIntoFloatVetor(rotationString, ',', floatVector);
                                rotationVector = Vector4D(floatVector[0],floatVector[1],floatVector[2],floatVector[3]);

                                //Parse out the position from the xml file    
                                scaleString = joint->Attribute("scale");
                                splitStringIntoFloatVetor(scaleString, ',', floatVector);
                                scaleVector = Vector4D(floatVector[0],floatVector[1],floatVector[2],floatVector[3]);

                                this->addMatrixes(Matrix4D::getPositionMatrix(positionVector),
                                                 Matrix4D::rotX(rotationVector.getFloat(0))*Matrix4D::rotY(rotationVector.getFloat(1))*Matrix4D::rotZ(rotationVector.getFloat(2)),
                                                 Matrix4D::getScaleMatrix(scaleVector));

                            joint = joint->NextSiblingElement("Joint");
                        }
                    }      
                }
            } 
        }    
    }
}*/

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