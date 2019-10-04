#version 430
layout(location=0) in vec3 pos;
layout(location = 1) in vec4 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec4 aTangent;
layout(location = 4) in vec4 aBinormal;
layout(location = 5) in vec4 aSkinWeights;
layout(location = 6) in vec4 aSkinJindices;

out vec2 TexCoord;
out vec4 OutNormal;
out vec4 CameraPosition;
out vec4 Tangent;
out vec4 Binormal;
out mat3 TBN;

uniform mat4 transform;
uniform vec4 cameraPosition;

void main()
{
	gl_Position = transform * vec4(pos, 1.0f);
    TexCoord = aTexCoord;
    OutNormal = aNormal;
    CameraPosition = cameraPosition;
    Tangent = aTangent;
    Binormal = aBinormal;

   vec3 T = normalize(vec3(mat4(1) * aTangent));
   vec3 B = normalize(vec3(mat4(1) * aBinormal));
   vec3 N = normalize(vec3(mat4(1) * aNormal));
   //vec3 B = cross(N, T);
   TBN = mat3(T, B, N);
}
