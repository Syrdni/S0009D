#include "debugLine.h"

const GLchar* vShader =
"#version 430\n"
"layout(location=0) in vec3 pos;\n"
"layout(location=0) out vec4 Color;\n"
"uniform mat4 aModelMatrix;\n"
"uniform mat4 aPerspectiveTransform;\n"
"uniform vec4 aColor;\n"
"void main()\n"
"{\n"
"	gl_Position = aPerspectiveTransform * aModelMatrix * vec4(pos, 1);\n"
"	Color = aColor;\n"
"}\n";

const GLchar* pShader =
"#version 430\n"
"layout(location=0) in vec4 color;\n"
"out vec4 Color;\n"
"void main()\n"
"{\n"
"	Color = color;\n"
"}\n";

DebugLine::DebugLine(){}

DebugLine::DebugLine(Vector4D point1, Vector4D point2)
{
    vertices.push_back(point1[0]);
    vertices.push_back(point1[1]);
    vertices.push_back(point1[2]);
    vertices.push_back(point2[0]);
    vertices.push_back(point2[1]);
    vertices.push_back(point2[2]);

    indices.push_back(0);
    indices.push_back(1);

    setup();
}

DebugLine::~DebugLine(){}

void DebugLine::draw(Matrix4D viewMatrix)
{
    glUseProgram(program);

    unsigned int transformLoc = glGetUniformLocation(program, "aPerspectiveTransform");
    glUniformMatrix4fv(transformLoc, 1, GL_TRUE, viewMatrix.getMatrix());

    transformLoc = glGetUniformLocation(program, "aModelMatrix");
    glUniformMatrix4fv(transformLoc, 1, GL_TRUE, Matrix4D().getMatrix());

    transformLoc = glGetUniformLocation(program, "aColor");
    glUniform4fv(transformLoc, 1, Vector4D(0.0, 1.0, 0.0, 1.0).getVector());

    glBindVertexArray(VAO);
    glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, NULL);
    glBindVertexArray(0);
    glUseProgram(0);
}

void DebugLine::setup()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size(), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*indices.size(), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, NULL);
    glBindVertexArray(0);

        //Shaders
    //Setup vertex shader
    this->vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLint length = static_cast<GLint>(strlen(vShader));
	glShaderSource(this->vertexShader, 1, &vShader, &length);
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
    length = static_cast<GLint>(strlen(pShader));
    glShaderSource(this->pixelShader, 1, &pShader, &length);
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