#include "joint.h"
#include "tinyxml.h"
#include "quaternion.h"

class AnimatedModel
{
    public:
        AnimatedModel();
        void buildJointTreeWithXML(std::string filePath);
        void jointDrawSetup(MeshResource* mr, TextureResource* tr, ShaderObject* so, LightingNode* ln, Vector4D cameraPos, std::string texture);
        void draw(Matrix4D mat);

    private:
        MeshResource mesh;
        TextureResource texture;
        ShaderObject shader;
        Joint* rootJoint;
        void splitStringIntoFloatVetor(const std::string &s, char delim, std::vector<float> &elems);
        void quaternionToEuler(std::vector<float> &quaternion);
        //Animator?

};