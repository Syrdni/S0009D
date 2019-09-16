#pragma once
//------------------------------------------------------------------------------
/**
	Application class used for example application.
	
	(C) 2015-2018 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "core/app.h"
#include "render/window.h"
#include "MeshResource.h"
#include "Matrix4D.h"
#include "TextureResource.h"
#include "GraphicsNode.h"
#include <cstring>
#include <iostream>
#define PI 3.14159265359
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
private:
	MeshResource mesh;
	TextureResource* tex;
	GraphicsNode GN;

	float cameraSpeed = 0.05f;

	//things to make the camera move
	Vector4D cameraPos = Vector4D(0.0f, 0.0f, 3.0f, 1.0f);
	Vector4D cameraFront = Vector4D(0.0f, 0.0f, -1.0f, 1.0f);
	Vector4D cameraUp = Vector4D(0.0f, 1.0f, 0.0f, 1.0f);
	Matrix4D position;
	bool click = false;
	bool firstMouse;
	float lastX;
	float lastY;
	float yaw = -90.0f;
	float pitch = 0;
	
	
	Display::Window* window;
};
} // namespace Example