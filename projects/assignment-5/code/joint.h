#pragma once
#include <list>
#include <string>
#include "Matrix4D.h"
#include "tinyxml.h"
#define TIXML_USE_STL

class Joint
{
    public:
        Joint(int id, Joint* parent, std::string name);
        Joint();
        ~Joint();
        void addChild(Joint child);
        void buildJointTreeWithXML(std::string filePath);
        void buildJointTree(TiXmlDocument doc);
        int ID; //Id if the joint
        Joint* parent; //Pointer to the parent joint
        std::string name; //Name of the joint
        std::list<Joint> children = std::list<Joint>(); //List of all the joints children

    private:
        Matrix4D transform;

};

