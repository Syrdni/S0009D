//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015-2018 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "exampleapp.h"
#include <cstring>
#include <iostream>
#include "core/app.h"
//#include <Windows.h>
#include <math.h>
#define PI 3.14159265359

struct POD{
	float color[3];
};

const GLchar* vs =
"#version 430\n"
"layout(location=0) in vec3 pos;\n"
"layout(location=1) in vec4 color;\n"
"layout(location = 2) in vec2 aTexCoord;\n"
"uniform mat4 transform;\n"
"out vec2 TexCoord;\n"
"out vec4 Color;\n"
"void main()\n"
"{\n"
"	gl_Position = transform * vec4(pos, 1.0f);\n"
"	Color = color;\n"
"	TexCoord = aTexCoord;\n"
"}\n";

const GLchar* ps =
"#version 430\n"
"out vec4 FragColor;\n"
"in vec2 TexCoord;\n"
"in vec4 Color;\n"
"uniform sampler2D ourTexture;\n"
"void main()\n"
"{\n"
" FragColor = texture(ourTexture, TexCoord);\n"
"}\n";

float vertexBuffer[]{
		-0.05f, -0.05f, 0.05f,
		 0.0f, 0.0f,

		 0.05f,  -0.05f, 0.05f,
		 0.0f, 1.0f,

		 0.05f,  0.05f, 0.05f,
		 1.0f, 1.0f,

		 -0.05f, 0.05f, 0.05f,
		 1.0f, 0.0f,

		-0.05f, -0.05f, -0.05f,
		 1.0f, 0.0f,

		0.05f,  -0.05f, -0.05f,
		 1.0f, 1.0f,

		 0.05f,  0.05f, -0.05f,
		 0.0f, 1.0f,

		 -0.05f, 0.05f, -0.05f,
		 0.0f, 0.0f
};

int iBuffer[36]{
	0, 1, 2,
	2, 3, 0,

	1, 5, 6,
	6, 2, 1,

	7, 6, 5,
	5, 4, 7,

	4, 0, 3,
	3, 7, 4,

	4, 5, 1,
	1, 0, 4,

	3, 2, 6,
	6, 7, 3,
};

