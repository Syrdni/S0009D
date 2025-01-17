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
    jointArray = std::vector<Joint*>();
    std::vector<int> orderVector;
    const char *path = filePath.c_str();
    if (doc.LoadFile(path, TIXML_ENCODING_UTF8))
    {
        TiXmlElement *nebula3, *model, *characterNodes, *characterNode, *skins, *skin, *fragment, *joints, *skinList, *joint;
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
                            if (rootjoint)
                            {
                                this->rootJoint = temp = new Joint(atoi(joint->Attribute("index")), nullptr, joint->Attribute("name"));
                                jointArray.push_back(this->rootJoint);
                                rootjoint = false;
                            }
                            else
                            {
                                temp = new Joint(atoi(joint->Attribute("index")), jointArray[atoi(joint->Attribute("parent"))], joint->Attribute("name"));
                                jointArray.push_back(temp);
                            }
                                std::string positionString, rotationString, scaleString;
                                Vector4D positionVector, rotationVector, scaleVector;
                                
                                std::vector<float> floatVector = std::vector<float>();   

                                //Parse out the position from the xml file    
                                positionString = joint->Attribute("position");
                                splitStringIntoFloatVector(positionString, ',', floatVector);
                                positionVector = Vector4D(floatVector[0],floatVector[1],floatVector[2],floatVector[3]);

                                //Parse out the rotation from the xml file    
                                floatVector.clear();
                                rotationString = joint->Attribute("rotation");
                                splitStringIntoFloatVector(rotationString, ',', floatVector);
                                Matrix4D rotationMatrix = Quaternion::createMatrix(floatVector[0],floatVector[1],floatVector[2],floatVector[3]);
                                rotationVector = Vector4D(floatVector[0],floatVector[1],floatVector[2],floatVector[3]);

                                //Parse out the scale from the xml file    
                                floatVector.clear();
                                scaleString = joint->Attribute("scale");
                                splitStringIntoFloatVector(scaleString, ',', floatVector);
                                scaleVector = Vector4D(floatVector[0],floatVector[1],floatVector[2],floatVector[3]);

                                temp->addMatrixes(Matrix4D::getPositionMatrix(positionVector),
                                                 rotationMatrix,
                                                 Matrix4D::getScaleMatrix(scaleVector));
                                                 //rotate[0][0], rotate[1][0], rotate[2][0], rotate[3][0], rotate[0][1], rotate[1][1], rotate[2][1], rotate[3][1], rotate[0][2], rotate[1][2], rotate[2][2], rotate[3][2], rotate[0][3], rotate[1][3], rotate[2][3], rotate[3][3]

                            joint = joint->NextSiblingElement("Joint");
                        }
                    }      
                }
                skins = model->FirstChildElement("Skins");
                if (skins)
                {
                    skin = skins->FirstChildElement("Skin");
                    if (skin)
                    {
                        fragment = skin->FirstChildElement("Fragment");
                        if (fragment)
                        {
                            joint = fragment->FirstChildElement("Joints");
                            std::string order = joint->GetText();

                            //Split the string and conver to ints
                            std::stringstream ss(order);
                            std::string token;
                            while(std::getline(ss, token, ','))
                            {
                                orderVector.push_back(atoi(token.c_str()));
                            }
                        }
                    }
                }  
            } 
        }    
    }
    std::vector<Joint*> tempVector;

    for (int i = 0; i < orderVector.size(); i++)
    {
        tempVector.push_back(jointArray[orderVector[i]]);
    }
    defaultArray = jointArray;
    jointArray = tempVector;

    rootJoint->calcInverseLocalPosition(Matrix4D());
}

