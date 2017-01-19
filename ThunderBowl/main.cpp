#include "CommonIncludes.h"
#include "Game.h"
#include "GameObject.h"
#include "ShaderUtility.h"
#include "GeoGenerator.h"
#include "Camera.h"
#include "Input.h"

using namespace std;
using namespace glm;

int WIDTH = 960;
int HEIGHT = 540;

Camera camera = Camera(Transform::identity());

//Intitialize and activate buffers for geometry, returning true if successful
bool InitializeGeometry(Geometry *geometry)
{
	Mesh mesh;

	mesh = Game::worldObjectList.at(0).mesh;
	geometry->elementCount = mesh.indicies.size();

	// these vertex attribute indices correspond to those specified for the
	// input variables in the vertex shader
	const GLuint POSITION_INDEX = 0;
	const GLuint COLOR_INDEX = 1;
	const GLuint NORMAL_INDEX = 2;
	const GLuint TEXCOORD_INDEX = 3;

	// create a vertex array object encapsulating all vertex attributes
	glGenVertexArrays(1, &geometry->vertexArray);
	glBindVertexArray(geometry->vertexArray);

	// create an array buffer object for storing positions
	glGenBuffers(1, &geometry->positionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, geometry->positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*3*mesh.positions.size(), mesh.positions.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(POSITION_INDEX, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(POSITION_INDEX);

	// create another one for storing colors
	glGenBuffers(1, &geometry->colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, geometry->colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*3*mesh.colors.size(), mesh.colors.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(COLOR_INDEX, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(COLOR_INDEX);

	// create another one for storing normals
	glGenBuffers(1, &geometry->normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, geometry->normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*3*mesh.normals.size(), mesh.normals.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(NORMAL_INDEX, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(NORMAL_INDEX);

	// create another one for storing texcoords
	glGenBuffers(1, &geometry->texCoordBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, geometry->texCoordBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*2*mesh.texcoords.size(), mesh.texcoords.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(TEXCOORD_INDEX, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(TEXCOORD_INDEX);

	// create another for storing indices
	glGenBuffers(1, &geometry->indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*mesh.indicies.size(), mesh.indicies.data(), GL_STATIC_DRAW);

	// unbind our buffers, resetting to default state
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// check for OpenGL errors and return false if error occurred
	return !ShaderUtility::CheckGLErrors();
}

// --------------------------------------------------------------------------
// Rendering function that loads and draws each object with the GPU

float theta = 0;
void RenderScene(Geometry *geometry, Shader *shader)
{
	// clear screen to a dark grey colour
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);

	glUseProgram(shader->program);
	glUniform1f(glGetUniformLocation(shader->program, "theta"), theta -= 0.00);

	//Program Uniforms for WorldToView and ViewToProjection
	camera.Update();

	glUniformMatrix4fv(glGetUniformLocation(shader->program, "WorldToView"), 1, GL_TRUE, value_ptr(camera.GetWorldToViewMatrix()));
	glUniformMatrix4fv(glGetUniformLocation(shader->program, "ViewToProjection"), 1, GL_TRUE, value_ptr(camera.GetViewToProjectionMatrix()));

	//For each GameObject Load geometry into GPU, bind and draw it
	//GLint modelToWorldUniform = glGetUniformLocation()
	for (int i = 0; i < Game::worldObjectList.size(); i++)
	{
		
	}
	mat4 mtw = mat4(
		1, 0, 0, 0,
		0, 1, 0, -0.8,
		0, 0, 1, 2.0,
		0, 0, 0, 1);
	glUniformMatrix4fv(glGetUniformLocation(shader->program, "ModelToWorld"), 1, GL_TRUE, value_ptr(mtw));
	glBindVertexArray(geometry->vertexArray);
	glDrawElements(GL_TRIANGLES, geometry->elementCount, GL_UNSIGNED_INT, nullptr);

	mtw = mat4(
		1, 0, 0, 1.5,
		0, 1, 0, -0.5,
		0, 0, 1, 5.5,
		0, 0, 0, 1);
	glUniformMatrix4fv(glGetUniformLocation(shader->program, "ModelToWorld"), 1, GL_TRUE, value_ptr(mtw));
	glBindVertexArray(geometry->vertexArray);
	glDrawElements(GL_TRIANGLES, geometry->elementCount, GL_UNSIGNED_INT, nullptr);

	//Unbind geometry and shader
	glBindVertexArray(0);
	glUseProgram(0);

	// check for and report any OpenGL errors
	ShaderUtility::CheckGLErrors();
}

// --------------------------------------------------------------------------
// GLFW callback functions

// reports GLFW errors
void ErrorCallback(int error, const char* description)
{
	cout << "GLFW ERROR " << error << ":" << endl;
	cout << description << endl;
}

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
			else
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
	if (button == GLFW_MOUSE_BUTTON_LEFT)
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
	glfwSetErrorCallback(ErrorCallback);

	// attempt to create a window with an OpenGL 4.1 core profile context
	GLFWwindow *window = 0;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(Camera::WIDTH, Camera::HEIGHT, "CPSC 453 OpenGL Boilerplate", 0, 0);
	if (!window) {
		cout << "Program failed to create GLFW window, TERMINATING" << endl;
		glfwTerminate();
		return -1;
	}
	Input::SetWindowSize(Camera::WIDTH, Camera::HEIGHT);

	// set keyboard callback function and make our context current (active)
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
	ShaderUtility::CheckGLErrors();
#endif

	ShaderUtility::QueryGLVersion();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	Game::BuildWorld();

	// call function to load and compile shader programs
	Shader shader;
	if (!ShaderUtility::InitializeShaders(&shader)) {
		cout << "Program could not initialize shaders, TERMINATING" << endl;
		return -1;
	}

	// call function to create and fill buffers with geometry data
	Geometry geometry;
	if (!InitializeGeometry(&geometry))
		cout << "Program failed to intialize geometry!" << endl;

	//MAIN LOOP
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		RenderScene(&geometry, &shader);

		glfwSwapBuffers(window);

		Input::UpdateInput();
		Input::SetOldMousePosition();
	}

	// clean up allocated resources before exit
	glfwDestroyWindow(window);
	glfwTerminate();

	cout << "Program Terminated by User!" << endl;
	return 0;
}
