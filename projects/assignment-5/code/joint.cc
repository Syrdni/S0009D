#include "joint.h"

Joint::Joint(int id, Joint* parent, std::string name)
{
    this->ID = id;
    this->parent = parent;
    this->name = name;
}

Joint::~Joint()
{

}

Joint::Joint()
{

}

void Joint::addChild(Joint child)
{
    children.push_front(child);
}

void Joint::buildJointTreeWithXML(std::string filePath)
{
    TiXmlDocument doc;
    const char *path = filePath.c_str();
    //doc.Parse(path, 0, TIXML_ENCODING_UTF8);
    if (doc.LoadFile(path, TIXML_ENCODING_UTF8))
    {
        TiXmlElement *Nebula3, *Model, *CharacterNodes, *Joint;
        Nebula3 = doc.FirstChildElement("Nebula3");
        if (Nebula3)
        {
            std::cout << "Det funkade!" << std::endl;
        }    
    }
}

void Joint::buildJointTree(TiXmlDocument doc)
{

}