using namespace Display;
namespace Example
{
//------------------------------------------------------------------------------
/**
*/
	 GLuint vertexShader;
	 GLuint fragmentShader;
	 GLuint program;
	 GLuint indexBuffer;
	 GLuint vertexArray;
	 GLuint VBO;
	 Matrix4D perspectiveProjection;
	 TextureResource* tr;
	 TextureResource tr2;
	 int width;
	 int height;
	 Matrix4D lookAt;

ExampleApp::ExampleApp()
{
	//mesh = MeshResource(iBuffer, vertexBuffer, sizeof(iBuffer) , sizeof(vertexBuffer));
}

//------------------------------------------------------------------------------
/**
*/
ExampleApp::~ExampleApp()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
bool
ExampleApp::Open()
{
	App::Open();
	this->window = new Display::Window;
	window->SetKeyPressFunction([this](int32 key, int32, int32, int32)
	{
		if (key == GLFW_KEY_ESCAPE) {
			this->window->Close();
		}
		if (key == GLFW_KEY_W){
			//position[11] += cameraSpeed;
			cameraPos = cameraPos + (cameraFront * cameraSpeed);
			//lookAt = Matrix4D::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		}
		if (key == GLFW_KEY_S) {
			//position[11] -= cameraSpeed;
			cameraPos = cameraPos - (cameraFront * cameraSpeed);
			//lookAt = Matrix4D::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		}
		if (key == GLFW_KEY_D) {
			//position[3] += cameraSpeed;
			cameraPos = cameraPos + (cameraFront.crossProduct(cameraUp).normalize()) * cameraSpeed;
			//lookAt = Matrix4D::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		}
		if (key == GLFW_KEY_A) {
			//position[3] -= cameraSpeed;
			cameraPos = cameraPos - (cameraFront.crossProduct(cameraUp).normalize()) * cameraSpeed;
			//lookAt = Matrix4D::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		}
	});
	window->SetMousePressFunction([this](int32 key, int32 action, int32) {
		if (key == GLFW_MOUSE_BUTTON_LEFT, action == GLFW_PRESS) {
			click = true;
		}
		if (key == GLFW_MOUSE_BUTTON_LEFT, action == GLFW_RELEASE) {
			click = false;
			firstMouse = true;
		}

	});
	window->SetMouseMoveFunction([this](float64 posX, float64 posY) {
		if (click) {

			if (firstMouse)
			{
				lastX = posX;
				lastY = posY;
				firstMouse = false;
			}

			float xoffset = posX - lastX;
			float yoffset = lastY - posY;
			lastX = posX;
			lastY = posY;

			float sensitivity = 0.1;
			xoffset *= sensitivity;
			yoffset *= sensitivity;

			yaw += xoffset;
			pitch += yoffset;

			if (pitch > 89.0f)
				pitch = 89.0f;
			if (pitch < -89.0f)
				pitch = -89.0f;

			Vector4D front;
			front[0] = cos(yaw*(PI/180)) * cos(pitch*(PI/180));
			front[1] = sin(pitch*(PI / 180));
			front[2] = sin(yaw*(PI / 180)) * cos(pitch*(PI / 180));
			front[3];
			cameraFront = front.normalize();
			//lookAt = Matrix4D::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
			//lookAt.print();
		}
	});

	if (this->window->Open())
	{

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		glEnable(GL_DEPTH_TEST);
		
		//Perspective projection
		const float n = 0.01f;
		const float f = 100000.0f;
		//const float l = -0.1f;
		//const float r = 0.1f;
		//const float t = 0.1f;
		//const float b = -0.1f;
		const float l = -0.8f;
		const float r = 0.8f;
		const float t = 0.8f;
		const float b = -0.8f;

		perspectiveProjection = Matrix4D(
			(2 * n) / (r - l), 0, ((r + l) / (r - l)), 0,
			0, (2 * n) / (t - b), ((t + b) / (t - b)), 0,
			0, 0, -((f + n) / (f - n)), -((2 * f*n) / (f - n)),
			0, 0, -1, 0
		);

		ShaderObject* so = new ShaderObject();
		tr = new TextureResource();
		tr->generaterTextureHandle();
		tr2 = TextureResource();
		TextureResource* tr3 = new TextureResource();
		MeshResource* mr = new MeshResource();
		MeshResource* mr2 = new MeshResource();

		//The point light source
		LightingNode* ln = new LightingNode(Vector4D(0, -300, 0, 1), Vector4D(1.0f, 1.0f, 1.0f, 1.0f), 1);

		position = Matrix4D(1, 0, 0, 150,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);

		position2 = Matrix4D(1, 0, 0, -150,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);

		tr2.loadFromFile("tex.jpg");

		so->loadFragmentShader("fragmentShader2.txt");
		mr->loadFromOBJFile("quad.obj");
		mr2->loadFromOBJFile("dog.obj");

		GN.setMeshResource(mr);
		GN.setShaderObject(so);


		//Sets and loads all the things we need to create the object,
		width = 1000;
		height = 1000;

		renderer.addVertexBuffer(mr2->getVertex());
		renderer.addIndexBuffer(mr2->getIndex());
		renderer.setupFrameBuffer(width, height);
		renderer.setTexture(tr2);

		auto vertexShader = [](Vertex vertex, Matrix4D transform, Matrix4D modelMatrix) -> Vertex {
			Vector4D position = transform * Vector4D(vertex.pos[0], vertex.pos[1], vertex.pos[2], 1.0f);
			Vector4D normal =  modelMatrix * Vector4D(vertex.normal[0], vertex.normal[1], vertex.normal[2], 1.0f);
			Vector4D uv = modelMatrix * Vector4D(vertex.uv[0], vertex.uv[1], 1.0, 1.0);

			Vertex returnVertex;
			returnVertex.pos[0] = position[0];
			returnVertex.pos[1] = position[1];
			returnVertex.pos[2] = position[2];

			returnVertex.uv[0] = vertex.uv[0];
			returnVertex.uv[1] = vertex.uv[1];

			returnVertex.normal[0] = normal[0];
			returnVertex.normal[1] = normal[1];
			returnVertex.normal[2] = normal[2];

			return returnVertex;
		};
		auto fragmentShader = [](Vertex vertex, Vector4D cameraPosition, TextureResource texture, int width, int height) -> Vector4D {
			
			Vector4D lightPosition = Vector4D(0, 200, 0, 1);
			Vector4D lightColor = Vector4D(1, 1, 1, 1);
			Vector4D position = Vector4D(vertex.pos[0], vertex.pos[1], vertex.pos[2], 1);
			Vector4D normal = Vector4D(vertex.normal[0], vertex.normal[1], vertex.normal[2], 1);
			float * tempColor = texture.getUVtexture(vertex.uv[0], vertex.uv[1]);

			Vector4D color = { tempColor[0], tempColor[1], tempColor[2], 1 };
			//Ta bort pointer frpn getUVtexture
			delete tempColor;
			//Ambient light
			Vector4D ambientLight = Vector4D(0.01f, 0.01f, 0.01f, 1);

			//Diffuse light
			Vector4D posToLightDirVec = (lightPosition - position).normalize();
			Vector4D diffuseColor = lightColor;
			float diffuse = posToLightDirVec.dotProduct(normal);

			//"clamp"
			if (diffuse > 1)
			{
				diffuse = 1;
			}
			else if (diffuse < 0)
			{
				diffuse = 0;
			}
			Vector4D diffuseFinal = diffuseColor * diffuse;

			//Specular light
			Vector4D lightToPosDirVec = (lightPosition - position).normalize();
			Vector4D posToViewVec = (cameraPosition - position);
			Vector4D specularFinal;
			if (normal.dotProduct(lightToPosDirVec) < 0.0)
			{
				specularFinal = Vector4D(0.0, 0.0, 0.0, 1);
			}
			else
			{
				Vector4D halfWayVector = (lightToPosDirVec + posToViewVec).normalize();
				float specularConstant = normal.dotProduct(halfWayVector);

				//"std::max"

				//if (specularConstant < 0.0)
				//{
				//	specularConstant = 0;
				//}

				specularConstant = std::pow(specularConstant, 8);
				//"Clamp"

				if (specularConstant > 1)
				{
					specularConstant = 1;
				}
				else if (specularConstant < 0)
				{
					specularConstant = 0;
				}

				specularFinal = lightColor * specularConstant * 0.25;
			}
			return Vector4D(color[0] * (ambientLight[0]+diffuseFinal[0]+specularFinal[0]), color[1] * (ambientLight[1] + diffuseFinal[1] + specularFinal[1]), color[2] * (ambientLight[2] + diffuseFinal[2] + specularFinal[2]), 1);
		};

		renderer.setVertexShader(vertexShader);
		renderer.setPixelShader(fragmentShader);

		GN.setTextureResource(tr);
		GN.setlightingNode(ln);
		GN.preDrawSetup();
		renderer.setCameraPosition(cameraPos);
		return true;
	}
	return false;
}

//------------------------------------------------------------------------------
/**
*/
void
ExampleApp::Run()
{
	// position[11] = -2.0f;
	float rotation = 0.01f;
	while (this->window->IsOpen())
	{	
		rotation += 0.01;
		this->window->Update();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lookAt = Matrix4D::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		renderer.setModelMatrix(Matrix4D());
		renderer.setMVPMatrix(perspectiveProjection*lookAt);
		renderer.rasterize();
		tr->loadFromArray(renderer.getFrameBufferPointer(), width, height);

		//GN.setTransform(perspectiveProjection*lookAt);
		//GN.setPosition(Matrix4D());

		GN.draw();
		renderer.clearFrameBuffer();

		glFlush();

		this->window->SwapBuffers();
	}
}

void ExampleApp::mouse()
{
}

Vector4D & ExampleApp::getCameraposition()
{
	return cameraPos;
}

} // namespace Example