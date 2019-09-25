#include "animatedModel.h"

AnimatedModel::AnimatedModel()
{
}

AnimatedModel::~AnimatedModel()
{
    delete rootJoint;
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

                                //Parse out the rotation from the xml file    
                                floatVector.clear();
                                rotationString = joint->Attribute("rotation");
                                splitStringIntoFloatVetor(rotationString, ',', floatVector);
                                Matrix4D rotationMatrix = Quaternion::createMatrix(floatVector[0],floatVector[1],floatVector[2],floatVector[3]);
                                rotationVector = Vector4D(floatVector[0],floatVector[1],floatVector[2],floatVector[3]);

                                //Parse out the scale from the xml file    
                                floatVector.clear();
                                scaleString = joint->Attribute("scale");
                                splitStringIntoFloatVetor(scaleString, ',', floatVector);
                                scaleVector = Vector4D(floatVector[0],floatVector[1],floatVector[2],floatVector[3]);

                                temp->addMatrixes(Matrix4D::getPositionMatrix(positionVector),
                                                 rotationMatrix,
                                                 Matrix4D::getScaleMatrix(scaleVector));
                                                 //rotate[0][0], rotate[1][0], rotate[2][0], rotate[3][0], rotate[0][1], rotate[1][1], rotate[2][1], rotate[3][1], rotate[0][2], rotate[1][2], rotate[2][2], rotate[3][2], rotate[0][3], rotate[1][3], rotate[2][3], rotate[3][3]

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

void AnimatedModel::drawLines(Matrix4D mat)
{
    rootJoint->drawLines(Matrix4D::transpose(mat));
}

void AnimatedModel::moveJointPosition(Matrix4D mat, std::string name)
{
    rootJoint->movePosition(mat, name);
    rootJoint->calcWorldSpace(Matrix4D());
}

void AnimatedModel::quaternionToEuler(std::vector<float> &quaternion)
{
    float x, y, z , w;
    quaternion.clear();

    //calc roll rotation
    x = quaternion[0]; y = quaternion[1]; z = quaternion[2]; w = quaternion[3];
    double sinr_cosp = +2.0 * ((w * x) + (y * z));
    double cosr_cosp = +1.0 - 2.0 * ((x * x) + (y * y));
    quaternion.push_back(atan2(sinr_cosp, cosr_cosp));

    //calc pitch rotation
    double sinp = +2.0 * (w * y - z * x);
    if (fabs(sinp) >= 1)
        quaternion.push_back(copysign(M_PI / 2, sinp));
    else
        quaternion.push_back(asin(sinp));

    //calc yaw
    double siny_cosp = +2.0 * ((w*z) + (x*y));
    double cosy_cosp = +1.0 - 2.0 * ((y*y) + (z*z));
    quaternion.push_back(atan2(siny_cosp, cosy_cosp)); 
}