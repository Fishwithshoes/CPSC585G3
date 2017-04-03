#pragma once

class Mathf
{
public:
	static const float PI;
	static const float rad2Deg;
	static const float deg2Rad;

	static float Clamp(float value, float min, float max);
	static int RoundToInt(float value);
};