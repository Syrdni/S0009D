#include "MeshResource.h"
#include "TextureResource.h"
#include "ShaderObject.h"
#include "joint.h"
#include "tinyxml.h"

class AnimatedModel
{
    public:
        AnimatedModel();

    private:
        MeshResource mesh;
        TextureResource texture;
        ShaderObject shader;
        Joint rootJoint;
        //Animator?

};