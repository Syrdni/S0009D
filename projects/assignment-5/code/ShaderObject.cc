#include "ShaderObject.h"
#include <cstring>

ShaderObject::ShaderObject()
{
}

ShaderObject::~ShaderObject()
{
	delete[] vs;
	delete[] fs;
}

//Loads a vertex shader from a file.
bool ShaderObject::loadVertexShader(const char* filename)
{
	std::ifstream file;
	file.open(filename);

	if (file.fail()) {
		std::cout << "Failed to load vertexShader" << std::endl;
		return false;
	}
	else {
		std::stringstream tempstream;
		tempstream << file.rdbuf();
		std::string temp = tempstream.str();
		vs = temp.c_str();
		linkVertexShader();
		file.close();
		return true;
	}

}

//Loads a fragment shader from a file.
bool ShaderObject::loadFragmentShader(const char * filename)
{
	std::ifstream file;
	file.open(filename);

	if (file.fail()) {
		std::cout << "Failed to load fragmentShader" << std::endl;
		return false;
	}
	else {
		std::stringstream tempstream;
		tempstream << file.rdbuf();
		std::string temp = tempstream.str();
		fs = temp.c_str();
		linkFragmentShader();
		file.close();
		return true;
	}
}

void ShaderObject::useProgram()
{
	glUseProgram(program);
}

//Sends a Matrix4D to the shader for it to use.
void ShaderObject::modifyMatrix4fvUniform(std::string name, Matrix4D matrix)
{
	unsigned int transformLoc = glGetUniformLocation(program, name.c_str());
	glUniformMatrix4fv(transformLoc, 1, GL_TRUE, matrix.getMatrix());
}

//Sends a Vector4D to the shader for it to use.
void ShaderObject::modifyVector4fvUniform(std::string name, Vector4D vector)
{
	unsigned int transformLoc = glGetUniformLocation(program, name.c_str());
	glUniform4fv(transformLoc, 1, vector.getVector());
}

//Attaches the shaders to the program.
void ShaderObject::compileShader()
{
	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
}

//Links the vertex shader and prints out errors if there is any.
void ShaderObject::linkVertexShader()
{
	const GLint lengthOfVertexShader = strlen(vs);
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
}

//Links the fragment shader and prints out errors if there is any.
void ShaderObject::linkFragmentShader()
{
	const GLint lengthOfPixelShader = std::strlen(fs);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fs, &lengthOfPixelShader);
	glCompileShader(fragmentShader);

	int  success;
	char infoLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}
