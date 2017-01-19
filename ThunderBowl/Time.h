#pragma once
#include <stdafx.h>
#include <Windows.h>

class Time
{
public:
	static void Init();
	static void StartFrame();
	static void EndFrame();

	static LARGE_INTEGER getClockFrequency();
	static LARGE_INTEGER getDeltaTicks();

	static int getElapsedFrames();
	static double getDeltaTime();
	static double getTimeSinceLoad();
	static double getTimeSinceGameStart();
	static double getFrameRate();

	static void resetGameTime();

private:
	static LARGE_INTEGER clockFrequency;
	static LARGE_INTEGER startTime;
	static LARGE_INTEGER endTime;
	static LARGE_INTEGER deltaTicks;

	static int elapsedFrames;
	static double deltaTime;
	static double timeSinceLoad;
	static double timeSinceGameStart;
};