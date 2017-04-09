#include "Time.h"
#include "CommonIncludes.h"

LARGE_INTEGER Time::clockFrequency;
LARGE_INTEGER Time::startTime;
LARGE_INTEGER Time::endTime;
LARGE_INTEGER Time::deltaTicks;

int Time::elapsedFrames = 0;
double Time::deltaTime = 0.0;
double Time::timeSinceLoad = 0.0;
double Time::timeSinceGameStart = 0.0;
float Time::timeScale = 1.0;

void Time::Init()
{
	QueryPerformanceFrequency(&clockFrequency);
}

void Time::StartFrame()
{
	QueryPerformanceCounter(&startTime);
}

void Time::EndFrame()
{
	elapsedFrames++;

	QueryPerformanceCounter(&endTime);
	deltaTicks.QuadPart = endTime.QuadPart - startTime.QuadPart;
	
	deltaTime = (double)deltaTicks.QuadPart / clockFrequency.QuadPart;

	//SLEEP AND SYNC TO 60fps
	double sleepTime = (1.0 / 60.0) - deltaTime;

	//cout << deltaTime << " : " << (int)(sleepTime * 100) << endl;
	if(sleepTime > 0.0000001)
		Sleep((int)(sleepTime*1000));

	//update the delta time to account for possible sleeping
	QueryPerformanceCounter(&endTime);
	deltaTicks.QuadPart = endTime.QuadPart - startTime.QuadPart;
	deltaTime = (double)deltaTicks.QuadPart / clockFrequency.QuadPart;
	//END SLEEP AND SYNC

	timeSinceLoad += deltaTime;
	timeSinceGameStart += deltaTime;
}

LARGE_INTEGER Time::getClockFrequency()
{
	return clockFrequency;
}

LARGE_INTEGER Time::getDeltaTicks()
{
	return deltaTicks;
}

int Time::getElapsedFrames()
{
	return elapsedFrames;
}

float Time::getDeltaTime()
{
	return (float)deltaTime;
}

double Time::getTimeSinceLoad()
{
	return timeSinceLoad;
}

double Time::getTimeSinceGameStart()
{
	return timeSinceGameStart;
}

double Time::getFrameRate()
{
	return 1.0 / deltaTime;
}

void Time::resetGameTime()
{
	timeSinceGameStart = 0.0;
}