#pragma once
#include "Vector4D.h"
#include "ShaderObject.h"
#include "Matrix4D.h"
class LightingNode {
public:
	LightingNode(Vector4D position, Vector4D color, float intensity);
	LightingNode();
	~LightingNode();
	void setup();
	Vector4D getPosition();
	Vector4D getColor();
	Vector4D getColorWithIntensity();
private:
	Vector4D position;
	Vector4D color;
	float intensity;
};