void AnimatedModel::splitStringIntoFloatVector(const std::string &s, char delim, std::vector<float> &elems)
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
    rootJoint->calcWorldSpace(positionMatrix);
    rootJoint->drawSetup(mr, tr, so, ln, cameraPos, texturePath);
    lightNode = ln;
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
    rootJoint->calcWorldSpace(positionMatrix);
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
        int sizeInBytes, isNormalized, sizeA;
        if (vertexComponentMask & (1<<i))
        {
            switch (1<<i)
            {
                case N2Coord:       vertexType = VertexType::Position;      format = Format::Float3;    sizeInBytes = 12;   isNormalized = 0; sizeA = 3; break;
                case N2Normal:      vertexType = VertexType::Normal;        format = Format::Float3;    sizeInBytes = 12;   isNormalized = 0; sizeA = 3; break;
                case N2NormalB4N:   vertexType = VertexType::Normal;        format = Format::Byte4N;    sizeInBytes = 4;    isNormalized = 1; sizeA = 4; break;
                case N2Uv0:         vertexType = VertexType::TexCoord1;     format = Format::Float2;    sizeInBytes = 8;    isNormalized = 0; sizeA = 2; break;
                case N2Uv0S2:       vertexType = VertexType::TexCoord1;     format = Format::Short2;    sizeInBytes = 4;    isNormalized = 0; sizeA = 2; break;
                case N2Uv1:         vertexType = VertexType::TexCoord2;     format = Format::Float2;    sizeInBytes = 8;    isNormalized = 0; sizeA = 2; break;
                case N2Uv1S2:       vertexType = VertexType::TexCoord2;     format = Format::Short2;    sizeInBytes = 4;    isNormalized = 0; sizeA = 2; break;
                case N2Color:       vertexType = VertexType::Color;         format = Format::Float4;    sizeInBytes = 16;   isNormalized = 0; sizeA = 4; break;
                case N2ColorUB4N:   vertexType = VertexType::Color;         format = Format::UByte4N;   sizeInBytes = 4;    isNormalized = 1; sizeA = 4; break;
                case N2Tangent:     vertexType = VertexType::Tangent;       format = Format::Float3;    sizeInBytes = 12;   isNormalized = 0; sizeA = 3; break;
                case N2TangentB4N:  vertexType = VertexType::Tangent;       format = Format::Byte4N;    sizeInBytes = 4;    isNormalized = 1; sizeA = 4; break;
                case N2Binormal:    vertexType = VertexType::Binormal;      format = Format::Float3;    sizeInBytes = 12;   isNormalized = 0; sizeA = 3; break;
                case N2BinormalB4N: vertexType = VertexType::Binormal;      format = Format::Byte4N;    sizeInBytes = 4;    isNormalized = 1; sizeA = 4; break;
                case N2Weights:     vertexType = VertexType::SkinWeights;   format = Format::Float4;    sizeInBytes = 16;   isNormalized = 0; sizeA = 4; break;
                case N2WeightsUB4N: vertexType = VertexType::SkinWeights;   format = Format::UByte4N;   sizeInBytes = 4;    isNormalized = 1; sizeA = 4; break;
                case N2JIndices:    vertexType = VertexType::SkinJIndices;  format = Format::Float4;    sizeInBytes = 16;   isNormalized = 0; sizeA = 4; break;
                case N2JIndicesUB4: vertexType = VertexType::SkinJIndices;  format = Format::UByte4;    sizeInBytes = 4;    isNormalized = 0; sizeA = 4; break;
            
            default:
                break;
            }
            this->vertexComponents.push_back(VertexComponent(vertexType, format, sizeInBytes, isNormalized, sizeA));
        }
    }
    int i = 0;
}

