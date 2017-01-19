#pragma once
#include "CommonIncludes.h"

class Input
{
public:
	static vector<int> buttonPrevList;
	static vector<int> buttonCurrentList;

	static bool GetButtonDown(int buttonCode);
	static bool GetButton(int buttonCode);
	static bool GetButtonUp(int buttonCode);

	static const int NumInputsToSet = 164;
	static void SetInputValue(int index, int value);
	static void UpdateInput();

	static void SetNewMousePosition(int xcoord, int ycoord);
	static void SetOldMousePosition();
	static vec2 GetNewMousePosition();
	static vec2 GetOldMousePosition();
	static vec2 GetMouseDelta();

	static void SetWindowSize(int width, int height);
	static vec2 GetWindowSize();

protected:

private:
	static vec2 newMousePos;
	static vec2 oldMousePos;
	static vec2 windowSize;
};