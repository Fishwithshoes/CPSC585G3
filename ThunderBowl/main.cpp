#include "CommonIncludes.h"
#include "Game.h"
#include "GameObject.h"
#include "Renderer.h"
#include "RendererUtility.h"
#include "Camera.h"
#include "Input.h"
#include "Physics.h"
#include "Audio.h"

// HANDLES KEYBOARD, MOUSE EVENTS AS PART OF glfwPolEvents() IN MAIN LOOP
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	for (int i = 10; i < Input::NumInputsToSet; i++)
	{
		if (key == i)
		{
			if (action == GLFW_PRESS)
				Input::SetInputValue(key, 1);
			if (action == GLFW_RELEASE)
				Input::SetInputValue(key, 0);
		}
	}
}

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (action == GLFW_PRESS)
			Input::SetInputValue(1, 1);
		else
			Input::SetInputValue(1, 0);
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		if (action == GLFW_PRESS)
			Input::SetInputValue(2, 1);
		else
			Input::SetInputValue(2, 0);
	}
	if (button == GLFW_MOUSE_BUTTON_MIDDLE)
	{
		if (action == GLFW_PRESS)
			Input::SetInputValue(4, 1);
		else
			Input::SetInputValue(4, 0);
	}
}

void MousePosCallback(GLFWwindow* window, double xpos, double ypos)
{
	Input::SetNewMousePosition((int)xpos, (int)ypos);
}

// ==========================================================================
// PROGRAM ENTRY POINT

int main(int argc, char *argv[])
{
	// initialize the GLFW windowing system
	if (!glfwInit()) {
		cout << "ERROR: GLFW failed to initilize, TERMINATING" << endl;
		return -1;
	}
	glfwSetErrorCallback(RendererUtility::ErrorCallback);

	// attempt to create a window with an OpenGL 4.5 core profile context
	GLFWwindow *window = 0;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWmonitor *monitor = glfwGetPrimaryMonitor();
	window = glfwCreateWindow(Camera::WIDTH, Camera::HEIGHT, "ThunderBowl", 0, 0);
	if (!window) {
		cout << "Program failed to create GLFW window, TERMINATING" << endl;
		glfwTerminate();
		return -1;
	}
	glfwSwapInterval(1);
	Input::SetWindowSize(Camera::WIDTH, Camera::HEIGHT);

	//Set input callbacks and make the window active
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetMouseButtonCallback(window, MouseButtonCallback);
	glfwSetCursorPosCallback(window, MousePosCallback);
	glfwMakeContextCurrent(window);

#ifdef _WIN32
	// Intialize GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		cout << "glew init failed" << endl;
		return -1;
	}
	RendererUtility::CheckGLErrors();
#endif

	RendererUtility::QueryGLVersion();

	Time::Init();
	Audio::Init();
	Physics::initializePhysX();
	Renderer renderer;
	Renderer::Init(&renderer);

	Game::BuildWorld();

	//MAIN LOOP
	while (!glfwWindowShouldClose(window))
	{
		//Start the frame
		Time::StartFrame();

		//Collect Input
		glfwPollEvents();

		//Game Logic
		for (int i = Game::staticObjectList.size() - 1; i >= 0; i--)
		{
			Game::staticObjectList[i].Update();
		}
		for (int i = Game::worldObjectList.size()-1; i >= 0; i--)
		{
			Game::worldObjectList[i].Update();
		}
		for (int i = Game::particleObjectList.size() - 1; i >= 0; i--)
		{
			Game::particleObjectList[i].Update();
		}
		for (int i = Game::overlayObjectList.size() - 1; i >= 0; i--)
		{
			Game::overlayObjectList[i].Update();
		}
		for (int i = Game::aiObjectList.size() - 1; i >= 0; i--)
		{
			Game::aiObjectList[i].Update();
		}

		Physics::stepPhysics();	//SUBJECT TO CHANGE

		Renderer::GetCamera(0)->Update();

		//Render Scene
		Renderer::RenderScene(&renderer);
		glfwSwapBuffers(window);

		//Update input for next frame's old
		Input::UpdateInput();
		Input::SetOldMousePosition();

		//End the frame
		Time::EndFrame();
	}

	// clean up allocated resources before exit
	Physics::cleanupPhysics();
	Game::DestroyWorld();
	glfwDestroyWindow(window);
	glfwTerminate();

	cout << "ThunderBowl out!" << endl;
	return 0;
}
