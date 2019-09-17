#pragma once
#include <vector>
#include <string>
#include "Matrix4D.h"
#include "tinyxml.h"
#include <sstream>
#define TIXML_USE_STL

class Joint
{
    public:
        Joint(int id, Joint* parent, std::string name);
        Joint();
        ~Joint();
        void addChild(Joint* child);
        void addMatrixes(Matrix4D pos, Matrix4D rot, Matrix4D sca);
        void buildJointTreeWithXML(std::string filePath);
        int ID; //Id if the joint
        //Joint* parent; //Pointer to the parent joint
        std::string name; //Name of the joint
        std::vector<Joint*> children = std::vector<Joint*>(); //List of all the joints children

    private:
        void splitStringIntoFloatVetor(const std::string &s, char delim, std::vector<float> &elems);
        Matrix4D position;
        Matrix4D rotation;
        Matrix4D scale;

};

