#pragma once
#include "joint.h"
#include "tinyxml.h"
#include "quaternion.h"
#include "nax3Structs.h"
#include "vertexComponent.h"

class AnimatedModel
{
    public:
        AnimatedModel();
        ~AnimatedModel();
        //Setups all the joints in the model. Each joint knows its children.
        void buildJointTreeWithXML(std::string filePath);

        //Setups everything that is needed to draw the joint spheres in the scene
        void jointDrawSetup(MeshResource* mr, TextureResource* tr, ShaderObject* so, LightingNode* ln, Vector4D cameraPos, std::string texture);

        //Draws the models joints. mat is the view and projection matrix
        void draw(Matrix4D mat);

        //Draws the bones of the model
        void drawLines(Matrix4D mat);

        //Moves a joint from its current position that is xyz away from that position
        void moveJointPosition(Matrix4D mat, std::string name);

        //Returns a joint with a certain id
        Joint* getJointFromId(int id);

        //Returns the root joint
        Joint* getRootJoint();

        //Resets the models pose to the original position. (Used in animator)
        void reset();

        //Load mesh data from a nax2 file
        bool loadMeshDataFromNax2(std::string filePath);

        void setup();

        void drawModel(Matrix4D mat);

    private:

    enum N2VertexComponent
    {
        N2Coord        = (1<<0),      // 3 floats
        N2Normal       = (1<<1),      // 3 floats
        N2NormalB4N   = (1<<2),      // 4 unsigned bytes, normalized
        N2Uv0          = (1<<3),      // 2 floats
        N2Uv0S2        = (1<<4),      // 2 shorts, 4.12 fixed point
        N2Uv1          = (1<<5),      // 2 floats
        N2Uv1S2        = (1<<6),      // 2 shorts, 4.12 fixed point
        N2Uv2          = (1<<7),      // 2 floats
        N2Uv2S2        = (1<<8),      // 2 shorts, 4.12 fixed point
        N2Uv3          = (1<<9),      // 2 floats
        N2Uv3S2        = (1<<10),     // 2 shorts, 4.12 fixed point
        N2Color        = (1<<11),     // 4 floats
        N2ColorUB4N    = (1<<12),     // 4 unsigned bytes, normalized
        N2Tangent      = (1<<13),     // 3 floats
        N2TangentB4N  = (1<<14),     // 4 unsigned bytes, normalized
        N2Binormal     = (1<<15),     // 3 floats
        N2BinormalB4N = (1<<16),     // 4 unsigned bytes, normalized
        N2Weights      = (1<<17),     // 4 floats
        N2WeightsUB4N  = (1<<18),     // 4 unsigned bytes, normalized
        N2JIndices     = (1<<19),     // 4 floats
        N2JIndicesUB4  = (1<<20),     // 4 unsigned bytes

        N2NumVertexComponents = 21,
        N2AllComponents = ((1<<N2NumVertexComponents) - 1),
    };

        unsigned int VAO;
        unsigned int EBO;
        unsigned int VBO;
        unsigned int TEX;
        unsigned int Diffuse;
        unsigned int Normal;
        const char* vs;
        const char* fs;
        unsigned int vertexShader;
        unsigned int fragmentShader;
        unsigned int program;
        

        MeshResource mesh;
        TextureResource texture;
        ShaderObject shader;

        void* groupDataPtr;
        void* vertexDataPtr;
        void* indexDataPtr;

        uint groupDataSize;
        uint vertexDataSize;
        uint indexDataSize;

        uint numGroups;
        uint numVertices;
        uint vertexWidth;
        uint numIndices;
        uint numEdges;
        uint vertexComponentMask;

        std::vector<VertexComponent> vertexComponents;

        //The rootjoin of the joint tree
        Joint* rootJoint;

        //Splits a string into a float vector. Used to pares the values from the xml file
        void splitStringIntoFloatVetor(const std::string &s, char delim, std::vector<float> &elems);

        std::vector<Joint*> jointArray;
};