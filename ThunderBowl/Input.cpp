#include "Input.h"

//Static member initialization
vector<int> Input::buttonPrevList = vector<int>(300);
vector<int> Input::buttonCurrentList = vector<int>(300);
const float Input::deadZone = 0.02;
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

bool Input::GetXBoxButton(int player, int buttonCode)
{
	int joy1 = -1;
	int joy2 = -1;
	int joy3 = -1;
	int joy4 = -1;

	//Collect and assign present joysticks
	for (int i = 0; i < 16; i++)
	{
		//Check if present
		if (glfwJoystickPresent(i))
		{
			//Assign if unassigned
			if (joy1 < 0)
				joy1 = i;
			else if (joy2 < 0)
				joy2 = i;
			else if (joy3 < 0)
				joy3 = i;
			else if (joy4 < 0)
				joy4 = i;
		}
	}
	int count;
	const unsigned char* states = NULL;

	switch (player)
	{
	case 1:
		if (joy1 >= 0)
			states = glfwGetJoystickButtons(joy1, &count);
		break;
	case 2:
		if (joy2 >= 0)
			states = glfwGetJoystickButtons(joy2, &count);
		break;
	case 3:
		if (joy3 >= 0)
			states = glfwGetJoystickButtons(joy3, &count);
		break;
	case 4:
		if (joy4 >= 0)
			states = glfwGetJoystickButtons(joy4, &count);
		break;
	default:
		cout << "Bad Player Number: only 1-4 supported!" << endl;
		return false;
		break;
	}

	//Not connected?
	if (states == NULL)
		return false;

	for (int i = 0; i < count; i++)
	{
		if (i + 300 == buttonCode && states[i] == GLFW_PRESS)
			return true;
	}
	return false;
}

float Input::GetXBoxAxis(int player, int axisCode)
{
	int joy1 = -1;
	int joy2 = -1;
	int joy3 = -1;
	int joy4 = -1;

	//Collect and assign present joysticks
	for (int i = 0; i < 16; i++)
	{
		//Check if present
		if (glfwJoystickPresent(i))
		{
			//Assign if unassigned
			if (joy1 < 0)
				joy1 = i;
			else if (joy2 < 0)
				joy2 = i;
			else if (joy3 < 0)
				joy3 = i;
			else if (joy4 < 0)
				joy4 = i;
		}
	}

	int count;
	const float* states = NULL;

	switch (player)
	{
	case 1:
		if(joy1 >= 0)
			states = glfwGetJoystickAxes(joy1, &count);
		break;
	case 2:
		if (joy2 >= 0)
			states = glfwGetJoystickAxes(joy2, &count);
		break;
	case 3:
		if (joy3 >= 0)
			states = glfwGetJoystickAxes(joy3, &count);
		break;
	case 4:
		if (joy4 >= 0)
			states = glfwGetJoystickAxes(joy4, &count);
		break;
	default:
		cout << "Bad Player Number: only 1-4 supported!" << endl;
		return 0;
		break;
	}

	//Not connected?
	if (states == NULL)
		return 0;

	if ((axisCode - 320) > 3)//Triggers
	{
		float result = (states[(axisCode - 320)] + 1.0) *0.5;
		if (result > deadZone)
			return result;
		else
			return 0;
	}
	else//Sticks
	{
		float result = states[(axisCode - 320)];
		if (result > deadZone || result < -deadZone)
			return result;
		else
			return 0;
	}
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

vec2 Input::GetControllerVibration(int playerNum)
{
	XINPUT_STATE state;
	XInputGetState(playerNum - 1, &state);

	return vec2();
}

void Input::SetControllerVibration(int playerNum, float left, float right)
{
	XINPUT_VIBRATION vibration;
	vibration.wLeftMotorSpeed = left*65535;
	vibration.wRightMotorSpeed = right*65535;
	XInputSetState(playerNum - 1, &vibration);
}

void Input::StopControllerVibration(int playerNum)
{
	XINPUT_VIBRATION vibration;
	vibration.wLeftMotorSpeed = 0;
	vibration.wRightMotorSpeed = 0;
	XInputSetState(playerNum - 1, &vibration);
}

void Input::StopAllControllerVibration()
{
	XINPUT_VIBRATION vibration;
	vibration.wLeftMotorSpeed = 0;
	vibration.wRightMotorSpeed = 0;
	for (int i = 0; i < 4; i++)
	{
		XInputSetState(i, &vibration);
	}
}