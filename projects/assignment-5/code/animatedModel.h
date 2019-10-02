#pragma once
#include "joint.h"
#include "tinyxml.h"
#include "quaternion.h"

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

    private:
        MeshResource mesh;
        TextureResource texture;
        ShaderObject shader;

        //The rootjoin of the joint tree
        Joint* rootJoint;

        //Splits a string into a float vector. Used to pares the values from the xml file
        void splitStringIntoFloatVetor(const std::string &s, char delim, std::vector<float> &elems);

};