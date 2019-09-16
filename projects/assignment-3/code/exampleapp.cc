//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015-2018 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "exampleapp.h"
#include <cstring>
#include <iostream>
#include "core/app.h"

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
		-0.25f, -0.25f, 0.25f,
		 1, 1, 1,
		 0.0f, 0.0f,

		 0.25f,  -0.25f, 0.25f,
		 1, 1, 1,
		 0.0f, 1.0f,

		 0.25f,  0.25f, 0.25f,
		 1, 0, 1,
		 1.0f, 1.0f,

		 -0.25f, 0.25f, 0.25f,
		 1, 0, 1,
		 1.0f, 0.0f,

		-0.25f, -0.25f, -0.25f,
		 0, 1, 0,
		 1.0f, 0.0f,

		0.25f,  -0.25f, -0.25f,
		 0, 1, 0,
		 1.0f, 1.0f,

		 0.25f,  0.25f, -0.25f,
		 0, 0, 1,
		 0.0f, 1.0f,

		 -0.25f, 0.25f, -0.25f,
		 0, 0, 1,
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
	 Vector4D cameraRight;
	 Vector4D cameraUp;
	 Vector4D cameraDirection;
	 Vector4D cameraPos;
	 Matrix4D perspectiveProjection;

ExampleApp::ExampleApp()
{
	mesh = MeshResource(iBuffer, vertexBuffer, sizeof(iBuffer) , sizeof(vertexBuffer));
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
	window->SetKeyPressFunction([this](int32, int32, int32, int32)
	{
		this->window->Close();
	});

	if (this->window->Open())
	{

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		//Setups and binds everything in the mesh class and loads the texture.
		mesh.setup();
		mesh.bindVertexBuffer();
		mesh.bindIndexBuffer();
		tex = new TextureResource();
		tex->loadFromFile("texture.jpg");
		mesh.setupVertexAttributePointers();

		const GLint lengthOfVertexShader = std::strlen(vs);
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vs, &lengthOfVertexShader);
		glCompileShader(vertexShader);

		//ERROR LOG
		int  success;
		char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		
		const GLint lengthOfPixelShader = std::strlen(ps);
		unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &ps, &lengthOfPixelShader);
		glCompileShader(fragmentShader);

		program = glCreateProgram();
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);
		glLinkProgram(program);

		//ERROR
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(program, 512, NULL, infoLog);
			std::cout << "ERROR::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		glUseProgram(program);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		glEnable(GL_DEPTH_TEST);
		
		//Perspective projection
		float n = 0.5, f = 6, l = -0.5, r = 0.5, t = 0.5, b = -0.5;
		perspectiveProjection = Matrix4D(
			2 * n / (r - l), 0, 0, 0,
			0, 2 * n / (t - b), 0, 0,
			((r + l) / (r - l)), ((t + b) / (t - b)), -((f + n) / (f - n)), -1,
			0, 0, -((2 * f*n) / (f - n)), 0
		);



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
	float rotation = 0.01f;
	while (this->window->IsOpen())
	{	
		this->window->Update();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mesh.bindVAO();
		glUseProgram(program);

		//Used to make the camera rotate
		float radius = 1.0f;
		float camX = sin(rotation) * radius;
		float camZ = cos(rotation) * radius;

		//Calculations to create a "Look at" matrix
		cameraPos = Vector4D(camX, camX, camZ, 1);
		Vector4D cameraTarget = Vector4D(0, 0, 0, 1);
		cameraDirection = (cameraPos - cameraTarget).normalize();
		Vector4D up = Vector4D(0, 1, 0, 1);
		cameraRight = (up.crossProduct(cameraDirection)).normalize();
		cameraUp = cameraDirection.crossProduct(cameraRight);

		lookAt = Matrix4D(
			cameraRight.getFloat(0), cameraRight.getFloat(1), cameraRight.getFloat(2), 0,
			cameraUp.getFloat(0), cameraUp.getFloat(1), cameraUp.getFloat(2), 0,
			cameraDirection.getFloat(0), cameraDirection.getFloat(1), cameraDirection.getFloat(2), 0,
			0, 0, 0, 1
		) * Matrix4D(
			1, 0, 0, -cameraPos.getFloat(0),
			0, 1, 0, -cameraPos.getFloat(1),
			0, 0, 1, -cameraPos.getFloat(2),
			0, 0, 0, 1
		);

		//Used to make the cube rotate or move
		Matrix4D temp = Matrix4D::getPositionMatrix(Vector4D(sinf(rotation)*0.6, 0.1, 0, 1));
		Matrix4D rotZ = Matrix4D::rotZ(rotation);
		Matrix4D rotY = Matrix4D::rotY(rotation * 2);
		Matrix4D rotX = Matrix4D::rotX(rotation);
		Matrix4D rotV = Matrix4D::rotVector(Vector4D(1, 2, 1, 1), rotation);



		unsigned int transformLoc = glGetUniformLocation(program, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_TRUE, (Matrix4D::transpose(perspectiveProjection)*lookAt).getMatrix());

		tex->bind(0);

		glDrawElements(GL_TRIANGLES, sizeof(iBuffer), GL_UNSIGNED_INT, NULL);

		rotation += 0.01;

		mesh.unbindVAO();
		this->window->SwapBuffers();
	}
}

} // namespace Example