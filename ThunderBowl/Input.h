#pragma once
#include "CommonIncludes.h"
#include "ButtonCode.h"

class Input
{
public:
	static vector<int> buttonPrevList;
	static vector<int> buttonCurrentList;

	static bool GetButtonDown(int buttonCode);
	static bool GetButton(int buttonCode);
	static bool GetButtonUp(int buttonCode);

	static bool GetXBoxButton(int player, int buttonCode);
	static float GetXBoxAxis(int player, int axisCode);
	static const float deadZone;

	static const int NumInputsToSet = 300;
	static void SetInputValue(int index, int value);
	static void UpdateInput();

	static void SetNewMousePosition(int xcoord, int ycoord);
	static void SetOldMousePosition();
	static vec2 GetNewMousePosition();
	static vec2 GetOldMousePosition();
	static vec2 GetMouseDelta();

	static void SetWindowSize(int width, int height);
	static vec2 GetWindowSize();

	//For XBOX 360 Controllers: Get both controller motor speeds. Range 0-1.
	static vec2 GetControllerVibration(int playerNum);
	//For XBOX 360 Controllers: Set both controller motor speeds. Range 0-1.
	static void SetControllerVibration(int playerNum, float left, float right);
	//For XBOX 360 Controllers: Stop controller motor.
	static void StopControllerVibration(int playerNum);
	//For XBOX 360 Controllers: Stop motors on all connected controllers
	static void StopAllControllerVibration();

protected:

private:
	static vec2 newMousePos;
	static vec2 oldMousePos;
	static vec2 windowSize;
};