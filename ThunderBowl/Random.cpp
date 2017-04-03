#include "Random.h"
#include "CommonIncludes.h"

int Random::psuedo()
{
	return rand();
}

float Random::random()
{
	return float(rand() % 10000 + 1) * 0.0001;
}

float Random::rangef(float min, float max)
{
	return (Random::random() * ((max - min) * 1000) + min * 1000) * 0.001;
}

int Random::rangei(int min, int max, bool inclusive)
{
	if(!inclusive)
	{
		return rand() % (max - min) + min;
	}
	else
	{
		return rand() % (max - min + 1) + min;
	}
}