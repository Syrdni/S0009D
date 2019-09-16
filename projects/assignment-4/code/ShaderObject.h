#pragma once
#include "core/app.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "Vector4D.h"

class ShaderObject {
public:
	ShaderObject();
	~ShaderObject();
	bool loadVertexShader(const char* filename);
	bool loadFragmentShader(const char* filename);
	void useProgram();
	void modifyMatrix4fvUniform(std::string name, float* mat);
	void modifyVector4fvUniform(std::string name, Vector4D vec);
	void compileShader();

private:
	const char* vs;
	const char* fs;
	unsigned int program;
	void linkVertexShader();
	void linkFragmentShader();
	unsigned int vertexShader;
	unsigned int fragmentShader;
};
