#include "ShaderObject.h"

ShaderObject::ShaderObject()
{
}

ShaderObject::~ShaderObject()
{
}

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

void ShaderObject::useProgram() {
	glUseProgram(program);
}

void ShaderObject::compileShader()
{
	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
}

void ShaderObject::modifyMatrix4fvUniform(std::string name, float* matrix)
{
	unsigned int transformLoc = glGetUniformLocation(program, name.c_str());
	glUniformMatrix4fv(transformLoc, 1, GL_TRUE, matrix);
}

void ShaderObject::modifyVector4fvUniform(std::string name, Vector4D vector)
{
	unsigned int transformLoc = glGetUniformLocation(program, name.c_str());
	glUniform4fv(transformLoc, 1, vector.getVector());
}


void ShaderObject::linkVertexShader()
{
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
}

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
