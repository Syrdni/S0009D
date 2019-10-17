#version 430
in vec2 TexCoord;
in vec4 OutNormal;
//in vec3 Pos;
in vec4 Tangent;
in vec4 Binormal;

//in vec4 LightPosition;
//in vec4 LightColor;
in vec4 CameraPosition;
in mat3 TBN;
in vec3 Pos;

out vec4 FragColor;

uniform sampler2D diffuseTexture;
uniform sampler2D normalMap;

void main()
{    
    vec4 LightPosition = vec4(0, 5, 0, 1);
    vec4 LightColor = vec4(1.0, 1.0, 1.0, 1.0);

	vec3 Normal = texture(normalMap, TexCoord).rgb;
    Normal = normalize(Normal * 2.0 - 1.0);
    Normal = normalize(TBN * Normal);

	//Ambient light
	vec3 ambientLight = vec3(0.01f, 0.01f, 0.01f);

	//Diffuse light
	vec3 posToLightDirVec = normalize(vec3(LightPosition) - Pos);					
	vec3 diffuseColor = vec3(LightColor);											
	float diffuse = clamp(dot(posToLightDirVec, Normal), 0, 1);
	vec3 diffuseFinal = diffuseColor * diffuse;
	
	//Specular light
	vec3 lightToPosDirVec = normalize(vec3(LightPosition) - Pos);
	vec3 posToViewVec = normalize(vec3(CameraPosition) - Pos);
	vec3 specularFinal;
	if(dot(Normal, lightToPosDirVec) < 0.0)
	{
		specularFinal = vec3(0.0, 0.0, 0.0);
	}
	else
	{
		vec3 halfWayVector = normalize(lightToPosDirVec + posToViewVec);
		float specularConstant = clamp(pow(max(dot(Normal, halfWayVector), 0.0), 16),0,1);
		specularFinal = vec3(LightColor) * specularConstant;		
	}							

	//Final light
 	FragColor = texture(diffuseTexture, TexCoord) * (vec4(ambientLight, 1.f) + vec4(diffuseFinal, 1.0f) + vec4(specularFinal, 1.0f));
}
