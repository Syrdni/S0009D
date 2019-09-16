#pragma once
//------------------------------------------------------------------------------
/**
	Application class used for example application.
	
	(C) 2015-2018 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "core/app.h"
#include "render/window.h"
#include "GraphicsNode.h"
#include <cstring>
#include <iostream>
#include "LightingNode.h"
#include "SoftwareRenderer.h"
#include <algorithm>

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
	Vector4D& getCameraposition();

private:
	GraphicsNode GN;
	GraphicsNode GN2;
	GraphicsNode GN3;
	SoftwareRenderer renderer;
	float cameraSpeed = 10.0f;

	//things to make the camera move
	Vector4D cameraPos = Vector4D(0.0f, 50.0f, 0.0f, 0.0f);
	Vector4D cameraFront = Vector4D(0.0f, 0.0f, -1.0f, 1.0f);
	Vector4D cameraUp = Vector4D(0.0f, 1.0f, 0.0f, 1.0f);
	Matrix4D position;
	Matrix4D position2;
	bool click = false;
	bool firstMouse;
	float lastX;
	float lastY;
	float yaw = -90.0f;
	float pitch = 0;
	
	
	Display::Window* window;
};
} // namespace Example