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

	static GameStates GetGameState();
	static float GetGameTime();

	static const float MAX_HEALTH;
	static const float RESPAWN_TIME;

	static const int START_MG_AMMO;
	static const float START_FLAMETHROWER_AMMO;
	static const int START_MISSILE_AMMO;

	static const int MAX_MACHINE_GUN_AMMO;
	static const int MAX_MISSILE_LAUNCHER_AMMO;
	static const float MAX_FLAMETHROWER_AMMO;

	enum ActiveMenuItems
	{
		AMI_PLAYER_COUNT,
		AMI_GAME_TIME
	};
	static bool isBloodMoon;

private:
	static void HandleMenu();
	static void RedrawTimers();
	static void CreateHUD(int paintColor);
	static void DestroyHUD();
	static void EndBloodMoon();

	static GameStates gameState;
	static int humanPlayerCount;
	static int timeMinuteCount;
	static float initialGameTime;//In seconds

	static float gameTimeRemaining;

	static int timerSeconds;
	static int timerMinutes;

	static string strMinutes;
	static string strSeconds;

	static bool startButtonPrev;
	static bool menuChangePrev;

	static ActiveMenuItems activeMenuItem;
};

