#version 430
layout(location=0) in vec3 pos;
layout(location = 1) in vec4 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec4 aTangent;
layout(location = 4) in vec4 aBinormal;
layout(location = 5) in vec4 aSkinWeights;
layout(location = 6) in vec4 aSkinJindices;

out vec3 Pos;
out vec2 TexCoord;
out vec4 OutNormal;
out vec4 CameraPosition;
out vec4 Tangent;
out vec4 Binormal;
out mat3 TBN;

uniform mat4 transform;
uniform vec4 cameraPosition;
uniform mat4 jointTransforms[21];
uniform mat4 modelMatrix;
uniform bool isPlaying;

void main()
{
    vec4 totalNormal;
    if(isPlaying)
    {
        vec4 normWeights = aSkinWeights / dot(aSkinWeights, vec4(1.0));
        mat4x4 joint =  jointTransforms[int(aSkinJindices[0])] * normWeights[0] +
                        jointTransforms[int(aSkinJindices[1])] * normWeights[1] + 
                        jointTransforms[int(aSkinJindices[2])] * normWeights[2] + 
                        jointTransforms[int(aSkinJindices[3])] * normWeights[3];
        gl_Position = transform * modelMatrix * (joint * vec4(pos, 1.0));
        totalNormal = inverse(transpose(joint)) * aNormal;
    }
    else
    {
        gl_Position = transform * modelMatrix * vec4(pos, 1.0);
        totalNormal = aNormal;
    }

    mat3 rotMat = mat3(inverse(transpose(modelMatrix)));

    vec3 T = normalize(vec3(rotMat * aTangent.xyz));
    vec3 N = normalize(vec3(rotMat * totalNormal.xyz));
    vec3 B = normalize(vec3(rotMat * aBinormal.xyz));
    TBN = mat3(T, B, N);

    Pos = (modelMatrix * vec4(pos, 1.0)).xyz;
    TexCoord = aTexCoord;
    OutNormal =  totalNormal;
    CameraPosition = cameraPosition;
}
