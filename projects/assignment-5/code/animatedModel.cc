#include "animatedModel.h"

AnimatedModel::AnimatedModel()
{
}

void AnimatedModel::buildJointTreeWithXML(std::string filePath)
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
                        Joint* temp;
                        while (joint)
                        {
                            std::cout << "index: " << joint->Attribute("index") << " parent: " << joint->Attribute("parent") << std::endl;

                            if (rootjoint)
                            {
                                this->rootJoint = temp = new Joint(atoi(joint->Attribute("index")), nullptr, joint->Attribute("name"));
                                tempList.push_back(this->rootJoint);
                                rootjoint = false;
                            }
                            else
                            {
                                temp = new Joint(atoi(joint->Attribute("index")), tempList[atoi(joint->Attribute("parent"))], joint->Attribute("name"));
                                tempList.push_back(temp);
                            }
                                std::string positionString, rotationString, scaleString;
                                Vector4D positionVector, rotationVector, scaleVector;
                                
                                std::vector<float> floatVector = std::vector<float>();   

                                //Parse out the position from the xml file    
                                positionString = joint->Attribute("position");
                                splitStringIntoFloatVetor(positionString, ',', floatVector);
                                positionVector = Vector4D(floatVector[0],floatVector[1],floatVector[2],floatVector[3]);

                                //Parse out the position from the xml file    
                                floatVector.clear();
                                rotationString = joint->Attribute("rotation");
                                splitStringIntoFloatVetor(rotationString, ',', floatVector);
                                rotationVector = Vector4D(floatVector[0],floatVector[1],floatVector[2],floatVector[3]);

                                //Parse out the position from the xml file    
                                floatVector.clear();
                                scaleString = joint->Attribute("scale");
                                splitStringIntoFloatVetor(scaleString, ',', floatVector);
                                scaleVector = Vector4D(floatVector[0],floatVector[1],floatVector[2],floatVector[3]);

                                temp->addMatrixes(Matrix4D::getPositionMatrix(positionVector),
                                                 Matrix4D::rotX(rotationVector.getFloat(0))*Matrix4D::rotY(rotationVector.getFloat(1))*Matrix4D::rotZ(rotationVector.getFloat(2)),
                                                 Matrix4D::getScaleMatrix(scaleVector));

                            joint = joint->NextSiblingElement("Joint");
                        }
                    }      
                }
            } 
        }    
    }
}

void AnimatedModel::splitStringIntoFloatVetor(const std::string &s, char delim, std::vector<float> &elems)
{
    std::stringstream ss(s);
    std::string item;
    float value;
    while(std::getline(ss, item, delim)) {
        value = stof(item);
        elems.push_back(value);
    }
}

void AnimatedModel::jointDrawSetup(MeshResource* mr, TextureResource* tr, ShaderObject* so, LightingNode* ln, Vector4D cameraPos, std::string texturePath)
{
    rootJoint->calcInverseLocalPosition(Matrix4D());
    rootJoint->calcWorldSpace(Matrix4D());
    rootJoint->drawSetup(mr, tr, so, ln, cameraPos, texturePath);
}

void AnimatedModel::draw(Matrix4D mat)
{
    rootJoint->draw(mat);
}