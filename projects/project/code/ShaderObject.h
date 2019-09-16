#pragma once
#include "core/app.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "Vector4D.h"
#include "Matrix4D.h"
#include <string.h>

class ShaderObject {
public:
	ShaderObject();
	~ShaderObject();
	bool loadVertexShader(const char* filename);
	bool loadFragmentShader(const char* filename);
	void useProgram();
	void modifyMatrix4fvUniform(std::string name, Matrix4D mat);
	void modifyVector4fvUniform(std::string name, Vector4D vec);
	void compileShader();
private:
	unsigned int program;
	const char* vs;
	const char* fs;
	void linkVertexShader();
	void linkFragmentShader();
	unsigned int vertexShader;
	unsigned int fragmentShader;
};
