#include "Ray.h"

const GLchar* vs2 =
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

const GLchar* ps2 =
"#version 430\n"
"layout(location=0) in vec4 color;\n"
"out vec4 Color;\n"
"void main()\n"
"{\n"
"	Color = color;\n"
"}\n";

Ray::Ray(){render = false;}

Ray::Ray(Vector4D pos, Vector4D dir, bool render)
{
    this->origin = pos;
    this->direction = dir;
    this->render = render;
    if (render)
    {
        setup();
    }
}

Ray::~Ray(){};

void Ray::setOrigin(Vector4D pos)
{
    this->origin = pos;
}

Vector4D Ray::getOrigin()
{
    return this->origin;
}

void Ray::setDirection(Vector4D dir)
{
    this->direction = dir;
}

Vector4D Ray::getDirection()
{
    return this->direction;
}

Vector4D Ray::intersect(mPlane plane)
{
    //Check if the ray is parallel to the plane. If that is the case the ray will never hit the plane
    float d = Vector4D::dotProduct(plane.getNormal(), this->direction);
    if (d == 0)
        return Vector4D(0,0,0,1);
    
    //Calculate t
    float t = Vector4D::dotProduct(plane.getPosition()-this->origin, plane.getNormal()) / d;

    if (t < 0)
    {
        std::cout << "Miss" << std::endl;
        return Vector4D(0,0,0,1);
    }
    

    //With t calculated we now can find where the ray is intersected with the plane
    std::cout << "Hit" << std::endl;
    return (this->origin + this->direction * t);
}

void Ray::setup()
{
    vertices.push_back(origin[0]); vertices.push_back(origin[1]); vertices.push_back(origin[2]);
    vertices.push_back(direction[0]*maxDist); vertices.push_back(direction[1]*maxDist); vertices.push_back(direction[2]*maxDist);
    indices.push_back(0); indices.push_back(1);
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
	GLint length = static_cast<GLint>(strlen(vs2));
	glShaderSource(this->vertexShader, 1, &vs2, &length);
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
    length = static_cast<GLint>(strlen(ps2));
    glShaderSource(this->pixelShader, 1, &ps2, &length);
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

void Ray::draw(Matrix4D viewMatrix)
{
    if(!render)
        return;

    glUseProgram(program);

    unsigned int transformLoc = glGetUniformLocation(program, "aPerspectiveTransform");
    glUniformMatrix4fv(transformLoc, 1, GL_TRUE, viewMatrix.getMatrix());

    transformLoc = glGetUniformLocation(program, "aModelMatrix");
    glUniformMatrix4fv(transformLoc, 1, GL_TRUE, Matrix4D().getMatrix());

    transformLoc = glGetUniformLocation(program, "aColor");
    glUniform4fv(transformLoc, 1, Vector4D(1.0, 1.0, 1.0, 1.0).getVector());

    glBindVertexArray(VAO);
    glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, NULL);
    glBindVertexArray(0);
    glUseProgram(0);
}

Vector4D Ray::getPoint(int i)
{
    if (i == 0)
        return origin;
    else
        return origin+(direction*maxDist);
}