#include "Input.h"

//Static member initialization
vector<int> Input::buttonPrevList = vector<int>(164);
vector<int> Input::buttonCurrentList = vector<int>(164);
vec2 Input::newMousePos = vec2(0, 0);
vec2 Input::oldMousePos = vec2(0,0);
vec2 Input::windowSize = vec2(0,0);

//Put this at the start of the game loop
//Loop and set based on async key state false0 true1
//Current inputs become this frames' currentList
void Input::SetInputValue(int index, int value)
{
	buttonCurrentList[index] = value;
}

//Put this at the end of the game loop. 
//CurrentList inputs become next frame's prevList.
void Input::UpdateInput()
{
	for(int i = 0; i < buttonCurrentList.size(); i++)
		buttonPrevList[i] = buttonCurrentList[i];
}

bool Input::GetButtonDown(int keyCode)
{
	return buttonCurrentList[keyCode] > 0 &&
		!buttonPrevList[keyCode] > 0;
}

bool Input::GetButton(int keyCode)
{
	return buttonCurrentList[keyCode] > 0;
}

bool Input::GetButtonUp(int keyCode)
{
	return !buttonCurrentList[keyCode] > 0 &&
		buttonPrevList[keyCode] > 0;
}

//Put at start of game loop. Stores current position for this frame
void Input::SetNewMousePosition(int xcoord, int ycoord)
{
	newMousePos = vec2(xcoord, ycoord);
}

//Put at end of game loop. Stores current position as previous frame's position
void Input::SetOldMousePosition()
{
	oldMousePos = newMousePos;
}

vec2 Input::GetNewMousePosition()
{
	return newMousePos;
}

vec2 Input::GetOldMousePosition()
{
	return newMousePos - oldMousePos;
}

vec2 Input::GetMouseDelta()
{
	return newMousePos - oldMousePos;
}

void Input::SetWindowSize(int width, int height)
{
	windowSize = vec2(width, height);
}

vec2 Input::GetWindowSize()
{
	return windowSize;
}