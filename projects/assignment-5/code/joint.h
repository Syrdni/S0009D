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

        //Adds a child to the child vector
        void addChild(Joint* child);

        //Sets the joints matrixes
        void addMatrixes(Matrix4D pos, Matrix4D rot, Matrix4D sca);

        //Calculates the inverse local position for the joint
        void calcInverseLocalPosition(Matrix4D parentLocalTransform);

        //Calculates the worldspace for the joint
        void calcWorldSpace(Matrix4D parentLocalTransform);

        //Setups everything that is needed to draw the join in the scene
        void drawSetup(MeshResource* mr, TextureResource* tr, ShaderObject* so, LightingNode* ln, Vector4D cameraPos, std::string texture);

        //Draws the joint on the screne. mat is view and projection matrix
        void draw(Matrix4D mat);

        //Draws a line from this joint and to all its children
        void drawLines(Matrix4D mat);

        //Moves a joint to another position. If the join that needs to be moved isnt this joint, call the function on the children
        int movePosition(Matrix4D mat, std::string name);
        
        //The joints world position
        Matrix4D worldPosition;

        //Returns a joint with a certain id. If the id doesnt match with this joint, call this function its children
        Joint* getJoint(int id);

        //Sets the worldPosition of the joint
        void setWorldPosition(Matrix4D mat);

        //Resets the joints position to its default transform
        void reset();

        //Returns the vector that contains all the children to the joint
        std::vector<Joint*> getChildren();

        //Returns the ID of the joint
        int getID();

    private:
        void splitStringIntoFloatVetor(const std::string &s, char delim, std::vector<float> &elems);

        //Id of the joint
        int ID;

        //The inverse local position of the joint
        Matrix4D inverseLocalPosition;

        //Vector of all the children to the joint
        std::vector<Joint*> children = std::vector<Joint*>();

        Matrix4D localPosition;
        Matrix4D defaultTransform;
        Matrix4D rotation;
        Matrix4D scale;
        Matrix4D transform;
        std::string name; //Name of the joint
        GraphicsNode JGN;

};

