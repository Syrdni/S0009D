#version 430
layout(location=0) in vec3 pos;
layout(location=0) out vec4 Color;
uniform mat4 aModelMatrix;
uniform mat4 aPerspectiveTransform;
uniform vec4 aColor;
void main()
{
	gl_Position = aPerspectiveTransform * aModelMatrix * vec4(pos, 1);
	Color = aColor;
}