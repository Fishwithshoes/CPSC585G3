#include "Game.h"
#include "GeoGenerator.h"
#include "Loader.h"
#include "Physics.h"

GameObject Game::skybox = GameObject();
vector<GameObject> Game::staticObjectList = {};
vector<GameObject> Game::worldObjectList = {};
vector<ParticleSystem> Game::particleObjectList = {};
vector<GameObject> Game::overlayObjectList = {};
vector<GameObject> Game::aiObjectList = {};
vector<vec3> Game::plVehStartPositions = {};
vector<vec3> Game::aiVehStartPositions = {};

void Game::BuildWorld()
{
	//Pre-allocate space in vectors
	staticObjectList.reserve(100);
	worldObjectList.reserve(1000);
	particleObjectList.reserve(100);
	overlayObjectList.reserve(50);
	aiObjectList.reserve(100);
	aiVehStartPositions.reserve(2);
	aiVehStartPositions.reserve(5);

	plVehStartPositions.push_back(vec3(0.0, 20.0, -120.0));
	plVehStartPositions.push_back(vec3(0.0, 20.0, 120.0));

	aiVehStartPositions.push_back(vec3(-120.0, 20.0, 0.0));
	aiVehStartPositions.push_back(vec3(120.0, 20.0, 0.0));
	aiVehStartPositions.push_back(vec3(0.0, 20.0, 120.0));

	//Skybox
	skybox.mesh = GeoGenerator::MakeSphere(5000, 16, 32, true);
	skybox.particleOverlayMat.mainTexture = MAP_ENV;

	//Add initial World GameObjects
	GameObject temp;//Change props then create with this
	GameObject *ptr;//Assign the result of creation to this and then add components

	ParticleSystem tempP;//Change props then create particle systemes with this
	ParticleSystem *ptrP;//Add components just as with basic gameObjects with this

	//temp = GameObject();
	//temp.mesh = GeoGenerator::MakeBox(100, 2, 100, false);
	//temp.standardMat.diffuseMap = MAP_CHASSIS_DIFFUSE;
	//temp.standardMat.bumpLevel = 2;
	//temp.standardMat.normalMap = MAP_CHASSIS_NORMAL;
	//temp.standardMat.tileUV = vec2(12, 12);
	//ptr = Game::CreateWorldObject(temp);

	/*temp = GameObject("Thunderbowl", TAGS_DECORATION);
	temp.staticGeo = SG_MAP;
	//temp.standardMat.diffuseColor = vec3(1,0,0);
	//temp.castShadow = false;
	temp.standardMat.metalness = 0.5;
	temp.standardMat.isMetallic = true;
	temp.standardMat.roughness = 0.25;
	temp.standardMat.diffuseMap = MAP_CHASSIS_DIFFUSE;
	temp.standardMat.normalMap = MAP_CHASSIS_NORMAL;
	temp.standardMat.tileUV = vec2(4, 4);
	Game::CreateStaticObject(temp);*/

	//ifndef Car Cacophony
	/*temp = GameObject();
	temp.mesh = GeoGenerator::MakeBox(2, 1, 2, false);
	temp.name = "Player1";
	ptr = Game::CreateWorldObject(temp);
	ptr->AddComponent(new VehicleComponent());
	ptr->AddComponent(new MachineGunComponent());*/

	GameObject pathNode1 = GameObject();
	pathNode1.name = "Node1";
	pathNode1.transform.position = vec3(-150.0, 1.0, 0.0);
	pathNode1.mesh = GeoGenerator::MakeBox(1, 1, 1, false);
	pathNode1.tag = TAGS_AI_NODE;
	ptr = Game::CreateWorldObject(pathNode1);
	ptr->AddComponent(new AINodeComponent());

	GameObject pathNode2 = GameObject();
	pathNode2.name = "Node2";
	pathNode2.transform.position = vec3(150.0, 1.0, 0.0);
	pathNode2.mesh = GeoGenerator::MakeBox(1, 1, 1, false);
	pathNode2.tag = TAGS_AI_NODE;
	ptr = Game::CreateWorldObject(pathNode2);
	ptr->AddComponent(new AINodeComponent());

	GameObject pathNode3 = GameObject();
	pathNode3.name = "Node3";
	pathNode3.transform.position = vec3(0.0, 1.0, -150.0);
	pathNode3.mesh = GeoGenerator::MakeBox(1, 1, 1, false);
	pathNode3.tag = TAGS_AI_NODE;
	ptr = Game::CreateWorldObject(pathNode3);
	ptr->AddComponent(new AINodeComponent());

	GameObject pathNode4 = GameObject();
	pathNode4.name = "Node4";
	pathNode4.transform.position = vec3(0.0, 1.0, 150.0);
	pathNode4.mesh = GeoGenerator::MakeBox(1, 1, 1, false);
	pathNode4.tag = TAGS_AI_NODE;
	ptr = Game::CreateWorldObject(pathNode4);
	ptr->AddComponent(new AINodeComponent());

	for (int i = 0; i < Physics::playerVehiclesNum; i++) {								//CREATE PLAYERS
		GameObject player = GameObject();
		player.mesh = GeoGenerator::MakeBox(3, 1, 3, false);
		player.transform.position = plVehStartPositions[i];
		player.name = "Player" + to_string(i);
		player.tag = TAGS_HUMAN_PLAYER;
		ptr = Game::CreateWorldObject(player);
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

	for (int i = 0; i < Physics::opponentVehiclesNum; i++) {								//CREATE AI
		GameObject opponent = GameObject();
		opponent.mesh = GeoGenerator::MakeBox(2, 1, 2, false);
		opponent.transform.position = aiVehStartPositions[i];
		opponent.transform.Rotate(Transform::Up(), Mathf::PI, false);
		opponent.name = "AI" + to_string(i);
		opponent.tag = TAGS_AI_PLAYER;
		ptr = Game::CreateWorldObject(opponent);
		ptr->AddComponent(new PlayerComponent());
		ptr->AddComponent(new EnemyComponent());
		ptr->AddComponent(new MachineGunComponent());
		ptr->AddComponent(new AIControlComponent1());
		ptr->AddComponent(new HealthComponent());
	}

	GameObject powerUp1 = GameObject();
	powerUp1.name = "Node5";
	powerUp1.mesh = GeoGenerator::MakeBox(2, 2, 2, false);
	powerUp1.transform.position = vec3(0.0, 1.0, -80.0);
	powerUp1.tag = TAGS_AI_NODE;
	ptr = Game::CreateWorldObject(powerUp1);
	ptr->AddComponent(new PowerUpComponent(GW_MACHINE_GUN));
	ptr->AddComponent(new FloatComponent());
	ptr->AddComponent(new RotateComponent());
	ptr->AddComponent(new AINodeComponent(NodeTypes::NT_MG_POWERUP));

	GameObject powerUp2 = GameObject();
	powerUp2.name = "Node6";
	powerUp2.mesh = GeoGenerator::MakeBox(2, 2, 2, false);
	powerUp2.transform.position = vec3(0.0, 1.0, 80.0);
	powerUp2.tag = TAGS_AI_NODE;
	ptr = Game::CreateWorldObject(powerUp2);
	ptr->AddComponent(new PowerUpComponent(GW_MACHINE_GUN));
	ptr->AddComponent(new FloatComponent());
	ptr->AddComponent(new RotateComponent());
	ptr->AddComponent(new AINodeComponent(NodeTypes::NT_MG_POWERUP));

	GameObject powerUp3 = GameObject();
	powerUp3.name = "Node7";
	powerUp3.mesh = GeoGenerator::MakeBox(2, 2, 2, false);
	powerUp3.transform.position = vec3(-80.0, 1.0, 0.0);
	powerUp3.tag = TAGS_AI_NODE;
	ptr = Game::CreateWorldObject(powerUp3);
	ptr->AddComponent(new PowerUpComponent(GW_FLAMETHROWER));
	ptr->AddComponent(new FloatComponent());
	ptr->AddComponent(new RotateComponent());
	ptr->AddComponent(new AINodeComponent(NodeTypes::NT_FT_POWERUP));

	GameObject powerUp4 = GameObject();
	powerUp4.name = "Node8";
	powerUp4.mesh = GeoGenerator::MakeBox(2, 2, 2, false);
	powerUp4.transform.position = vec3(80.0, 1.0, 0.0);
	powerUp4.tag = TAGS_AI_NODE;
	ptr = Game::CreateWorldObject(powerUp4);
	ptr->AddComponent(new PowerUpComponent(GW_FLAMETHROWER));
	ptr->AddComponent(new FloatComponent());
	ptr->AddComponent(new RotateComponent());
	ptr->AddComponent(new AINodeComponent(NodeTypes::NT_FT_POWERUP));

	GameObject powerUp5 = GameObject();
	powerUp5.name = "Node9";
	powerUp5.mesh = GeoGenerator::MakeBox(2, 2, 2, false);
	powerUp5.transform.position = vec3(0.0, 1.0, 0.0);
	powerUp5.tag = TAGS_AI_NODE;
	ptr = Game::CreateWorldObject(powerUp5);
	ptr->AddComponent(new PowerUpComponent(GW_MISSILE_LAUNCHER));
	ptr->AddComponent(new FloatComponent());
	ptr->AddComponent(new RotateComponent());
	ptr->AddComponent(new AINodeComponent(NodeTypes::NT_RK_POWERUP));

	for (int i = 0; i < Physics::playerVehiclesNum; i++) {								//CREATE PLAYERS
		GameObject player = GameObject();
		player.mesh = GeoGenerator::MakeBox(3, 1, 3, false);
		player.transform.position = plVehStartPositions[i];
		player.name = "Player" + to_string(i);
		player.tag = TAGS_HUMAN_PLAYER;
		ptr = Game::CreateWorldObject(player);
		ptr->AddComponent(new PlayerComponent());
		ptr->AddComponent(new VehicleComponent());
		ptr->AddComponent(new MachineGunComponent());
		ptr->AddComponent(new MissileLauncherComponent());
		ptr->AddComponent(new FlamethrowerComponent());
		ptr->AddComponent(new HealthComponent());
	}

	for (int i = 0; i < Physics::opponentVehiclesNum; i++) {								//CREATE AI
		GameObject opponent = GameObject();
		opponent.mesh = GeoGenerator::MakeBox(2, 1, 2, false);
		opponent.transform.position = aiVehStartPositions[i];
		opponent.transform.Rotate(Transform::Up(), Mathf::PI, false);
		opponent.name = "AI" + to_string(i);
		opponent.tag = TAGS_AI_PLAYER;
		ptr = Game::CreateWorldObject(opponent);
		ptr->AddComponent(new PlayerComponent());
		ptr->AddComponent(new EnemyComponent());
		ptr->AddComponent(new MachineGunComponent());
		ptr->AddComponent(new AIControlComponent1());
		ptr->AddComponent(new HealthComponent());
	}

	/*GameObject obstacle1 = GameObject();
	obstacle1.mesh = GeoGenerator::MakeBox(16.0, 40.0, 16.0, false);
	obstacle1.transform.position = vec3(-25.0, 5.0, -25.0);
	ptr = Game::CreateWorldObject(obstacle1);
	ptr->AddComponent(new StaticComponent());

	GameObject obstacle2 = GameObject();
	obstacle2.mesh = GeoGenerator::MakeBox(16.0, 40.0, 16.0, false);
	obstacle2.transform.position = vec3(25.0, 5.0, -25.0);
	ptr = Game::CreateWorldObject(obstacle2);
	ptr->AddComponent(new StaticComponent());

	GameObject obstacle3 = GameObject();
	obstacle3.mesh = GeoGenerator::MakeBox(16.0, 40.0, 16.0, false);
	obstacle3.transform.position = vec3(-25.0, 5.0, 25.0);
	ptr = Game::CreateWorldObject(obstacle3);
	ptr->AddComponent(new StaticComponent());

	GameObject obstacle4 = GameObject();
	obstacle4.mesh = GeoGenerator::MakeBox(16.0, 40.0, 16.0, false);
	obstacle4.transform.position = vec3(25.0, 5.0, 25.0);
	ptr = Game::CreateWorldObject(obstacle4);
	ptr->AddComponent(new StaticComponent());

	/*GameObject temp3 = GameObject();
	temp3.mesh = GeoGenerator::MakeBox(2, 2, 2, false);
	ptr = Game::CreateWorldObject(temp2);
	ptr->AddComponent(new EnemyComponent());*/
	//ptr->AddComponent(new HealthComponent());

	//temp = GameObject();
	//temp.mesh = GeoGenerator::MakeCylinder(5, 5, 100, 32, true);
	//temp.transform.position = vec3(70, 0, 50);
	//temp.standardMat.roughness = 0.5;
	//temp.standardMat.metalness = 0.2;
	//temp.standardMat.diffuseMap = MAP_CHASSIS_DIFFUSE;
	//temp.standardMat.normalMap = MAP_CHASSIS_NORMAL;
	//temp.standardMat.bumpLevel = 1.4;
	//temp.standardMat.tileUV = vec2(2, 10);
	//ptr = Game::CreateWorldObject(temp);
	//ptr->AddComponent(new RigidStaticComponent(5, 100));

	//temp = GameObject();
	//temp.mesh = GeoGenerator::MakeSphere(1, 32, 64, false);
	//temp.name = "BigGoldAndShiny";
	//temp.transform.scale = vec3(5);
	//temp.transform.position = vec3(10, 5, 10);
	//temp.standardMat.diffuseColor = vec3(1.0, 1.0, 1.0);
	//temp.standardMat.roughness = 0.0;
	//temp.standardMat.metalness = 0.04;
	//temp.standardMat.isMetallic = true;
	//temp.standardMat.transparency = 1.0;
	//temp.standardMat.IOR = 0.33;
	//temp.standardMat.refractColor = vec3(0.0, 1.0, 1.0);
	//temp.standardMat.isPhysicalTransparency = true;
	//temp.getGrabPass = true;
	//ptr = Game::CreateWorldObject(temp);

	/*tempP = ParticleSystem();
	tempP.name = "testParticleSystemDefault";
	tempP.tag = TAGS_PARTICLE_SYSTEM;
	tempP.transform.position.y = 6;
	tempP.useSystemLifespan = false;
	//tempP.AddParticleBurst(100, 3);
	Game::CreateParticleObject(tempP);

	tempP = ParticleSystem();
	tempP.name = "testParticleSystemSmoke";
	tempP.tag = TAGS_PARTICLE_SYSTEM;
	tempP.spawnRate = 40;
	tempP.initialRadius.min = 0.15;
	tempP.initialRadius.max = 0.25;
	tempP.initialSpeed.min = 0.0;
	tempP.initialSpeed.max = 0.2;
	tempP.spawnPointVariance = vec3(0.5, 0.2, 0.5);
	tempP.initialColor.alpha = vec4(0.1, 0.1, 0.1, 1);
	tempP.initialColor.bravo = vec4(0.4, 0.4, 0.4, 1);
	tempP.monochromatic = true;
	tempP.transform.position = vec3(-3, 3, 0);
	tempP.gravityScale = -2;
	tempP.mainTexture = MAP_SMOKE_PART;
	tempP.useSystemLifespan = false;
	Game::CreateParticleObject(tempP);

	tempP = ParticleSystem();
	tempP.name = "testParticleSystemBubbles";
	tempP.tag = TAGS_PARTICLE_SYSTEM;
	tempP.spawnRate = 10;
	tempP.initialColor.alpha = vec4(1, 0.4, 1, 1);
	tempP.initialColor.bravo = vec4(1, 0.8, 1, 1);
	tempP.transform.position.y = 6;
	tempP.transform.position.x = -8;
	tempP.mainTexture = MAP_BUBBLE_PART;
	tempP.useSystemLifespan = false;
	Game::CreateParticleObject(tempP);*/
	
	//temp = GameObject();
	//temp.mesh = GeoGenerator::MakeCylinder(1, 2, 32);
	//temp.transform.Translate(vec3(3, 2, 0));
	//Game::CreateWorldObject(temp);

	temp = GameObject("Sun", TAGS_DECORATION);
	temp.mesh = GeoGenerator::MakeSphere(250, 16, 32, false);
	temp.transform.position = vec3(5, 2, 5)*560.0f;
	temp.standardMat.roughness = 1.0;
	temp.standardMat.metalness = 0.00;
	temp.standardMat.diffuseLevel = 0.0;
	temp.standardMat.selfIllumLevel = 2.0;
	temp.standardMat.selfIllumColor = vec3(1.0, 0.5, 0.1);
	temp.standardMat.fogLevel = 0.0;
	Game::CreateWorldObject(temp);

	temp = GameObject("LightSymbol", TAGS_DEBUG_2);
	temp.mesh = GeoGenerator::MakeSphere(20, 4, 8, false);
	temp.transform.position = vec3(5, 2, 5)*50.0f;
	temp.standardMat.roughness = 1.0;
	temp.standardMat.metalness = 0.00;
	temp.standardMat.diffuseLevel = 0.0;
	temp.standardMat.selfIllumLevel = 3.0;
	temp.standardMat.selfIllumColor = vec3(1.0, 0.0, 0.0);
	temp.standardMat.fogLevel = 0.0;
	//Game::CreateWorldObject(temp);

	temp = GameObject("Moon", TAGS_DECORATION);
	temp.mesh = GeoGenerator::MakeSphere(100, 16, 32, false);
	temp.transform.position = vec3(-4, 1, -4)*700.0f;
	temp.standardMat.roughness = 1.0;
	temp.standardMat.metalness = 0.00;
	temp.standardMat.diffuseLevel = 0.0;
	temp.standardMat.diffuseMap = MAP_MOON_DIFFUSE;
	temp.standardMat.selfIllumLevel = 1.4;
	temp.standardMat.selfIllumColor = vec3(0.8, 0.8, 1.0);
	temp.standardMat.normalMap = MAP_MOON_NORMAL;
	temp.standardMat.bumpLevel = 0.5;
	temp.standardMat.fogLevel = 0.0;
	Game::CreateWorldObject(temp);

	/*temp = GameObject("OceanTop", TAGS_DECORATION);
	temp.staticGeo = SG_OCEAN;
	temp.castShadow = false;
	temp.transform.position.y = 8;
	temp.standardMat.diffuseColor = vec3(0.0, 1.0, 1.0)*0.5f;
	temp.standardMat.roughness = 0.0;
	temp.standardMat.metalness = 0.10;
	temp.standardMat.transparency = 0.4;
	temp.standardMat.normalMap = MAP_WATER_NORMAL;
	temp.standardMat.bumpLevel = 0.2;
	temp.standardMat.tileUV = vec2(10, 10);
	temp.getGrabPass = true;
	ptr = Game::CreateStaticObject(temp);
	ptr->AddComponent(new OceanComponent());*/

	/*temp = GameObject("OceanBottom", TAGS_DECORATION);
	temp.staticGeo = SG_OCEAN_DOWN;
	temp.castShadow = false;
	temp.transform.position.y = 8;
	temp.standardMat.diffuseColor = vec3(0.0, 1.0, 1.0)*0.5f;
	temp.standardMat.roughness = 0.0;
	temp.standardMat.metalness = 0.10;
	temp.standardMat.transparency = 0.4;
	temp.standardMat.normalMap = MAP_WATER_NORMAL;
	temp.standardMat.bumpLevel = 0.2;
	temp.standardMat.tileUV = vec2(10, 10);
	ptr = Game::CreateStaticObject(temp);
	temp.getGrabPass = true;
	ptr->AddComponent(new OceanComponent());*/

	/*temp = GameObject("Puddle", TAGS_DECORATION);
	temp.staticGeo = SG_PUDDLE;
	temp.transform.position.y = 1;
	temp.standardMat.diffuseColor = vec3(0.3, 0.3, 0.5)*0.5f;
	temp.standardMat.roughness = 0.0;
	temp.standardMat.metalness = 0.20;
	temp.standardMat.normalMap = MAP_WATER_NORMAL;
	temp.standardMat.bumpLevel = 0.2;
	temp.standardMat.tileUV = vec2(2, 2);*/
	//ptr = Game::CreateStaticObject(temp);
	//ptr->AddComponent(new PuddleComponent());

//OVERLAY GAMEOBJECTS - FRONT MENU
	temp = GameObject("StartMenuBack", TAGS_MENU);
	temp.mesh = GeoGenerator::MakeRect(1.0, 0.40, GA_CENTER);
	temp.transform.Translate(vec3(0.0, -0.7, 0.0), false);
	temp.particleOverlayMat.color = vec4(0, 0, 0, 0.5);
	//Game::CreateOverlayObject(temp);

	temp = GameObject("StartMenuButton", TAGS_MENU);
	temp.mesh = GeoGenerator::MakeRect(0.95, 0.45, GA_CENTER);
	temp.transform.Translate(vec3(0.0, -0.7, -0.1), false);
	temp.particleOverlayMat.mainTexture = MAP_START;
	temp.particleOverlayMat.color = vec4(1, 1, 1, 1.0);
	Game::CreateOverlayObject(temp);
	
//OVERLAY GAMEOBJECTS - HUD
	//NOW CREATED IN GameManager::StartGame() as of v0.3.15

//OVERLAY GAMEOBJECTS - PAUSE
	temp = GameObject("PauseMenuBack", TAGS_PAUSE);
	temp.mesh = GeoGenerator::MakeRect(1.0, 0.40, GA_CENTER);
	temp.transform.Translate(vec3(0.0, 0.0, -0.6), false);
	temp.particleOverlayMat.color = vec4(0, 0, 0, 0.25);
	//Game::CreateOverlayObject(temp);

	temp = GameObject("PauseMenuButton", TAGS_PAUSE);
	temp.mesh = GeoGenerator::MakeRect(0.95, 0.35, GA_CENTER);
	temp.transform.Translate(vec3(0.0, 0.0, -0.7), false);
	temp.particleOverlayMat.mainTexture = MAP_PAUSED;
	temp.particleOverlayMat.color = vec4(1, 1, 1, 1.0);
	Game::CreateOverlayObject(temp);

	//IF_DEF HIDE PAUSE
	vector<GameObject*> pauseItems = FindGameObjectsWithTag(TAGS_PAUSE);
	for (int i = 0; i < pauseItems.size(); i++)
		pauseItems[i]->isVisible = false;
	//END_IF HIDE PAUSE

//OVERLAY GAMEOBJECTS - GAME OVER
	temp = GameObject("GameOverBack", TAGS_GAME_OVER);
	temp.mesh = GeoGenerator::MakeRect(1.0, 0.50, GA_CENTER);
	temp.transform.Translate(vec3(0.0, 0.7, -0.0), false);
	temp.particleOverlayMat.color = vec4(0, 0, 0, 1.0);	
	//Game::CreateOverlayObject(temp);

	temp = GameObject("GameOver", TAGS_GAME_OVER);
	temp.mesh = GeoGenerator::MakeRect(0.95, 0.55, GA_CENTER);
	temp.transform.Translate(vec3(0.0, 0.8, -0.01), false);
	temp.particleOverlayMat.mainTexture = MAP_GAME_OVER;
	temp.particleOverlayMat.color = vec4(1, 0, 0, 1.0);
	Game::CreateOverlayObject(temp);

	for (int i = 0; i < Physics::totalVehiclesNum; i++)
	{
		float offset = -i*0.2;
		float layer = -i*0.1;

		vec4 nameColor = i < Physics::playerVehiclesNum ? vec4(1) : vec4(vec3(0.6), 1);

		temp = GameObject("PlayerScoreBack" + i, TAGS_GAME_OVER);
		temp.mesh = GeoGenerator::MakeRect(1.5, 0.18, GA_CENTER);
		temp.transform.Translate(vec3(0.0, 0.6 + offset, -0.10 + layer), false);
		temp.particleOverlayMat.color = vec4(0, 0, 0, 0.5);
		Game::CreateOverlayObject(temp);

		temp = GameObject("PlayerScoreName-0-" + i, TAGS_GAME_OVER);
		temp.mesh = GeoGenerator::MakeRect(0.2, 0.15, GA_LEFT);
		temp.transform.Translate(vec3(-0.40, 0.6 + offset, -0.11 + layer), false);
		temp.particleOverlayMat.mainTexture = MAP_ZERO;
		temp.particleOverlayMat.color = nameColor;
		Game::CreateOverlayObject(temp);

		temp = GameObject("PlayerScoreName-1-" + i, TAGS_GAME_OVER);
		temp.mesh = GeoGenerator::MakeRect(0.2, 0.15, GA_LEFT);
		temp.transform.Translate(vec3(-0.33, 0.6 + offset, -0.12 + layer), false);
		temp.particleOverlayMat.mainTexture = MAP_ONE + i;
		temp.particleOverlayMat.color = nameColor;
		Game::CreateOverlayObject(temp);

		temp = GameObject("PlayerScoreLabel" + i, TAGS_GAME_OVER);
		temp.mesh = GeoGenerator::MakeRect(0.3, 0.3, GA_LEFT);
		temp.transform.Translate(vec3(-0.02, 0.6 + offset, -0.13 + layer), false);
		temp.particleOverlayMat.mainTexture = MAP_SCORE;
		temp.particleOverlayMat.color = nameColor;
		Game::CreateOverlayObject(temp);

		temp = GameObject("PlayerScore-1-" + i, TAGS_GAME_OVER);
		temp.mesh = GeoGenerator::MakeRect(0.16, 0.12, GA_LEFT);
		temp.transform.Translate(vec3(0.15, 0.6 + offset, -0.14 + layer), false);
		temp.particleOverlayMat.mainTexture = MAP_ZERO;
		temp.particleOverlayMat.color = nameColor;
		Game::CreateOverlayObject(temp);

		temp = GameObject("PlayerScore-2-" + i, TAGS_GAME_OVER);
		temp.mesh = GeoGenerator::MakeRect(0.15, 0.12, GA_LEFT);
		temp.transform.Translate(vec3(0.20, 0.6 + offset, -0.15 + layer), false);
		temp.particleOverlayMat.mainTexture = MAP_ZERO;
		temp.particleOverlayMat.color = nameColor;
		Game::CreateOverlayObject(temp);

		temp = GameObject("PlayerScore-3-" + i, TAGS_GAME_OVER);
		temp.mesh = GeoGenerator::MakeRect(0.15, 0.12, GA_LEFT);
		temp.transform.Translate(vec3(0.25, 0.6 + offset, -0.16 + layer), false);
		temp.particleOverlayMat.mainTexture = MAP_ZERO;
		temp.particleOverlayMat.color = nameColor;
		Game::CreateOverlayObject(temp);

		temp = GameObject("PlayerScore-4-" + i, TAGS_GAME_OVER);
		temp.mesh = GeoGenerator::MakeRect(0.15, 0.12, GA_LEFT);
		temp.transform.Translate(vec3(0.30, 0.6 + offset, -0.17 + layer), false);
		temp.particleOverlayMat.mainTexture = MAP_ZERO;
		temp.particleOverlayMat.color = nameColor;
		Game::CreateOverlayObject(temp);
	}

	//IF_DEF HIDE GAME OVER
	vector<GameObject*> gameOverItems = FindGameObjectsWithTag(TAGS_GAME_OVER);
	for (int i = 0; i < gameOverItems.size(); i++)
		gameOverItems[i]->isVisible = false;
	//END_IF HIDE GAME OVER

	//temp = GameObject();
	//temp.mesh = GeoGenerator::MakeCircle(1.0, 32, 0.2);
	//Game::CreateOverlayObject(temp);
}

void Game::DestroyWorld()
{
	for (int i = staticObjectList.size() - 1; i >= 0; i--)
	{
		DestroyStaticObjectAt(i);
	}
	for (int i = worldObjectList.size() - 1; i >= 0; i--)
	{
		DestroyWorldObjectAt(i);
	}
	for (int i = particleObjectList.size() - 1; i >= 0; i--)
	{
		DestroyParticleObjectAt(i);
	}
	for (int i = overlayObjectList.size() - 1; i >= 0; i--)
	{
		DestroyOverlayObjectAt(i);
	}
}

void Game::KillAllParticles()
{
	for (int i = particleObjectList.size() - 1; i >= 0; i--)
	{
		DestroyParticleObjectAt(i);
	}
}

//CREATORS
GameObject* Game::CreateStaticObject(GameObject object)
{
	object.objectID = staticObjectList.size();
	object.Start();
	staticObjectList.push_back(object);
	return &staticObjectList[staticObjectList.size() - 1];
}
GameObject* Game::CreateWorldObject(GameObject object)
{
	object.objectID = worldObjectList.size();
	object.Start();
	worldObjectList.push_back(object);
	return &worldObjectList[worldObjectList.size() - 1];
}
ParticleSystem* Game::CreateParticleObject(ParticleSystem object)
{
	object.objectID = particleObjectList.size();
	object.Start();
	particleObjectList.push_back(object);
	return &particleObjectList[particleObjectList.size() - 1];
}
GameObject* Game::CreateOverlayObject(GameObject object)
{
	object.objectID = overlayObjectList.size();
	object.Start();
	overlayObjectList.push_back(object);
	return &overlayObjectList[overlayObjectList.size() - 1];
}
/*GameObject* Game::CreateAIObject(GameObject object)
{
	object.objectID = aiObjectList.size();
	object.Start();
	aiObjectList.push_back(object);
	return &aiObjectList[aiObjectList.size() - 1];
}*/

//DESTROYERS
void Game::DestroyStaticObjectAt(int objectID)
{
	staticObjectList[objectID].RemoveComponents();
	staticObjectList.erase(staticObjectList.begin() + objectID);
	for (int i = 0; i < staticObjectList.size(); i++)
	{
		staticObjectList[i].objectID = i;

		for (int j = 0; j < staticObjectList[i].componentList.size(); j++)
		{
			staticObjectList[i].componentList[j]->SetSelfName(&staticObjectList[i].name);
			staticObjectList[i].componentList[j]->SetMesh(&staticObjectList[i].mesh);
			staticObjectList[i].componentList[j]->SetTransform(&staticObjectList[i].transform);
			staticObjectList[i].componentList[j]->SetStandardMaterial(&staticObjectList[i].standardMat);
			staticObjectList[i].componentList[j]->SetParticleOverlayMaterial(&staticObjectList[i].particleOverlayMat);
		}
	}
	for (int i = 0; i < staticObjectList.size(); i++)
	{
		for (int j = 0; j < staticObjectList[i].componentList.size(); j++)
			staticObjectList[i].componentList[j]->SetSelfID(&staticObjectList[i].objectID);
	}
}
void Game::DestroyWorldObjectAt(int objectID)
{
	worldObjectList[objectID].RemoveComponents();
	worldObjectList.erase(worldObjectList.begin() + objectID);
	for (int i = 0; i < worldObjectList.size(); i++)
	{
		worldObjectList[i].objectID = i;

		for (int j = 0; j < worldObjectList[i].componentList.size(); j++)
		{
			worldObjectList[i].componentList[j]->SetSelfName(&worldObjectList[i].name);
			worldObjectList[i].componentList[j]->SetMesh(&worldObjectList[i].mesh);
			worldObjectList[i].componentList[j]->SetTransform(&worldObjectList[i].transform);
			worldObjectList[i].componentList[j]->SetStandardMaterial(&worldObjectList[i].standardMat);
			worldObjectList[i].componentList[j]->SetParticleOverlayMaterial(&worldObjectList[i].particleOverlayMat);
		}
	}
	for (int i = 0; i < worldObjectList.size(); i++)
	{
		for (int j = 0; j < worldObjectList[i].componentList.size(); j++)
			worldObjectList[i].componentList[j]->SetSelfID(&worldObjectList[i].objectID);
	}
}
void Game::DestroyParticleObjectAt(int objectID)
{
	particleObjectList[objectID].RemoveComponents();
	particleObjectList.erase(particleObjectList.begin() + objectID);
	for (int i = 0; i < particleObjectList.size(); i++)
	{
		particleObjectList[i].objectID = i;

		for (int j = 0; j < particleObjectList[i].componentList.size(); j++)
		{
			particleObjectList[i].componentList[j]->SetSelfID(&i);
			particleObjectList[i].componentList[j]->SetSelfName(&particleObjectList[i].name);
			particleObjectList[i].componentList[j]->SetMesh(&particleObjectList[i].mesh);
			particleObjectList[i].componentList[j]->SetTransform(&particleObjectList[i].transform);
			particleObjectList[i].componentList[j]->SetStandardMaterial(&particleObjectList[i].standardMat);
			particleObjectList[i].componentList[j]->SetParticleOverlayMaterial(&particleObjectList[i].particleOverlayMat);
		}
	}
	for (int i = 0; i < particleObjectList.size(); i++)
	{
		for (int j = 0; j < particleObjectList[i].componentList.size(); j++)
			particleObjectList[i].componentList[j]->SetSelfID(&particleObjectList[i].objectID);
	}
}
void Game::DestroyOverlayObjectAt(int objectID)
{
	overlayObjectList[objectID].RemoveComponents();
	overlayObjectList.erase(overlayObjectList.begin() + objectID);
	for (int i = 0; i < overlayObjectList.size(); i++)
	{
		overlayObjectList[i].objectID = i;

		for (int j = 0; j < overlayObjectList[i].componentList.size(); j++)
		{
			overlayObjectList[i].componentList[j]->SetSelfName(&overlayObjectList[i].name);
			overlayObjectList[i].componentList[j]->SetMesh(&overlayObjectList[i].mesh);
			overlayObjectList[i].componentList[j]->SetTransform(&overlayObjectList[i].transform);
			overlayObjectList[i].componentList[j]->SetStandardMaterial(&overlayObjectList[i].standardMat);
			overlayObjectList[i].componentList[j]->SetParticleOverlayMaterial(&overlayObjectList[i].particleOverlayMat);
		}
	}
	for (int i = 0; i < overlayObjectList.size(); i++)
	{
		for (int j = 0; j < overlayObjectList[i].componentList.size(); j++)
			overlayObjectList[i].componentList[j]->SetSelfID(&overlayObjectList[i].objectID);
	}
}

/*void Game::DestroyAIObjectAt(int objectID)
{
	GameObject object = aiObjectList[objectID];
	aiObjectList.erase(aiObjectList.begin() + object.objectID);
	for (int i = 0; i < aiObjectList.size(); i++)
	{
		if (aiObjectList[i].objectID > object.objectID)
			aiObjectList[i].objectID--;
	}
	object.RemoveComponents();
}*/

//THE FIND ENGINE
GameObject* Game::Find(string name)
{
	for (int i = 0; i < worldObjectList.size(); i++)
	{
		if (name == worldObjectList[i].name)
			return &worldObjectList[i];
	}

	for (int i = 0; i < staticObjectList.size(); i++)
	{
		if (name == staticObjectList[i].name)
			return &staticObjectList[i];
	}

	for (int i = 0; i < particleObjectList.size(); i++)
	{
		if (name == particleObjectList[i].name)
			return &particleObjectList[i];
	}

	for (int i = 0; i < overlayObjectList.size(); i++)
	{
		if (name == overlayObjectList[i].name)
			return &overlayObjectList[i];
	}
	/*for (int i = 0; i < aiObjectList.size(); i++)
	{
		if (name == aiObjectList[i].name)
			return &aiObjectList[i];
	}*/

	cout << "GameObject with name: " << name << " not found! Check thy spelling?" << endl;
	return nullptr;//Nothing was found
}

vector<GameObject*> Game::FindGameObjectsWithTag(Tags tag)
{
	vector<GameObject*> result;

	for (int i = 0; i < worldObjectList.size(); i++)
	{
		if (tag == worldObjectList[i].tag)
			result.push_back(&worldObjectList[i]);
	}

	for (int i = 0; i < staticObjectList.size(); i++)
	{
		if (tag == staticObjectList[i].tag)
			result.push_back(&staticObjectList[i]);
	}

	for (int i = 0; i < particleObjectList.size(); i++)
	{
		if (tag == particleObjectList[i].tag)
			result.push_back(&particleObjectList[i]);
	}

	for (int i = 0; i < overlayObjectList.size(); i++)
	{
		if (tag == overlayObjectList[i].tag)
			result.push_back(&overlayObjectList[i]);
	}

	return result;
}