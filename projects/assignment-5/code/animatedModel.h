#include "joint.h"
#include "tinyxml.h"
#include "quaternion.h"
#include "animator.h"

class AnimatedModel
{
    public:
        AnimatedModel();
        ~AnimatedModel();
        void buildJointTreeWithXML(std::string filePath);
        void jointDrawSetup(MeshResource* mr, TextureResource* tr, ShaderObject* so, LightingNode* ln, Vector4D cameraPos, std::string texture);
        void draw(Matrix4D mat);
        void drawLines(Matrix4D mat);
        void moveJointPosition(Matrix4D mat, std::string name);

    private:
        MeshResource mesh;
        TextureResource texture;
        ShaderObject shader;
        Joint* rootJoint;
        Animator animator;
        void splitStringIntoFloatVetor(const std::string &s, char delim, std::vector<float> &elems);
        void quaternionToEuler(std::vector<float> &quaternion);

};