#include "GameManager.h"
#include "Game.h"
#include "GeoGenerator.h"
#include "Renderer.h"
#include "Audio.h"

float		GameManager::initialGameTime = 300.0;
GameStates	GameManager::gameState = GS_FRONT_MENU;
float		GameManager::gameTimeRemaining = 0.0;
int			GameManager::timerSeconds = 0;
int			GameManager::timerMinutes = 0;
string		GameManager::strMinutes = "0";
string		GameManager::strSeconds = "0";

const float		GameManager::MAX_HEALTH = 100.0;
const float		GameManager::RESPAWN_TIME = 20.0;

const int		GameManager::START_MG_AMMO = 50;
const float		GameManager::START_FLAMETHROWER_AMMO = 5.0;
const int		GameManager::START_MISSILE_AMMO = 0;

const int		GameManager::MAX_MACHINE_GUN_AMMO = 200;
const float		GameManager::MAX_FLAMETHROWER_AMMO = 10.0;
const int		GameManager::MAX_MISSILE_LAUNCHER_AMMO = 4;

bool		GameManager::startButtonPrev = false;

void GameManager::Update()//Check stuff as game runs
{
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
		
		RedrawTimers();
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

	Renderer::SetCameraCount(1);

	CreateHUD();

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

	Renderer::SetCameraCount(1);

	DestroyHUD();
	vector<GameObject*> gameOverItems = Game::FindGameObjectsWithTag(TAGS_GAME_OVER);
	for (int i = 0; i < gameOverItems.size(); i++)
		gameOverItems[i]->isVisible = true;

	//Display scores
	vector<GameObject*> players = Game::FindGameObjectsWithTag(TAGS_HUMAN_PLAYER);
	vector<GameObject*> playeri = Game::FindGameObjectsWithTag(TAGS_AI_PLAYER);
	players.insert(players.end(), playeri.begin(), playeri.end());

	for (int i = 0; i < players.size(); i++)
	{
		PlayerComponent* player = &PlayerComponent();
		player = (PlayerComponent*)players[i]->GetComponent(player);

		string strScore = to_string(player->playerScore);
		GameObject* score1 = Game::Find("PlayerScore-1-" + i);
		GameObject* score2 = Game::Find("PlayerScore-2-" + i);
		GameObject* score3 = Game::Find("PlayerScore-3-" + i);
		GameObject* score4 = Game::Find("PlayerScore-4-" + i);

		for (int i = strScore.length() - 1; i >= 0; i--)
		{
			switch (i)
			{
			case 0:
				score4->particleOverlayMat.mainTexture = MAP_ZERO + (strScore[-1 + strScore.length()] - 48);
				break;
			case 1:
				score3->particleOverlayMat.mainTexture = MAP_ZERO + (strScore[-2 + strScore.length()] - 48);
				break;
			case 2:
				score2->particleOverlayMat.mainTexture = MAP_ZERO + (strScore[-3 + strScore.length()] - 48);
				break;
			case 3:
				score1->particleOverlayMat.mainTexture = MAP_ZERO + (strScore[-4 + strScore.length()] - 48);
				break;
			default:
				cout << "What are ya doin' matey? Score be too large!" << endl;
				break;
			}
		}
	}
}
void GameManager::GotoMainMenu()
{
	gameState = GS_FRONT_MENU;
	Time::timeScale = 1.0;

	//Return all players back to normal
	vector<GameObject*> players = Game::FindGameObjectsWithTag(TAGS_HUMAN_PLAYER);
	vector<GameObject*> playeri = Game::FindGameObjectsWithTag(TAGS_AI_PLAYER);
	players.insert(players.end(), playeri.begin(), playeri.end());

	for (int i = 0; i < players.size(); i++)
	{
		PlayerComponent* player = &PlayerComponent();
		player = (PlayerComponent*)players[i]->GetComponent(player);
		player->currentWeapon = GW_MACHINE_GUN;
		player->machineGunAmmo = START_MG_AMMO;
		player->flamethrowerAmmo = START_FLAMETHROWER_AMMO;
		player->missileLauncherAmmo = START_MISSILE_AMMO;
		player->playerScore = 0;

		HealthComponent* health = &HealthComponent();
		health = (HealthComponent*)players[i]->GetComponent(health);
		health->currentHealth = MAX_HEALTH;

		if (players[i]->tag == TAGS_HUMAN_PLAYER)
		{
			VehicleComponent* vehicle = &VehicleComponent();
			vehicle = (VehicleComponent*)players[i]->GetComponent(vehicle);

			vehicle->physVehicle->setGlobalPose(physx::PxTransform(vehicle->myStartPosition, physx::PxQuat(physx::PxIdentity)));
			vehicle->physVehicle->setAngularVelocity(physx::PxVec3(0, 0, 0));
			vehicle->physVehicle->setLinearVelocity(physx::PxVec3(0, 0, 0));
		}
		else
		{
			EnemyComponent* enemy = &EnemyComponent();
			enemy = (EnemyComponent*)players[i]->GetComponent(enemy);

			enemy->enPhysVehicle->setGlobalPose(physx::PxTransform(enemy->startPosition, enemy->startRotation));
			enemy->enPhysVehicle->setAngularVelocity(physx::PxVec3(0, 0, 0));
			enemy->enPhysVehicle->setLinearVelocity(physx::PxVec3(0, 0, 0));
		}
	}

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

void GameManager::RedrawTimers()
{
	for (int i = 0; i < Renderer::GetCameraCount(); i++)
	{
		GameObject* timer1 = Game::Find("Timer1"+(i+1));//minutes * 00
		GameObject* timer2 = Game::Find("Timer2"+(i+1));//seconds 0 *0
		GameObject* timer3 = Game::Find("Timer3"+(i+1));//seconds 0 0*

		if(timer1 != nullptr)
			timer1->particleOverlayMat.mainTexture = MAP_ZERO + (strMinutes[0] - 48);

		if (timer2 != nullptr && timer3 != nullptr)
		{
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
		}
	}
}
void GameManager::CreateHUD()
{
	for (int i = 0; i < Renderer::GetCameraCount(); i++)
	{
		float rescale = 1.0;
		vec3 metersOffset = vec3(0);
		vec3 timerOffset = vec3(0);
		vec3 scoreOffset = vec3(0);
		vec3 speedometerOffset = vec3(0);
		if (Renderer::GetCameraCount() == 2)
		{
			rescale = 0.75;
			switch (i)
			{
			case 0:
				metersOffset		= vec3(0.1, 0.06, 0.0);
				timerOffset			= vec3(0.15, 0.032, 0.0);
				scoreOffset			= vec3(-0.06, 0.032, 0.0);
				speedometerOffset	= vec3(-0.175, 0.9, 0.0);
				break;
			case 1:
				metersOffset		= vec3(0.1, -0.935, 0.0);
				timerOffset			= vec3(0.15, -0.964, 0.0);
				scoreOffset			= vec3(-0.06, -0.964, 0.0);
				speedometerOffset	= vec3(-0.175, -0.125, 0.0);
				break;
			default:
				cout << "ERROR: Can't set HUD for player 3 when there are only 2!" << endl;
				break;
			}
		}
		else if (Renderer::GetCameraCount() >= 3)
		{
			rescale = 0.5;
			switch (i)
			{
			case 0:
				metersOffset		= vec3(-0.35, 0.11, 0.0);
				timerOffset			= vec3(-0.23, 0.054, 0.0);
				scoreOffset			= vec3(-0.70, 0.054, 0.0);
				speedometerOffset	= vec3(-0.855, 0.81, 0.0);
				break;
			case 1:
				metersOffset		= vec3(0.75, 0.11, 0.0);
				timerOffset			= vec3(0.87, 0.054, 0.0);
				scoreOffset			= vec3(0.40, 0.054, 0.0);
				speedometerOffset	= vec3(0.142, 0.81, 0.0);
				break;
			case 2:
				metersOffset		= vec3(-0.35, -0.89, 0.0);
				timerOffset			= vec3(-0.23, -0.946, 0.0);
				scoreOffset			= vec3(-0.70, -0.946, 0.0);
				speedometerOffset	= vec3(-0.855,-0.19, 0.0);
				break;
			case 3:
				metersOffset		= vec3(0.75, -0.89, 0.0);
				timerOffset			= vec3(0.87, -0.946, 0.0);
				scoreOffset			= vec3(0.40, -0.946, 0.0);
				speedometerOffset	= vec3(0.142,-0.19, 0.0);
				break;
			default:
				cout << "ERROR: Can't set HUD for player 5+!" << endl;
				break;
			}
		}

		//Health meter
		GameObject temp = GameObject("StatusMeterBack" + (i + 1), TAGS_HUD);
		temp.mesh = GeoGenerator::MakeRect(1.25*rescale, 0.32*rescale, GA_LEFT);
		temp.transform.Translate(vec3(-0.3725, 0.79, 0.0) + metersOffset, false);
		temp.particleOverlayMat.color = vec4(0, 0, 0, 0.5);
		Game::CreateOverlayObject(temp);

		temp = GameObject("HealthMeter" + (i + 1), TAGS_HUD);
		temp.mesh = GeoGenerator::MakeRect(1.00*rescale, 0.15*rescale, GA_LEFT);
		temp.transform.Translate(vec3(-0.3725 + 0.1225*rescale, 0.79 + 0.06*rescale, -0.1) + metersOffset, false);
		temp.particleOverlayMat.color = vec4(0, 0.5, 1, 1);
		Game::CreateOverlayObject(temp);

		temp = GameObject("HeathIcon" + (i + 1), TAGS_HUD);
		temp.mesh = GeoGenerator::MakeRect(0.15*rescale, 0.15*rescale, GA_CENTER);
		temp.transform.Translate(vec3(-0.3725 + 0.055*rescale, 0.79 + 0.06*rescale, -0.1) + metersOffset, false);
		temp.particleOverlayMat.mainTexture = MAP_HEALTH_ICON;
		Game::CreateOverlayObject(temp);

		//Ammo meter
		temp = GameObject("AmmoMeter" + (i + 1), TAGS_HUD);
		temp.mesh = GeoGenerator::MakeRect(1.00*rescale, 0.05*rescale, GA_LEFT);
		temp.transform.Translate(vec3(-0.3725 + 0.1225*rescale, 0.79 - 0.09*rescale, -0.1) + metersOffset, false);
		temp.particleOverlayMat.color = vec4(0.8, 0.4, 0.0, 1);
		Game::CreateOverlayObject(temp);

		temp = GameObject("WeaponIcon" + (i + 1), TAGS_HUD);
		temp.mesh = GeoGenerator::MakeRect(0.1*rescale, 0.1*rescale, GA_CENTER);
		temp.transform.Translate(vec3(-0.3775 + 0.06*rescale, 0.79 - 0.09*rescale, -0.1) + metersOffset, false);
		temp.particleOverlayMat.mainTexture = MAP_MACHINE_GUN_ICON;
		Game::CreateOverlayObject(temp);

		//Speedometer
		temp = GameObject("SpeedometerBack" + (i + 1), TAGS_HUD);
		temp.mesh = GeoGenerator::MakeRect(0.8*rescale, 0.5*rescale, GA_CENTER);
		temp.transform.Translate(vec3(0.73, -0.67, 0.0) + speedometerOffset, false);
		temp.particleOverlayMat.color = vec4(0, 0, 0, 0.5);
		Game::CreateOverlayObject(temp);

		temp = GameObject("SpeedometerGauge" + (i + 1), TAGS_HUD);
		temp.mesh = GeoGenerator::MakeRect(0.75*rescale, 0.45*rescale, GA_CENTER);
		temp.transform.Translate(vec3(0.73, -0.67, -0.1) + speedometerOffset, false);
		temp.particleOverlayMat.color = vec4(0.5, 0.5, 0.5, 1.0);
		Game::CreateOverlayObject(temp);

		temp = GameObject("SpeedometerNeedle" + (i + 1), TAGS_HUD);
		temp.mesh = GeoGenerator::MakeRect(0.35*rescale, 0.05*rescale, GA_RIGHT);
		temp.transform.Translate(vec3(0.73, -0.67 - 0.18*rescale, -0.2) + speedometerOffset, false);
		temp.particleOverlayMat.color = vec4(1.0, 0.0, 0.0, 1.0);
		Game::CreateOverlayObject(temp);

		//Game Timer
		temp = GameObject("TimerBack" + (i + 1), TAGS_HUD);
		temp.mesh = GeoGenerator::MakeRect(0.442*rescale, 0.1*rescale, GA_LEFT);
		temp.transform.Translate(vec3(-0.63, 0.9, 0.0) + timerOffset, false);
		temp.particleOverlayMat.color = vec4(0, 0, 0, 0.5);
		Game::CreateOverlayObject(temp);

		temp = GameObject("TimerLabel" + (i + 1), TAGS_HUD);
		temp.mesh = GeoGenerator::MakeRect(0.2*rescale, 0.2*rescale, GA_LEFT);
		temp.transform.Translate(vec3(-0.63, 0.9, -0.1) + timerOffset, false);
		temp.particleOverlayMat.color = vec4(1.0, 1.0, 1.0, 1.0);
		temp.particleOverlayMat.mainTexture = MAP_TIME;
		Game::CreateOverlayObject(temp);

		temp = GameObject("Timer1" + (i + 1), TAGS_HUD);
		temp.mesh = GeoGenerator::MakeRect(0.075*rescale, 0.075*rescale, GA_LEFT);
		temp.transform.Translate(vec3(-0.63 + 0.11*rescale, 0.9, -0.2) + timerOffset, false);
		temp.particleOverlayMat.color = vec4(1.0, 1.0, 1.0, 1.0);
		temp.particleOverlayMat.mainTexture = MAP_ZERO;
		Game::CreateOverlayObject(temp);

		temp = GameObject("Timer2" + (i + 1), TAGS_HUD);
		temp.mesh = GeoGenerator::MakeRect(0.075*rescale, 0.075*rescale, GA_LEFT);
		temp.transform.Translate(vec3(-0.63 + 0.15*rescale, 0.9, -0.3) + timerOffset, false);
		temp.particleOverlayMat.color = vec4(1.0, 1.0, 1.0, 1.0);
		temp.particleOverlayMat.mainTexture = MAP_ZERO;
		Game::CreateOverlayObject(temp);

		temp = GameObject("Timer3" + (i + 1), TAGS_HUD);
		temp.mesh = GeoGenerator::MakeRect(0.075*rescale, 0.075*rescale, GA_LEFT);
		temp.transform.Translate(vec3(-0.63 + 0.17*rescale, 0.9, -0.4) + timerOffset, false);
		temp.particleOverlayMat.color = vec4(1.0, 1.0, 1.0, 1.0);
		temp.particleOverlayMat.mainTexture = MAP_ZERO;
		Game::CreateOverlayObject(temp);

		//Score
		temp = GameObject("ScoreBack" + (i + 1), TAGS_HUD);
		temp.mesh = GeoGenerator::MakeRect(0.5*rescale, 0.1*rescale, GA_LEFT);
		temp.transform.Translate(vec3(0.34, 0.9, 0.0) + scoreOffset, false);
		temp.particleOverlayMat.color = vec4(0, 0, 0, 0.5);
		Game::CreateOverlayObject(temp);

		temp = GameObject("ScoreLabel" + (i + 1), TAGS_HUD);
		temp.mesh = GeoGenerator::MakeRect(0.2*rescale, 0.2*rescale, GA_LEFT);
		temp.transform.Translate(vec3(0.34 + 0.02*rescale, 0.9, -0.1) + scoreOffset, false);
		temp.particleOverlayMat.color = vec4(1.0, 1.0, 1.0, 1.0);
		temp.particleOverlayMat.mainTexture = MAP_SCORE;
		Game::CreateOverlayObject(temp);

		temp = GameObject("Score1" + (i + 1), TAGS_HUD);
		temp.mesh = GeoGenerator::MakeRect(0.075*rescale, 0.075*rescale, GA_LEFT);
		temp.transform.Translate(vec3(0.34 + 0.14*rescale, 0.9, -0.2) + scoreOffset, false);
		temp.particleOverlayMat.color = vec4(1.0, 1.0, 1.0, 1.0);
		temp.particleOverlayMat.mainTexture = MAP_ZERO;
		Game::CreateOverlayObject(temp);

		temp = GameObject("Score2" + (i + 1), TAGS_HUD);
		temp.mesh = GeoGenerator::MakeRect(0.075*rescale, 0.075*rescale, GA_LEFT);
		temp.transform.Translate(vec3(0.34 + 0.16*rescale, 0.9, -0.3) + scoreOffset, false);
		temp.particleOverlayMat.color = vec4(1.0, 1.0, 1.0, 1.0);
		temp.particleOverlayMat.mainTexture = MAP_ZERO;
		Game::CreateOverlayObject(temp);

		temp = GameObject("Score3" + (i + 1), TAGS_HUD);
		temp.mesh = GeoGenerator::MakeRect(0.075*rescale, 0.075*rescale, GA_LEFT);
		temp.transform.Translate(vec3(0.34 + 0.18*rescale, 0.9, -0.4) + scoreOffset, false);
		temp.particleOverlayMat.color = vec4(1.0, 1.0, 1.0, 1.0);
		temp.particleOverlayMat.mainTexture = MAP_ZERO;
		Game::CreateOverlayObject(temp);

		temp = GameObject("Score4" + (i + 1), TAGS_HUD);
		temp.mesh = GeoGenerator::MakeRect(0.075*rescale, 0.075*rescale, GA_LEFT);
		temp.transform.Translate(vec3(0.34 + 0.2*rescale, 0.9, -0.5) + scoreOffset, false);
		temp.particleOverlayMat.color = vec4(1.0, 1.0, 1.0, 1.0);
		temp.particleOverlayMat.mainTexture = MAP_ZERO;
		Game::CreateOverlayObject(temp);
	}
}
void GameManager::DestroyHUD()
{
	vector<GameObject*> hudContents = Game::FindGameObjectsWithTag(TAGS_HUD);
	for (int i = hudContents.size() - 1; i >= 0; i--)
	{
		Game::DestroyOverlayObjectAt(hudContents[i]->objectID);
	}
}