void AnimatedModel::drawModel(Matrix4D ViewProction, Matrix4D modelMatrix, Vector4D cameraPos)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Diffuse);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, Normal);

    glUseProgram(program);

    unsigned int transformLoc = glGetUniformLocation(program, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_TRUE, ViewProction.getMatrix());

    transformLoc = glGetUniformLocation(program, "cameraPosition");
	glUniform4fv(transformLoc, 1, cameraPos.getVector());


    std::vector<float> transformArray;
    if(animationPlaying)
    {
        for (int i = 0; i < jointArray.size(); i++)
        {
            for (int j = 0; j < 16; j++)
            {
                transformArray.push_back(jointArray[i]->transform.getMatrix()[j]);
            }  
        }
    }
    else
    {
        for (int i = 0; i < jointArray.size(); i++)
        {
            for (int j = 0; j < 16; j++)
            {
                transformArray.push_back(defaultArray[i]->worldPosition.getMatrix()[j]);
            }  
        }
    }
    
    transformLoc = glGetUniformLocation(program, "jointTransforms");
    glUniformMatrix4fv(transformLoc, jointArray.size(), GL_TRUE, &transformArray[0]);

    transformLoc = glGetUniformLocation(program, "modelMatrix");
    glUniformMatrix4fv(transformLoc, 1, GL_TRUE, modelMatrix.getMatrix());

    transformLoc = glGetUniformLocation(program, "isPlaying");
    glUniform1i(transformLoc, animationPlaying);

    transformLoc = glGetUniformLocation(program, "aLightColor");
    glUniform4fv(transformLoc, 1, lightNode->getColorWithIntensity().getVector());

    transformLoc = glGetUniformLocation(program, "aLightPosition");
    glUniform4fv(transformLoc, 1, lightNode->getPosition().getVector());

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, vertexDataSize, GL_UNSIGNED_INT, NULL);
    glBindVertexArray(0); 
    glUseProgram(0);
}

void AnimatedModel::setup()
{
    glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexDataSize, vertexDataPtr, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexDataSize, indexDataPtr, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, vertexComponents[0].sizeArray, vertexComponents[0].getType(), vertexComponents[0].isNormalized, sizeof(float)* vertexWidth, NULL);

    int amountOfBytes = vertexComponents[0].sizeInBytes;
    for (int i = 1; i < vertexComponents.size(); i++)
    {
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, vertexComponents[i].sizeArray, vertexComponents[i].getType(), vertexComponents[i].isNormalized, sizeof(float)* vertexWidth, (void*)(sizeof(char)*amountOfBytes));
        amountOfBytes += vertexComponents[i].sizeInBytes;
    }
	glBindVertexArray(0);

    //Handels for textures
    glGenTextures(1, &Diffuse);

    //Get diffuse Texutre
    glBindTexture(GL_TEXTURE_2D, Diffuse);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, nrChannels;
    unsigned char* data = stbi_load("Footman_Diffuse.tga", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0 , GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    //Get Normal Texutre
    glGenTextures(1, &Normal);
    glBindTexture(GL_TEXTURE_2D, Normal);
    data = stbi_load("Footman_Normal.tga", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0 , GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    //Vertex Shader
	std::ifstream file;
	file.open("vertexShaderTest.vert");
	if (file.fail()) {
		std::cout << "Failed to load vertexShader" << std::endl;
		return;
	}
	else {
		std::stringstream tempstream;
		tempstream << file.rdbuf();
		std::string temp = tempstream.str();
		vs = temp.c_str();

        const GLint lengthOfVertexShader = strlen(vs);
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vs, &lengthOfVertexShader);
        glCompileShader(vertexShader);

        //ERROR LOG
        int  success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
		file.close();
	}

	file.open("fragmentShaderTest.frag");
	if (file.fail()) {
		std::cout << "Failed to load fragmentShader" << std::endl;
		return;
	}

	else {
		std::stringstream tempstream;
		tempstream << file.rdbuf();
		std::string temp = tempstream.str();
		fs = temp.c_str();
		const GLint lengthOfPixelShader = strlen(fs);
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fs, &lengthOfPixelShader);
        glCompileShader(fragmentShader);

        int  success;
        char infoLog[512];
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
		file.close();
	}

    program = glCreateProgram();
	glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
	glLinkProgram(program);

    glUseProgram(program);
    glUniform1i(glGetUniformLocation(program, "diffuseTexture"), 0);
    glUniform1i(glGetUniformLocation(program, "normalMap"), 1);
    glUseProgram(0);
}

void AnimatedModel::setPosition(Matrix4D mat)
{
    positionMatrix = mat;
    rootJoint->calcWorldSpace(mat);
}

void AnimatedModel::isPlaying(bool var)
{
    animationPlaying = var;
}