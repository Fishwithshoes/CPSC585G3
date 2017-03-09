#include "GameManager.h"
#include "Game.h"
#include "Renderer.h"
#include "Audio.h"

float		GameManager::initialGameTime = 300.0;
GameStates	GameManager::gameState = GS_FRONT_MENU;
float		GameManager::gameTimeRemaining = 0.0;
int			GameManager::timerSeconds = 0;
int			GameManager::timerMinutes = 0;
string		GameManager::strMinutes = "0";
string		GameManager::strSeconds = "0";

bool		GameManager::startButtonPrev = false;

void GameManager::Update()//Check stuff as game runs
{
	GameObject* timer1 = Game::Find("Timer1");//minutes * 00
	GameObject* timer2 = Game::Find("Timer2");//seconds 0 *0
	GameObject* timer3 = Game::Find("Timer3");//seconds 0 0*

	if (!Input::GetXBoxButton(1, ButtonCode::XBOX_START))
		startButtonPrev = false;

	switch (gameState)
	{
	case GS_FRONT_MENU:
		if (Input::GetXBoxButton(1, ButtonCode::XBOX_START) && !startButtonPrev)
		{
			StartGame();
			startButtonPrev = true;
		}
		break;
	case GS_IN_GAME:
		if (Input::GetXBoxButton(1, ButtonCode::XBOX_START) && !startButtonPrev)
		{
			ToggleGamePause();
			startButtonPrev = true;
		}

		gameTimeRemaining -= Time::getDeltaTime();
		if (gameTimeRemaining < 0.0)
		{
			gameTimeRemaining = 0.0;
			EndGame();
		}

		timerSeconds = (int)std::floorf(gameTimeRemaining);
		timerMinutes = 0;
		while (timerSeconds > 59)
		{
			timerSeconds -= 60;
			timerMinutes++;
		}
		
		strMinutes = std::to_string(timerMinutes);
		strSeconds = std::to_string(timerSeconds);
		
		timer1->particleOverlayMat.mainTexture = MAP_ZERO + (strMinutes[0] - 48);
		if (strSeconds.length() > 1)//At least 10 seconds
		{
			timer2->particleOverlayMat.mainTexture = MAP_ZERO + (strSeconds[0] - 48);
			timer3->particleOverlayMat.mainTexture = MAP_ZERO + (strSeconds[1] - 48);
		}
		else//less than 10
		{
			timer2->particleOverlayMat.mainTexture = MAP_ZERO;
			timer3->particleOverlayMat.mainTexture = MAP_ZERO + (strSeconds[0] - 48);
		}
		break;
	case GS_PAUSED:
		if (Input::GetXBoxButton(1, ButtonCode::XBOX_START) && !startButtonPrev)
		{
			ToggleGamePause();
			startButtonPrev = true;
		}
		break;
	case GS_GAME_OVER:
		if (Input::GetXBoxButton(1, ButtonCode::XBOX_START) && !startButtonPrev)
		{
			GotoMainMenu();
			startButtonPrev = true;
		}
		break;
	default:
		cout << "Game is not in a recognized state!" << endl;
		break;
	}
}

void GameManager::StartGame()
{
	cout << "Game Manager Started a New Game!" << endl;
	gameTimeRemaining = initialGameTime;
	gameState = GS_IN_GAME;
	Time::timeScale = 1.0;

	Renderer::GetCamera(0)->mode = Camera::Modes::MODE_GAME;
	Renderer::GetCamera(0)->transform.parent = nullptr;

	vector<GameObject*> hudItems = Game::FindGameObjectsWithTag(TAGS_HUD);
	for (int i = 0; i < hudItems.size(); i++)
		hudItems[i]->isVisible = true;

	vector<GameObject*> menuItems = Game::FindGameObjectsWithTag(TAGS_MENU);
	for (int i = 0; i < menuItems.size(); i++)
		menuItems[i]->isVisible = false;

	Audio::Play2DSound(SFX_Select, 0.20, 0.0);
}

void GameManager::ToggleGamePause()
{
	if (gameState == GS_IN_GAME)
	{
		gameState = GS_PAUSED;
		Time::timeScale = 0.0;

		vector<GameObject*> pauseItems = Game::FindGameObjectsWithTag(TAGS_PAUSE);
		for (int i = 0; i < pauseItems.size(); i++)
			pauseItems[i]->isVisible = true;
	}
	else if(gameState == GS_PAUSED)
	{
		gameState = GS_IN_GAME;
		Time::timeScale = 1.0;

		vector<GameObject*> pauseItems = Game::FindGameObjectsWithTag(TAGS_PAUSE);
		for (int i = 0; i < pauseItems.size(); i++)
			pauseItems[i]->isVisible = false;
	}
	else
	{
		cout << "ERROR: Can't toggle pause in MENU or GAME_OVER states!" << endl;
	}
	Audio::Play2DSound(Sounds::SFX_Pause, 0.20, 0);
	
}
void GameManager::EndGame()
{
	gameState = GS_GAME_OVER;
	Time::timeScale = 1.0;

	vector<GameObject*> hudItems = Game::FindGameObjectsWithTag(TAGS_HUD);
	for (int i = 0; i < hudItems.size(); i++)
		hudItems[i]->isVisible = false;

	vector<GameObject*> gameOverItems = Game::FindGameObjectsWithTag(TAGS_GAME_OVER);
	for (int i = 0; i < gameOverItems.size(); i++)
		gameOverItems[i]->isVisible = true;
}
void GameManager::GotoMainMenu()
{
	gameState = GS_FRONT_MENU;
	Time::timeScale = 1.0;

	vector<GameObject*> gameOverItems = Game::FindGameObjectsWithTag(TAGS_GAME_OVER);
	for (int i = 0; i < gameOverItems.size(); i++)
		gameOverItems[i]->isVisible = false;

	vector<GameObject*> menuItems = Game::FindGameObjectsWithTag(TAGS_MENU);
	for (int i = 0; i < menuItems.size(); i++)
		menuItems[i]->isVisible = true;

	Audio::Play2DSound(SFX_Select, 0.20, 0.0);
}

GameStates GameManager::GetGameState()
{
	return gameState;
}
float GameManager::GetGameTime()
{
	return gameTimeRemaining;
}