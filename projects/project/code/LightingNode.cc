#include "LightingNode.h"

LightingNode::LightingNode(Vector4D position, Vector4D color, float intensity)
{
	this->position = position;
	this->color = color;
	this->intensity = intensity;
}

LightingNode::LightingNode()
{
	this->position = Vector4D();
	this->color = Vector4D();
	this->intensity = 0;
}

LightingNode::~LightingNode()
{
}

void LightingNode::setup()
{

}

Vector4D LightingNode::getPosition()
{
	return position;
}

Vector4D LightingNode::getColor()
{
	return color;
}

Vector4D LightingNode::getColorWithIntensity()
{
	return color * intensity;
}
