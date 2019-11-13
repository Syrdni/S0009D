#include "Square.h"
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

Square::Square(Vector4D pos, Vector4D normal, float w, float h, Vector4D rgb)
{
    this->position = pos;
    this->width = w;
    this->height = h;
    this->color = rgb;
    this->normal = normal.normalize();
    this->plane = mPlane(pos, normal);

    vertices[0] = vertices[0] * w;
    vertices[1] = vertices[1] * h;
    vertices[2] = vertices[2] * w;
    vertices[3] = vertices[3] * h;
    vertices[4] = vertices[4] * w;
    vertices[5] = vertices[5] * h;
    vertices[6] = vertices[6] * w;
    vertices[7] = vertices[7] * h;

    point1 = Vector4D(vertices[0], vertices[1], 0, 1);
    point2 = Vector4D(vertices[2], vertices[3], 0, 1);
    point3 = Vector4D(vertices[4], vertices[5], 0, 1);
    point4 = Vector4D(vertices[6], vertices[7], 0, 1);
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
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*6, &indices, GL_STATIC_DRAW);

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

void Square::drawSquare(Matrix4D viewMatrix)
{
    glUseProgram(program);

    unsigned int transformLoc = glGetUniformLocation(program, "aPerspectiveTransform");
    glUniformMatrix4fv(transformLoc, 1, GL_TRUE, viewMatrix.getMatrix());

    transformLoc = glGetUniformLocation(program, "aModelMatrix");
    glUniformMatrix4fv(transformLoc, 1, GL_TRUE, (Matrix4D::getPositionMatrix(position)* Matrix4D::rotationDir(normal)).getMatrix());

    transformLoc = glGetUniformLocation(program, "aColor");
    glUniform4fv(transformLoc, 1, color.getVector());

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, sizeof(int)*6, GL_UNSIGNED_INT, NULL);
    glBindVertexArray(0);
    glUseProgram(0);
}

void Square::update(Matrix4D viewMatrix)
{
    this->plane.setPosition(position);
    this->plane.setNormal(normal);
    this->drawSquare(viewMatrix);
}

mPlane Square::getPlane()
{
    return plane;
}

mPlane& Square::getReferenceToPlane()
{
    return plane;
}

Vector4D& Square::getPosition()
{
    return position;
}

Vector4D& Square::getNormal()
{
    return normal;
}

Vector4D& Square::getColor()
{
    return color;
}

float Square::checkIfHit(Ray ray)
{
    PointAndDistance result = ray.intersect(plane);

    if (result.distance == -1)
        return -1;

    Vector4D temp = result.point;
    temp[3] = 1;
    temp = Matrix4D::inverse(Matrix4D::getPositionMatrix(position)* Matrix4D::rotationDir(normal)) * temp;

    //Normals for the sides of the square
    Vector4D normal1 = point1 - point2;
    Vector4D normal2 = point2 - point1;
    Vector4D normal3 = point2 - point3;
    Vector4D normal4 = point3 - point2;

    //Calculate vector to the point from the corners
    Vector4D intersectionVector1 = temp - point1;
    Vector4D intersectionVector2 = temp - point2;
    Vector4D intersectionVector3 = temp - point3;
    Vector4D intersectionVector4 = temp - point4;

    //Check if we are inside the area
    if (intersectionVector1.dotProduct(normal3) < 0 &&
        intersectionVector3.dotProduct(normal4) < 0 &&
        intersectionVector2.dotProduct(normal2) < 0 &&
        intersectionVector4.dotProduct(normal1) < 0)
    {
        DebugManager::getInstance()->createCube(result.point, 0.05, 0.05, 0.05, Vector4D(1, 0, 0, 1));
        return result.distance;
    }
    else
    {
        return -1;
    }
}