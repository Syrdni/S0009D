#include "MeshResource.h"
#include "TextureResource.h"
#include "ShaderObject.h"
#include "joint.h"
#include "tinyxml.h"

class AnimatedModel
{
    public:
        AnimatedModel();
        void buildJointTreeWithXML(std::string filePath);

    private:
        MeshResource mesh;
        TextureResource texture;
        ShaderObject shader;
        Joint* rootJoint;
        void splitStringIntoFloatVetor(const std::string &s, char delim, std::vector<float> &elems);
        //Animator?

};