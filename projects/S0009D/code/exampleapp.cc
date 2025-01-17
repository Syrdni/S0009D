//------------------------------------------------------------------------------
// exampleapp.cc
// (C) 2015-2018 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "exampleapp.h"
#include <iostream>
#include "core/app.h"
#include <chrono>
#include <thread>
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"
#define PI 3.14159265359
using namespace Display;

DebugManager *DebugManager::instance = 0;
Vector4D Rigidbody::gravitationDirection = Vector4D(0, 0, 0, 1);
float Rigidbody::gravidationPower = 0;

namespace Example
{
//------------------------------------------------------------------------------
/**
*/
	 GLuint vertexShader;
	 GLuint fragmentShader;
	 GLuint program;
	 GLuint indexBuffer;
	 GLuint vertexArray;
	 GLuint VBO;
	 Matrix4D lookAt;
	 Matrix4D perspectiveProjection;

ExampleApp::ExampleApp()
{
}

//------------------------------------------------------------------------------
/**
*/
ExampleApp::~ExampleApp()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
bool
ExampleApp::Open()
{
	App::Open();
	this->window = new Display::Window;
	window->SetKeyPressFunction([this](int32 key, int32, int32 action, int32)
	{
		if (key == GLFW_KEY_ESCAPE) {
			this->window->Close();
		}
		if (key == GLFW_KEY_W){
			cameraPos = cameraPos + (cameraFront * cameraSpeed);
		}
		if (key == GLFW_KEY_S) {
			cameraPos = cameraPos - (cameraFront * cameraSpeed);
		}
		if (key == GLFW_KEY_D) {;
			cameraPos = cameraPos + (cameraFront.crossProduct(cameraUp).normalize()) * cameraSpeed;;
		}
		if (key == GLFW_KEY_A) {
			cameraPos = cameraPos - (cameraFront.crossProduct(cameraUp).normalize()) * cameraSpeed;
		}
		
	});
	window->SetMousePressFunction([this](int32 key, int32 action, int32 test) {
		if (key == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
			click = true;
		}
		if (key == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
			click = false;
			firstMouse = true;
		}
		if (key == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		{
			//Shoot ray
			int windowWidth, windowHeight;
			window->GetSize(windowWidth, windowHeight);
			//Convert screencords to worldspace
			Vector4D nds = Vector4D((mousePos[0] * 2.0f) / windowWidth -1.0f, 1.0f - (2.0f * mousePos[1]) / windowHeight, 1, 1);
			Vector4D clip = Vector4D(nds[0], nds[1], -1.0f, 1.0);
			Vector4D eye = Matrix4D::inverse(perspectiveProjection) * clip;
			eye = Vector4D(eye[0], eye[1], -1.0, 0.0f);
			Vector4D world = Matrix4D::inverse(lookAt) * eye;
			world = world.normalize();

			ray = Ray(cameraPos, world);
			//DebugManager::getInstance()->createLine(ray.getOrigin(), ray.getPoint(1), Vector4D(0, 1, 0, 1));

			float closest = 100000000;
			PointAndDistance meshIntersection;
			PointAndDistance closestIntersection;
			for (int i = 0; i < objectVector.size(); i++)
			{
				PointAndDistance PaD = ray.intersect(objectVector[i]->getAABB());
				//Check if we hit an AABB
				if(PaD.distance != -1)
				{
					meshIntersection = objectVector[i]->checkIfRayIntersects(ray);
					if (closest > meshIntersection.distance && meshIntersection.distance != -1)
					{
						o = objectVector[i];
						closest = meshIntersection.distance;
						closestIntersection = meshIntersection;
					}
				}
			}
			if (closest != 100000000)
				o->getReferenceToRigidbody().applyForce(closestIntersection.point, ray.getDirection()*30);
		}
	});
	window->SetMouseMoveFunction([this](float64 posX, float64 posY) {
		mousePos = Vector4D(posX, posY, 0, 1);

		if (click) {

			if (firstMouse)
			{
				lastX = posX;
				lastY = posY;
				firstMouse = false;
			}

			//Fixes a bug which causes the camera to be stuck in x-axis (LastX is too big)
			if (lastX > 10000)
				lastX = 0;
			

			float xoffset = posX - lastX;
			float yoffset = lastY - posY;
			lastX = posX;
			lastY = posY;

			float sensitivity = 0.3;
			xoffset *= sensitivity;
			yoffset *= sensitivity;

			yaw += xoffset;
			pitch += yoffset;


			if (pitch > 89.0f)
				pitch = 89.0f;
			if (pitch < -89.0f)
				pitch = -89.0f;

			Vector4D front;
			front[0] = cos(yaw*(PI/180)) * cos(pitch*(PI/180));
			front[1] = sin(pitch*(PI / 180));
			front[2] = sin(yaw*(PI / 180)) * cos(pitch*(PI / 180));
			front[3];
			cameraFront = front.normalize();
		}
	});

	if (this->window->Open())
	{

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		glEnable(GL_DEPTH_TEST);
		//Perspective projection
		const float n = 0.1f;
		const float f = 100000.0f;
		const float l = -0.1f;
		const float r = 0.1f;
		const float t = 0.1f;
		const float b = -0.1f;

		perspectiveProjection = Matrix4D(
			(2 * n) / (r - l), 0, ((r + l) / (r - l)), 0,
			0, (2 * n) / (t - b), ((t + b) / (t - b)), 0,
			0, 0, -((f + n) / (f - n)), -((2 * f*n) / (f - n)),
			0, 0, -1, 0
		);

		ShaderObject* so = new ShaderObject();

		TextureResource* tr = new TextureResource();
		TextureResource* tr2 = new TextureResource();
		TextureResource* tr3 = new TextureResource();

		MeshResource* mr = new MeshResource();
		mr->loadFromOBJFile("cube.obj");

		MeshResource* mr2 = new MeshResource();
		mr2->loadFromOBJFile("cat.obj");

		//The point light source
		ln = new LightingNode(Vector4D(0, 100, 3, 1), Vector4D(1.0f, 1.0f, 1.0f, 1.0f), 1);

		position = Matrix4D(1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);


		objectVector.push_back(new Object(mr, so, tr3, ln, cameraPos, "texture.jpg", Vector4D(100, 4, 100, 1), 10, true));
		objectVector.push_back(new Object(mr, so, tr, ln, cameraPos, "texture2.jpg", Vector4D(4, 4, 4, 1), 100, false));
		objectVector.push_back(new Object(mr, so, tr2, ln, cameraPos, "tex.jpg", Vector4D(3, 3, 3, 1), 40, false));
		objectVector.push_back(new Object(mr, so, tr2, ln, cameraPos, "tex.jpg", Vector4D(2, 2, 2, 1), 20, false));
		objectVector.push_back(new Object(mr, so, tr2, ln, cameraPos, "tex.jpg", Vector4D(2, 2, 2, 1), 20, false));
		objectVector.push_back(new Object(mr, so, tr2, ln, cameraPos, "tex.jpg", Vector4D(2, 2, 2, 1), 20, false));
		
		o = objectVector[0];
		objectVector[0]->getReferenceToRigidbody().setPosition(Vector4D(0, -20, 0, 1));
		objectVector[1]->getReferenceToRigidbody().setPosition(Vector4D(10, 2, 0, 1));
		objectVector[2]->getReferenceToRigidbody().setPosition(Vector4D(10, 10, 0, 1));
		objectVector[3]->getReferenceToRigidbody().setPosition(Vector4D(8, 25, 1, 1));
		objectVector[4]->getReferenceToRigidbody().setPosition(Vector4D(10, 22, 2, 1));
		objectVector[5]->getReferenceToRigidbody().setPosition(Vector4D(12, 16, -1, 1));


		for (int i = 0; i < objectVector.size(); i++)
		{
			physicsServer.addObject(objectVector[i]);
		}

		//Setup for Dear ImGui context
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui_ImplGlfwGL3_Init(window->GetWindow(), false);

		//Setup style
		ImGui::StyleColorsDark();


		return true;
	}
	return false;
}

//------------------------------------------------------------------------------
/**
*/
void
ExampleApp::Run()
{
	bool useImGuiWindow = true;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	Vector4D pos = Vector4D(0, 0, 0, 1);
	bool skip = true;

	while (this->window->IsOpen())
	{	
		this->window->Update();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//ImGui
		ImGui_ImplGlfwGL3_NewFrame();
		if (useImGuiWindow)	
		{
			if(ImGui::Button("Clear debug shapes"))
			{
				debugManager->clear();
				system("clear");
			}
			ImGui::Checkbox("Render debugShapes", debugManager->getRenderBool());
			ImGui::Checkbox("Create debugShapes", debugManager->getCreateShapes());
			ImGui::SliderFloat("CameraSpeed", &cameraSpeed, 0.01f, 10);	
			ImGui::InputFloat4("Gravitation direction", Rigidbody::gravitationDirection.getVector());
			ImGui::InputFloat("Gravitation power", &Rigidbody::gravidationPower);
			ImGui::Spacing();
			ImGui::Spacing();		
			ImGui::Spacing();
		}

		lookAt = Matrix4D::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		Matrix4D combinedMatrix = perspectiveProjection*lookAt;
		debugManager->setViewMatrix(combinedMatrix);

		for (int i = 0; i < squareVector.size(); i++)
		{
			squareVector[i].update(combinedMatrix);
		}

		debugManager->drawDebugShapes();
		debugManager->clearSingleFrameVector();

		//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

		for (int i = 0; i < objectVector.size(); i++)
		{
			objectVector[i]->setViewMatrix(combinedMatrix);
			objectVector[i]->update();
		}
		
		physicsServer.update();

		ImGui::Render();
		glFlush();
		this->window->SwapBuffers();
	}
}

void ExampleApp::mouse()
{
}

} // namespace Example