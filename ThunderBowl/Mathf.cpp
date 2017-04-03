#include "Mathf.h"

const float Mathf::PI = 3.14159265358979323846;
const float Mathf::rad2Deg = 180 / Mathf::PI;
const float Mathf::deg2Rad = Mathf::PI / 180;

float Mathf::Clamp(float value, float min, float max)
{
	if(value > max)
		return max;
	else if(value < min)
		return min;
	else
		return value;
}

int Mathf::RoundToInt(float value)
{
	return int(value);
}