#include "debugShape.h"

DebugShape::DebugShape()
{
    debugMessageSent = false;
}

void DebugShape::draw(Matrix4D viewMatrix)
{
    if (!debugMessageSent)
    {
        std::cout << "Draw function missing" << std::endl;
        debugMessageSent = true;
    }
}

void DebugShape::setup(){}

std::string DebugShape::setupFragmentShader()
{
	std::ifstream file;
	file.open("debugFragmentShader.frag");

	if (file.fail()) {
		std::cout << "Failed to load fragmentShader" << std::endl;
        return 0;
	}
	else {
		std::stringstream tempstream;
		tempstream << file.rdbuf();
		std::string temp = tempstream.str();
		return temp;
	}
}

std::string DebugShape::setupVertexShader()
{
	std::ifstream file;
	file.open("debugVertexShader.vert");

	if (file.fail()) {
		std::cout << "Failed to load fragmentShader" << std::endl;
        return 0;
	}
	else {
		std::stringstream tempstream;
		tempstream << file.rdbuf();
		std::string temp = tempstream.str();
		return temp;
	}
}