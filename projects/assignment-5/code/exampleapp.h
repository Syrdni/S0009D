#pragma once
//------------------------------------------------------------------------------
/**
	Application class used for example application.
	
	(C) 2015-2018 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "core/app.h"
#include "render/window.h"
#include "graphics/GraphicsNode.h"
#include <iostream>
#include "LightingNode.h"
#include "animations/joint.h"
#include "animations/animatedModel.h"
#include <cmath>
#include "animations/animator.h"
#include "Square.h"
#include "Ray.h"
#include "debug/debugManager.h"
#include "debug/debugLine.h"

namespace Example
{
class ExampleApp : public Core::App
{
public:
	/// constructor
	ExampleApp();
	/// destructor
	~ExampleApp();

	/// open app
	bool Open();
	/// run app
	void Run();

	void mouse();


private:
	GraphicsNode GN;
	GraphicsNode GN2;
	GraphicsNode GN3;
	AnimatedModel* am;
	Animator an;

	Square s;

	float cameraSpeed = 0.05f;

	//things to make the camera move
	Vector4D cameraPos = Vector4D(0.0f, 0.0f, 3.0f, 1.0f);
	Vector4D cameraFront = Vector4D(0.0f, 0.0f, -1.0f, 1.0f);
	Vector4D cameraUp = Vector4D(0.0f, 1.0f, 0.0f, 1.0f);
	Matrix4D position;
	Matrix4D position2;
	bool click = false;
	bool firstMouse = true;
	float lastX;
	float lastY;
	float yaw = 0;
	float pitch = 0;
	LightingNode* ln;
	
	Display::Window* window;

	Ray ray;
	Vector4D mousePos;

	DebugManager debugManager;
};
} // namespace Example