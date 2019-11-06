#include "Square.h"

float vertices[] = {
    1.0f, 1.0f,
    1.0f, -1.0f,
    -1.0f, -1.0f,
    -1.0f, 1.0f
};

int indicies[] = {
    0, 1, 2,
    0, 3, 2
};

const GLchar* vs =
"#version 430\n"
"layout(location=0) in vec2 pos;\n"
"layout(location=0) out vec4 Color;\n"
"uniform mat4 aModelMatrix;\n"
"uniform mat4 aPerspectiveTransform;\n"
"uniform vec4 aColor;\n"
"void main()\n"
"{\n"
"	gl_Position = aPerspectiveTransform * aModelMatrix * vec4(pos, 0, 1);\n"
"	Color = aColor;\n"
"}\n";

const GLchar* ps =
"#version 430\n"
"layout(location=0) in vec4 color;\n"
"out vec4 Color;\n"
"void main()\n"
"{\n"
"	Color = color;\n"
"}\n";

Square::Square(){}

Square::Square(Vector4D pos, float w, float h, Vector4D rgb)
{
    this->position = pos;
    this->width = w;
    this->height = h;
    this->color = rgb;
    setupSquare();
}

Square::~Square(){}

void Square::setupSquare()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*8, &vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*6, &indicies, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, NULL);
    glBindVertexArray(0);

    //Shaders
    //Setup vertex shader
    this->vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLint length = static_cast<GLint>(strlen(vs));
	glShaderSource(this->vertexShader, 1, &vs, &length);
	glCompileShader(this->vertexShader);

    //Error log
    GLint shaderLogSize;
		glGetShaderiv(this->vertexShader, GL_INFO_LOG_LENGTH, &shaderLogSize);
		if (shaderLogSize > 0)
		{
			GLchar* buf = new GLchar[shaderLogSize];
			glGetShaderInfoLog(this->vertexShader, shaderLogSize, NULL, buf);
			printf("[SHADER COMPILE ERROR]: %s", buf);
			delete[] buf;
		}
    //Setup pixel shader
		this->pixelShader = glCreateShader(GL_FRAGMENT_SHADER);
		length = static_cast<GLint>(strlen(ps));
		glShaderSource(this->pixelShader, 1, &ps, &length);
		glCompileShader(this->pixelShader);

		//Error log
		shaderLogSize;
		glGetShaderiv(this->pixelShader, GL_INFO_LOG_LENGTH, &shaderLogSize);
		if (shaderLogSize > 0)
		{
			GLchar* buf = new GLchar[shaderLogSize];
			glGetShaderInfoLog(this->pixelShader, shaderLogSize, NULL, buf);
			printf("[SHADER COMPILE ERROR]: %s", buf);
			delete[] buf;
		}

    program = glCreateProgram();
	glAttachShader(program, vertexShader);
    glAttachShader(program, pixelShader);
	glLinkProgram(program);
}

void Square::drawSquare(Matrix4D modelMatrix, Matrix4D viewMatrix)
{
    glUseProgram(program);

    unsigned int transformLoc = glGetUniformLocation(program, "aPerspectiveTransform");
    glUniformMatrix4fv(transformLoc, 1, GL_TRUE, viewMatrix.getMatrix());

    transformLoc = glGetUniformLocation(program, "aModelMatrix");
    glUniformMatrix4fv(transformLoc, 1, GL_TRUE, modelMatrix.getMatrix());

    transformLoc = glGetUniformLocation(program, "aColor");
    glUniform4fv(transformLoc, 1, color.getVector());

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, sizeof(float)*8, GL_UNSIGNED_INT, NULL);
    glBindVertexArray(0);
    glUseProgram(0);
}