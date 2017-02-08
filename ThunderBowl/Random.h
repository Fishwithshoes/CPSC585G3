#pragma once

class Random
{
public:
	static int psuedo();
	static float random();
	static float rangef(float min, float max);
	static int rangei(int min, int max, bool inclusive);
};