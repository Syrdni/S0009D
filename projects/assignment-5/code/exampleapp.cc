//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015-2018 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "exampleapp.h"
#include <cstring>
#include <iostream>
#include "core/app.h"
#include <math.h>
#define PI 3.14159265359

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
	 Matrix4D lookAt;
	 Matrix4D perspectiveProjection;

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
			cameraPos = cameraPos + (cameraFront * cameraSpeed);
		}
		if (key == GLFW_KEY_S) {
			cameraPos = cameraPos - (cameraFront * cameraSpeed);
		}
		if (key == GLFW_KEY_D) {;
			cameraPos = cameraPos + (cameraFront.crossProduct(cameraUp).normalize()) * cameraSpeed;;
		}
		if (key == GLFW_KEY_A) {
			cameraPos = cameraPos - (cameraFront.crossProduct(cameraUp).normalize()) * cameraSpeed;
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

			//Fixes a bug which causes the camera to be stuck in x-axis (LastX is too big)
			if (lastX > 10000)
				lastX = 0;
			

			float xoffset = posX - lastX;
			float yoffset = lastY - posY;
			lastX = posX;
			lastY = posY;

			float sensitivity = 0.3;
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
		}
	});

	if (this->window->Open())
	{

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		glEnable(GL_DEPTH_TEST);
		
		//Perspective projection
		const float n = 0.1f;
		const float f = 100000.0f;
		const float l = -0.1f;
		const float r = 0.1f;
		const float t = 0.1f;
		const float b = -0.1f;

		perspectiveProjection = Matrix4D(
			(2 * n) / (r - l), 0, ((r + l) / (r - l)), 0,
			0, (2 * n) / (t - b), ((t + b) / (t - b)), 0,
			0, 0, -((f + n) / (f - n)), -((2 * f*n) / (f - n)),
			0, 0, -1, 0
		);

		ShaderObject* so = new ShaderObject();

		TextureResource* tr = new TextureResource();
		TextureResource* tr2 = new TextureResource();
		TextureResource* tr3 = new TextureResource();

		MeshResource* mr = new MeshResource();
		MeshResource* mr2 = new MeshResource();

		//The point light source
		LightingNode* ln = new LightingNode(Vector4D(0, 100, 0, 1), Vector4D(1.0f, 1.0f, 1.0f, 1.0f), 1);

		position = Matrix4D(1, 0, 0, 150,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);

		position2 = Matrix4D(1, 0, 0, -150,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);

		mr->loadFromOBJFile("dog.obj");
		mr2->loadFromOBJFile("sphere.obj");


		//Sets and loads all the things we need to create the object,
		GN.setMeshResource(mr);
		GN.setShaderObject(so);
		GN.setTextureResource(tr);
		GN.setlightingNode(ln);
		GN.setCameraPosition(cameraPos);
		GN.loadTexture("texture.jpg");
		GN.preDrawSetup();


		GN2.setMeshResource(mr2);
		GN2.setShaderObject(so);
		GN2.setTextureResource(tr2);
		GN2.setlightingNode(ln);
		GN2.setCameraPosition(cameraPos);
		GN2.loadTexture("tex.jpg");
		GN2.preDrawSetup();

		GN3.setMeshResource(mr);
		GN3.setShaderObject(so);
		GN3.setTextureResource(tr3);
		GN3.setlightingNode(ln);
		GN3.setCameraPosition(cameraPos);
		GN3.loadTexture("texture2.jpg");
		GN3.preDrawSetup();
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
		this->window->Update();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		rotation += 0.01;

		Matrix4D rotY = Matrix4D::rotY(rotation);

		lookAt = Matrix4D::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		GN.setTransform(perspectiveProjection*lookAt*rotY);
		GN.setPosition(rotY);

		GN2.setTransform(perspectiveProjection*lookAt*(position*rotY));
		GN2.setPosition(position*rotY);

		GN3.setTransform(perspectiveProjection*lookAt*(position2*rotY));
		GN3.setPosition(position2*rotY);

		GN.draw();
		GN2.draw();
		GN3.draw();

		glFlush();

		this->window->SwapBuffers();
	}
}

void ExampleApp::mouse()
{
}

} // namespace Example