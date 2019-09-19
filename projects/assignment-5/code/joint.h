#pragma once
#include <vector>
#include <string>
#include "Matrix4D.h"
#include "tinyxml.h"
#include <sstream>
#include "GraphicsNode.h"
#define TIXML_USE_STL

class Joint
{
    public:
        Joint(int id, Joint* parent, std::string name);
        Joint();
        ~Joint();
        void addChild(Joint* child);
        void addMatrixes(Matrix4D pos, Matrix4D rot, Matrix4D sca);
        void calcInverseLocalPosition(Matrix4D parentLocalTransform);
        void calcWorldSpace(Matrix4D parentLocalTransform);
       // void buildJointTreeWithXML(std::string filePath);
        void drawSetup(MeshResource* mr, TextureResource* tr, ShaderObject* so, LightingNode* ln, Vector4D cameraPos, std::string texture);
        void draw(Matrix4D mat);
        int ID; //Id if the joint
        //Joint* parent; //Pointer to the parent joint
        std::string name; //Name of the joint
        std::vector<Joint*> children = std::vector<Joint*>(); //List of all the joints children
        Matrix4D worldPosition;

    private:
        void splitStringIntoFloatVetor(const std::string &s, char delim, std::vector<float> &elems);
        Matrix4D localPosition;
        Matrix4D inverseLocalPosition;
        Matrix4D rotation;
        Matrix4D scale;
        GraphicsNode JGN;

};

