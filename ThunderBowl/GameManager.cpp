#include "GameManager.h"
#include "Game.h"
#include "GeoGenerator.h"
#include "Renderer.h"
#include "Audio.h"
#include "Physics.h"

float		GameManager::initialGameTime = 300.0;
int			GameManager::humanPlayerCount = 1;
int			GameManager::timeMinuteCount = 5;
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

bool							GameManager::startButtonPrev = false;
bool							GameManager::menuChangePrev = false;
GameManager::ActiveMenuItems	GameManager::activeMenuItem = AMI_PLAYER_COUNT;

void GameManager::Update()//Check stuff as game runs
{
	if (!Input::GetXBoxButton(1, ButtonCode::XBOX_START))
		startButtonPrev = false;

	switch (gameState)
	{
	case GS_FRONT_MENU:
		HandleMenu();
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
		if (Input::GetXBoxButton(1, ButtonCode::XBOX_BACK))
		{
			ToggleGamePause();
			EndGame();
			GotoMainMenu();
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

	for (int i = 0; i < humanPlayerCount; i++) //CREATE PLAYERS
	{
		GameObject player = GameObject();
		player.mesh = GeoGenerator::MakeBox(3, 1, 3, false);
		player.transform.position = Game::plVehStartPositions[i];
		player.name = "Player" + to_string(i);
		player.tag = TAGS_HUMAN_PLAYER;
		GameObject* ptr = Game::CreateWorldObject(player);
		ptr->AddComponent(new PlayerComponent());
		ptr->AddComponent(new VehicleComponent());
		ptr->AddComponent(new MachineGunComponent());
		ptr->AddComponent(new MissileLauncherComponent());
		ptr->AddComponent(new FlamethrowerComponent());
		ptr->AddComponent(new HealthComponent());
		VehicleComponent* vehicle = &VehicleComponent();
		vehicle = (VehicleComponent*)ptr->GetComponent(vehicle);
		vehicle->SetPlayerNum(i + 1);
	}

	for (int i = 0; i < (4 - humanPlayerCount); i++) //CREATE AI
	{
		GameObject opponent = GameObject();
		opponent.mesh = GeoGenerator::MakeBox(2, 1, 2, false);
		opponent.transform.position = Game::aiVehStartPositions[i];
		opponent.transform.Rotate(Transform::Up(), Mathf::PI, false);
		opponent.name = "AI" + to_string(i);
		opponent.tag = TAGS_AI_PLAYER;
		GameObject* ptr = Game::CreateWorldObject(opponent);
		ptr->AddComponent(new PlayerComponent());
		ptr->AddComponent(new EnemyComponent());
		ptr->AddComponent(new MachineGunComponent());
		ptr->AddComponent(new AIControlComponent1());
		ptr->AddComponent(new HealthComponent());
	}

	Renderer::GetCamera(0)->mode = Camera::Modes::MODE_GAME;
	Renderer::GetCamera(0)->transform.parent = nullptr;

	Renderer::SetCameraCount(humanPlayerCount);

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
	activeMenuItem = AMI_PLAYER_COUNT;
	Time::timeScale = 1.0;

	//Destroy all human and bot duplicants so they can be reprinted by StartGame()
	vector<GameObject*> players = Game::FindGameObjectsWithTag(TAGS_HUMAN_PLAYER);
	vector<GameObject*> playeri = Game::FindGameObjectsWithTag(TAGS_AI_PLAYER);
	players.insert(players.end(), playeri.begin(), playeri.end());
	for (int i = players.size()-1; i >= 0; i--)//Chassis objects
	{	
		Game::DestroyWorldObjectAt(players[i]->objectID);
	}
	vector<GameObject*> wheels = Game::FindGameObjectsWithTag(TAGS_VEHICLE_WHEEL);
	for (int i = wheels.size() - 1; i >= 0; i--)//Chassis objects
	{
		Game::DestroyWorldObjectAt(wheels[i]->objectID);
	}

	//Get rid of the physics representations
	for (int i = 0; i < Physics::playerVehicleNoDrives.size(); i++)
	{
		Physics::playerVehicleNoDrives[i]->getRigidDynamicActor()->release();
		Physics::playerVehicleNoDrives[i]->free();
	}
	for (int i = 0; i < Physics::opponentVehicleNoDrives.size(); i++)
	{
		Physics::opponentVehicleNoDrives[i]->getRigidDynamicActor()->release();
		Physics::opponentVehicleNoDrives[i]->free();
	}
	Physics::playerVehicleNoDrives = {};
	Physics::opponentVehicleNoDrives = {};

	Game::KillAllParticles();

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

void GameManager::HandleMenu()
{
	vector<GameObject*> menuItems = Game::FindGameObjectsWithTag(TAGS_MENU);
	vector<GameObject*> playerMenu = {};
	vector<GameObject*> timeMenu = {};
	for (int i = 0; i < menuItems.size(); i++)
	{
		if (menuItems[i]->name.find("Players") != string::npos)			
			playerMenu.push_back(menuItems[i]);
		if (menuItems[i]->name.find("Time") != string::npos)
			timeMenu.push_back(menuItems[i]);
	}

	switch (activeMenuItem)
	{
	case AMI_PLAYER_COUNT:
		if (Input::GetXBoxAxis(1, ButtonCode::XBOX_JOY_LEFT_VERTICAL) < -0.5 && !menuChangePrev)
		{
			menuChangePrev = true;
			activeMenuItem = AMI_GAME_TIME;
		}
		for (int i = 0; i < playerMenu.size(); i++)
		{
			playerMenu[i]->transform.scale = vec3(1.3);
			playerMenu[i]->particleOverlayMat.color = vec4(vec3(0.8), 1.0);
		}
		for (int i = 0; i < timeMenu.size(); i++)
		{
			timeMenu[i]->transform.scale = vec3(1.0);
			timeMenu[i]->particleOverlayMat.color = vec4(vec3(0.5), 1.0);
		}
		if (Input::GetXBoxAxis(1, ButtonCode::XBOX_JOY_LEFT_HORIZONTAL) < -0.5 && !menuChangePrev)
		{
			menuChangePrev = true;
			humanPlayerCount--;
			if (humanPlayerCount < 1)
				humanPlayerCount = 1;			
		}
		if (Input::GetXBoxAxis(1, ButtonCode::XBOX_JOY_LEFT_HORIZONTAL) > 0.5 && !menuChangePrev)
		{
			menuChangePrev = true;
			humanPlayerCount++;
			if (humanPlayerCount > 4)
				humanPlayerCount = 4;
		}
		Game::Find("PlayersCount")->particleOverlayMat.mainTexture = MAP_ZERO + humanPlayerCount;
		break;
	case AMI_GAME_TIME:
		if (Input::GetXBoxAxis(1, ButtonCode::XBOX_JOY_LEFT_VERTICAL) > 0.5 && !menuChangePrev)
		{
			menuChangePrev = true;
			activeMenuItem = AMI_PLAYER_COUNT;
		}
		for (int i = 0; i < playerMenu.size(); i++)
		{
			playerMenu[i]->transform.scale = vec3(1.0);
			playerMenu[i]->particleOverlayMat.color = vec4(vec3(0.5), 1.0);
		}
		for (int i = 0; i < timeMenu.size(); i++)
		{
			timeMenu[i]->transform.scale = vec3(1.3);
			timeMenu[i]->particleOverlayMat.color = vec4(vec3(0.8), 1.0);
		}
		if (Input::GetXBoxAxis(1, ButtonCode::XBOX_JOY_LEFT_HORIZONTAL) < -0.5 && !menuChangePrev)
		{
			menuChangePrev = true;
			timeMinuteCount--;
			if (timeMinuteCount < 3)
				timeMinuteCount = 3;
		}
		if (Input::GetXBoxAxis(1, ButtonCode::XBOX_JOY_LEFT_HORIZONTAL) > 0.5 && !menuChangePrev)
		{
			menuChangePrev = true;
			timeMinuteCount++;
			if (timeMinuteCount > 8)
				timeMinuteCount = 8;
		}
		Game::Find("TimeCount")->particleOverlayMat.mainTexture = MAP_ZERO + timeMinuteCount;
		initialGameTime = timeMinuteCount * 60;
		break;
	}

	if ((abs(Input::GetXBoxAxis(1, ButtonCode::XBOX_JOY_LEFT_HORIZONTAL)) +
		abs(Input::GetXBoxAxis(1, ButtonCode::XBOX_JOY_LEFT_VERTICAL))) < 0.5)
		menuChangePrev = false;
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