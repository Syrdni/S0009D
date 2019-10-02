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
    reset();
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

Joint* AnimatedModel::getJointFromId(int id)
{
    return rootJoint->getJoint(id);
}

Joint* AnimatedModel::getRootJoint()
{
    return rootJoint;
}

void AnimatedModel::reset()
{
    rootJoint->reset();
}

bool AnimatedModel::loadMeshDataFromNax2(std::string filePath)
{
    //Open the Nax3 File
    std::ifstream infile(filePath.c_str(), std::ifstream::binary);

    //Check if the file exists
    if (!infile)
        return false;
    
    //Get the pointer to the buffer object
    std::filebuf* pbuf = infile.rdbuf();

    //Get the file size
    std::size_t size = pbuf->pubseekoff(0, infile.end, infile.in);
    pbuf->pubseekpos(0, infile.in);

    //Allocate memory to contain the data
    char* ptr = new char[size];

    //Get the file data
    pbuf->sgetn(ptr,size);
    infile.close();

    //Header data
    Nvx2Header* header = (Nvx2Header*) ptr;
    header->numIndices *= 3;

    if(header->magic != NAX2_MAGICNUMBER)
        return false;

    numGroups = header->numGroups;
    numVertices = header->numVertices;
    vertexWidth = header->vertexWidth;
    numIndices = header->numIndices;
    vertexComponentMask = header->vertexComponentMask;
    groupDataSize = 6 * sizeof(uint) * this->numGroups;
    vertexDataSize = this->numVertices * this->vertexWidth * sizeof(float);
    indexDataSize = numIndices * sizeof(int);
    groupDataPtr = header + 1;
    vertexDataPtr = ((char*)groupDataPtr) + groupDataSize;
    indexDataPtr = ((char*)vertexDataPtr) + vertexDataSize;

    //Load vertices
    for (int i = 0; i < 21; i++)
    {
        VertexType vertexType;
        Format format;
        if (vertexComponentMask & (1<<i))
        {
            switch (1<<i)
            {
                case N2Coord:       vertexType = VertexType::Position; format = Format::Float3; break;
                case N2Normal:      vertexType = VertexType::Normal; format = Format::Float3; break;
                case N2NormalB4N:   vertexType = VertexType::Normal; format = Format::Byte4N; break;
                case N2Uv0:         vertexType = VertexType::TexCoord1; format = Format::Float2; break;
                case N2Uv0S2:       vertexType = VertexType::TexCoord1; format = Format::Short2; break;
                case N2Uv1:         vertexType = VertexType::TexCoord2; format = Format::Float2; break;
                case N2Uv1S2:       vertexType = VertexType::TexCoord2; format = Format::Short2; break;
                case N2Color:       vertexType = VertexType::Color; format = Format::Float4; break;
                case N2ColorUB4N:   vertexType = VertexType::Color; format = Format::UByte4N; break;
                case N2Tangent:     vertexType = VertexType::Tangent; format = Format::Float3; break;
                case N2TangentB4N:  vertexType = VertexType::Tangent; format = Format::Byte4N; break;
                case N2Binormal:    vertexType = VertexType::Binormal; format = Format::Float3; break;
                case N2BinormalB4N: vertexType = VertexType::Binormal; format = Format::Byte4N; break;
                case N2Weights:     vertexType = VertexType::SkinWeights; format = Format::Float4; break;
                case N2WeightsUB4N: vertexType = VertexType::SkinWeights; format = Format::UByte4N; break;
                case N2JIndices:    vertexType = VertexType::SkinJIndices; format = Format::Float4; break;
                case N2JIndicesUB4: vertexType = VertexType::SkinJIndices; format = Format::UByte4; break;
            
            default:
                break;
            }
            this->vertexComponents.push_back(VertexComponent(vertexType, format));
        }
    }

    glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexDataSize, &vertexDataPtr, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexDataSize, &indexDataPtr, GL_STATIC_DRAW);

    
    
    int i = 0;
}