/*
TODO:
	General Debug/code cleanup
	Righting player only when upside down
*/


#include "CommonIncludes.h"
#include "GameManager.h"
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
		std::cout << "ERROR: GLFW failed to initilize, TERMINATING" << endl;
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
	window = glfwCreateWindow(Camera::WIDTH, Camera::HEIGHT, "Thunderbowl", 0, 0);
	if (!window) {
		std::cout << "Program failed to create GLFW window, TERMINATING" << endl;
		glfwTerminate();
		return -1;
	}
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
		std::cout << "glew init failed" << endl;
		return -1;
	}
	RendererUtility::CheckGLErrors();
#endif

#pragma omp parallel
	{
		printf("Running on multiple threads\n");
	}

	RendererUtility::QueryGLVersion();
	glfwSwapInterval(0);

	Time::Init();
	Audio::Init();
	Renderer renderer;
	Renderer::Init(&renderer);
	Physics::initializePhysX();

	Game::BuildWorld();
	Renderer::GetCamera(0)->Start();

	//Audio::PlayMusic(MUS_Conta, 0.25);
	Audio::PlayMusic(MUS_Menu, 0.3);

	//MAIN LOOP
	while (!glfwWindowShouldClose(window))
	{
		//Start the frame
		Time::StartFrame();

		//Collect Input
		glfwPollEvents();

		if (Input::GetButtonDown(ButtonCode::SPACE))
		{
			if (GameManager::GetGameState() == GS_IN_GAME || GameManager::GetGameState() == GS_PAUSED)
			{
				if (Renderer::GetCamera(0)->mode == Camera::Modes::MODE_GAME)
					Renderer::GetCamera(0)->mode = Camera::Modes::MODE_FREE;
				else
					Renderer::GetCamera(0)->mode = Camera::Modes::MODE_GAME;
			}
		}
		if (GameManager::GetGameState() == GS_FRONT_MENU || GameManager::GetGameState() == GS_GAME_OVER)
		{
			Renderer::GetCamera(0)->mode = Camera::Modes::MODE_PRESENTATION;
		}

		//Game Logic
		if (GameManager::GetGameState() == GS_IN_GAME)
		{
#pragma parallel for
			for (int i = Game::staticObjectList.size() - 1; i >= 0; i--)
			{
				Game::staticObjectList[i].Update();
			}
#pragma parallel for
			for (int i = Game::worldObjectList.size() - 1; i >= 0; i--)
			{
				Game::worldObjectList[i].Update();
			}
#pragma parallel for
			for (int i = Game::particleObjectList.size() - 1; i >= 0; i--)
			{
				Game::particleObjectList[i].Update();
			}
#pragma parallel for
			for (int i = Game::overlayObjectList.size() - 1; i >= 0; i--)
			{
				Game::overlayObjectList[i].Update();
			}
#pragma parallel for
			for (int i = Game::aiObjectList.size() - 1; i >= 0; i--)
			{
				Game::aiObjectList[i].Update();
			}
			Physics::stepPhysics();	//TODO SUBJECT TO CHANGE
		}

		//cout << "Dynamics to Douse: " << Game::worldObjectList.size() << endl;

		Renderer::GetCamera(0)->Update();

		GameManager::Update(); //TODO should this go elsewhere?

		//Render Scene
		Renderer::RenderScene(&renderer);
		glfwSwapBuffers(window);

		//Update input for next frame's old
		Input::UpdateInput();
		Input::SetOldMousePosition();

		//Framerate display
		if (Input::GetButton(ButtonCode::F))
		{
			vector<GameObject*> frameItems = Game::FindGameObjectsWithTag(TAGS_DEBUG_0);
			for (int i = 0; i < frameItems.size(); i++)
				frameItems[i]->isVisible = true;

			string strFrame = to_string((int)Time::getFrameRate());
			GameObject* frame1 = Game::Find("framerate1");
			GameObject* frame2 = Game::Find("framerate2");
			GameObject* frame3 = Game::Find("framerate3");

			for (int i = strFrame.length() - 1; i >= 0; i--)
			{
				switch (i)
				{
				case 0:
					frame3->particleOverlayMat.mainTexture = MAP_ZERO + (strFrame[-1 + strFrame.length()] - 48);
					break;
				case 1:
					frame2->particleOverlayMat.mainTexture = MAP_ZERO + (strFrame[-2 + strFrame.length()] - 48);
					break;
				case 2:
					frame1->particleOverlayMat.mainTexture = MAP_ZERO + (strFrame[-3 + strFrame.length()] - 48);
					break;
				default:
					std::cout << "What are ya doin' matey? Score be too large!" << std::endl;
					break;
				}
			}
		}
		else
		{
			vector<GameObject*> frameItems = Game::FindGameObjectsWithTag(TAGS_DEBUG_0);
			for (int i = 0; i < frameItems.size(); i++)
				frameItems[i]->isVisible = false;
		}

		//End the frame
		Time::EndFrame();
	}

	// clean up allocated resources before exit
	Physics::cleanupPhysics();
	Game::DestroyWorld();
	glfwDestroyWindow(window);
	glfwTerminate();

	std::cout << "ThunderBowl out!" << std::endl;
	return 0;
}
