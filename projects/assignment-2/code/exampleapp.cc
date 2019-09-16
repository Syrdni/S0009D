//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015-2018 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "exampleapp.h"
#include <cstring>
#include <iostream>

//"	gl_Position = vec4(pos, 1);\n"
const GLchar* vs =
"#version 430\n"
"layout(location=0) in vec3 pos;\n"
"layout(location=1) in vec4 color;\n"
"layout(location=0) out vec4 Color;\n"
"uniform mat4 transform;\n"
"void main()\n"
"{\n"
"gl_Position = transform * vec4(pos, 1.0f);\n"
"	Color = color;\n"
"}\n";

const GLchar* ps =
"#version 430\n"
"layout(location=0) in vec4 color;\n"
"out vec4 Color;\n"
"void main()\n"
"{\n"
"	Color = color;\n"
"}\n";

float vertexBuffer[24]{
	-0.5f, -0.5f, 0,
	 1, 0, 0,
	-0.5f,  0.5f, 0,
	 0, 1, 0,
	 0.5f,  0.5f, 0,
	 0, 0, 1,
	 0.5f, -0.5f, 0,
	 0, 0, 0
};

int iBuffer[6]{
0, 1, 3,
2, 1, 3
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

ExampleApp::ExampleApp()
{
	mesh = MeshResource(iBuffer, vertexBuffer, 6, 24);
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

		mesh.setup();
		mesh.bindVertexBuffer();
		mesh.bindIndexBuffer();
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
		glClear(GL_COLOR_BUFFER_BIT);

		mesh.bindVAO();
		glUseProgram(program);

		Matrix4D temp = Matrix4D::getPositionMatrix(Vector4D(sinf(rotation), 0.1, 0, 1));
		Matrix4D rot = Matrix4D::rotZ(rotation);
		

		unsigned int transformLoc = glGetUniformLocation(program, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_TRUE, (temp * rot).getMatrix());

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

		rotation += 0.01;
		mesh.unbindVAO();
		this->window->SwapBuffers();
	}
}

} // namespace Example