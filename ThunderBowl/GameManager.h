#pragma once
#include "CommonIncludes.h"

class GameManager
{
public:
	static void Update();

	//State changers
	static void StartGame();
	static void ToggleGamePause();
	static void EndGame();
	static void GotoMainMenu();

	static float initialGameTime;//In seconds

	static GameStates GetGameState();
	static float GetGameTime();

private:
	static GameStates gameState;

	static float gameTimeRemaining;

	static int timerSeconds;
	static int timerMinutes;

	static string strMinutes;
	static string strSeconds;

	static bool startButtonPrev;
};

