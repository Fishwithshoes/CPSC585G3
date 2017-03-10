#include "Game.h"
#include "GeoGenerator.h"
#include "Loader.h"

GameObject Game::skybox = GameObject();
vector<GameObject> Game::staticObjectList = {};
vector<GameObject> Game::worldObjectList = {};
vector<ParticleSystem> Game::particleObjectList = {};
vector<GameObject> Game::overlayObjectList = {};
vector<GameObject> Game::aiObjectList = {};

void Game::BuildWorld()
{
	//Pre-allocate space in vectors
	staticObjectList.reserve(100);
	worldObjectList.reserve(1000);
	particleObjectList.reserve(100);
	overlayObjectList.reserve(50);
	aiObjectList.reserve(100);

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

	temp = GameObject("Thunderbowl", TAGS_DECORATION);
	temp.staticGeo = SG_MAP;
	//temp.standardMat.diffuseColor = vec3(1,0,0);
	temp.standardMat.metalness = 0.5;
	temp.standardMat.isMetallic = true;
	temp.standardMat.roughness = 0.25;
	temp.standardMat.diffuseMap = MAP_CHASSIS_DIFFUSE;
	temp.standardMat.normalMap = MAP_CHASSIS_NORMAL;
	temp.standardMat.tileUV = vec2(4, 4);
	Game::CreateStaticObject(temp);

	//ifndef Car Cacophony
	/*temp = GameObject();
	temp.mesh = GeoGenerator::MakeBox(2, 1, 2, false);
	temp.name = "Player1";
	ptr = Game::CreateWorldObject(temp);
	ptr->AddComponent(new VehicleComponent());
	ptr->AddComponent(new MachineGunComponent());*/
	GameObject pathNode1 = GameObject();
	pathNode1.name = "Node1";
	pathNode1.transform.position = vec3(0.0, 1.0, 100.0);
	ptr = Game::CreateAIObject(pathNode1);
	ptr->AddComponent(new AINodeComponent());

	GameObject pathNode2 = GameObject();
	pathNode2.name = "Node2";
	pathNode2.transform.position = vec3(0.0, 1.0, -100.0);
	ptr = Game::CreateAIObject(pathNode2);
	ptr->AddComponent(new AINodeComponent());

	GameObject pathNode3 = GameObject();
	pathNode3.name = "Node3";
	pathNode3.transform.position = vec3(100.0, 1.0, 0.0);
	ptr = Game::CreateAIObject(pathNode3);
	ptr->AddComponent(new AINodeComponent());

	GameObject pathNode4 = GameObject();
	pathNode4.name = "Node4";
	pathNode4.transform.position = vec3(-100.0, 1.0, 0.0);
	ptr = Game::CreateAIObject(pathNode4);
	ptr->AddComponent(new AINodeComponent());

	GameObject player1 = GameObject("Player1", TAGS_HUMAN_PLAYER);
	player1.mesh = GeoGenerator::MakeBox(3, 1, 3, false);
	player1.transform.position = vec3(0.0, 20.0, -120.0);
	player1.name = "Player1";
	ptr = Game::CreateWorldObject(player1);
	ptr->AddComponent(new VehicleComponent());
	ptr->AddComponent(new MachineGunComponent());
	ptr->AddComponent(new MissileLauncherComponent());
	ptr->AddComponent(new FlamethrowerComponent());
	ptr->AddComponent(new PlayerComponent());

	GameObject opponent1 = GameObject("AI1", TAGS_AI_PLAYER);
	opponent1.mesh = GeoGenerator::MakeBox(2, 1, 2, false);
	opponent1.transform.position = vec3(0.0, 20.0, 120.0);
	opponent1.transform.Rotate(Transform::Up(), Mathf::PI, false);
	opponent1.name = "AI1";
	ptr = Game::CreateWorldObject(opponent1);
	ptr->AddComponent(new PlayerComponent());
	ptr->AddComponent(new EnemyComponent());
	ptr->AddComponent(new MachineGunComponent());
	ptr->AddComponent(new AIControlComponent1());
	ptr->AddComponent(new HealthComponent());

	GameObject powerUp1 = GameObject();
	powerUp1.mesh = GeoGenerator::MakeBox(2, 2, 2, false);
	powerUp1.transform.position = vec3(0.0, 10.0, -80.0);
	powerUp1.standardMat.selfIllumLevel = 1.0;
	powerUp1.standardMat.selfIllumColor = vec3(0.0, 1.0, 0.0);
	ptr = Game::CreateWorldObject(powerUp1);
	ptr->AddComponent(new PowerUpComponent());
	ptr->AddComponent(new FloatComponent());
	ptr->AddComponent(new RotateComponent());

	GameObject powerUp2 = GameObject();
	powerUp2.mesh = GeoGenerator::MakeBox(2, 2, 2, false);
	powerUp2.transform.position = vec3(0.0, 10.0, 80.0);
	powerUp2.standardMat.selfIllumLevel = 1.0;
	powerUp2.standardMat.selfIllumColor = vec3(0.0, 1.0, 0.0);
	ptr = Game::CreateWorldObject(powerUp2);
	ptr->AddComponent(new PowerUpComponent());
	ptr->AddComponent(new FloatComponent());
	ptr->AddComponent(new RotateComponent());

	GameObject powerUp3 = GameObject();
	powerUp3.mesh = GeoGenerator::MakeBox(2, 2, 2, false);
	powerUp3.transform.position = vec3(-80.0, 10.0, 0.0);
	powerUp3.standardMat.selfIllumLevel = 1.0;
	powerUp3.standardMat.selfIllumColor = vec3(0.0, 1.0, 0.0);
	ptr = Game::CreateWorldObject(powerUp3);
	ptr->AddComponent(new PowerUpComponent());
	ptr->AddComponent(new FloatComponent());
	ptr->AddComponent(new RotateComponent());

	GameObject powerUp4 = GameObject();
	powerUp4.mesh = GeoGenerator::MakeBox(2, 2, 2, false);
	powerUp4.transform.position = vec3(80.0, 10.0, 0.0);
	powerUp4.standardMat.selfIllumLevel = 1.0;
	powerUp4.standardMat.selfIllumColor = vec3(0.0, 1.0, 0.0);
	ptr = Game::CreateWorldObject(powerUp4);
	ptr->AddComponent(new PowerUpComponent());
	ptr->AddComponent(new FloatComponent());
	ptr->AddComponent(new RotateComponent());

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
	temp.transform.position = vec3(5, 2, 5)*650.0f;
	temp.standardMat.roughness = 1.0;
	temp.standardMat.metalness = 0.00;
	temp.standardMat.diffuseLevel = 0.0;
	temp.standardMat.selfIllumLevel = 2.0;
	temp.standardMat.selfIllumColor = vec3(1.0, 0.5, 0.1);
	temp.standardMat.fogLevel = 0.0;
	Game::CreateWorldObject(temp);

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

	temp = GameObject("OceanTop", TAGS_DECORATION);
	temp.staticGeo = SG_OCEAN;
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
	ptr->AddComponent(new OceanComponent());

	temp = GameObject("OceanBottom", TAGS_DECORATION);
	temp.staticGeo = SG_OCEAN_DOWN;
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
	ptr->AddComponent(new OceanComponent());

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
	//Health meter
	temp = GameObject("HealthMeterBack", TAGS_HUD);
	temp.mesh = GeoGenerator::MakeRect(1.05, 0.20, GA_LEFT);
	temp.transform.Translate(vec3(-0.2625, 0.8525, 0.0), false);
	temp.particleOverlayMat.color = vec4(0, 0, 0, 0.5);
	Game::CreateOverlayObject(temp);

	temp = GameObject("HealthMeter", TAGS_HUD);
	temp.mesh = GeoGenerator::MakeRect(1.00, 0.15, GA_LEFT);
	temp.transform.Translate(vec3(-0.25, 0.85, -0.1), false);
	temp.particleOverlayMat.color = vec4(0, 0.5, 1, 1);
	Game::CreateOverlayObject(temp);

	//Ammo meter
	temp = GameObject("AmmoMeterBack", TAGS_HUD);
	temp.mesh = GeoGenerator::MakeRect(1.05, 0.10, GA_LEFT);
	temp.transform.Translate(vec3(-0.2625, 0.7025, 0.0), false);
	temp.particleOverlayMat.color = vec4(0, 0, 0, 0.5);
	Game::CreateOverlayObject(temp);

	temp = GameObject("AmmoMeter", TAGS_HUD);
	temp.mesh = GeoGenerator::MakeRect(1.00, 0.05, GA_LEFT);
	temp.transform.Translate(vec3(-0.25, 0.70, -0.1), false);
	temp.particleOverlayMat.color = vec4(0.8, 0.4, 0.0, 1);
	Game::CreateOverlayObject(temp);

	//Speedometer
	temp = GameObject("SpeedometerBack", TAGS_HUD);
	temp.mesh = GeoGenerator::MakeRect(0.8, 0.5, GA_CENTER);
	temp.transform.Translate(vec3(0.73, -0.67, 0.0), false);
	temp.particleOverlayMat.color = vec4(0, 0, 0, 0.5);
	Game::CreateOverlayObject(temp);

	temp = GameObject("SpeedometerGauge", TAGS_HUD);
	temp.mesh = GeoGenerator::MakeRect(0.75, 0.45, GA_CENTER);
	temp.transform.Translate(vec3(0.73, -0.67, -0.1), false);
	temp.particleOverlayMat.color = vec4(0.5, 0.5, 0.5, 1.0);
	Game::CreateOverlayObject(temp);

	temp = GameObject("SpeedometerNeedle", TAGS_HUD);
	temp.mesh = GeoGenerator::MakeRect(0.35, 0.05, GA_RIGHT);
	temp.transform.Translate(vec3(0.73, -0.85, -0.2), false);
	temp.particleOverlayMat.color = vec4(1.0, 0.0, 0.0, 1.0);
	Game::CreateOverlayObject(temp);

	//Game Timer
	temp = GameObject("TimerBack", TAGS_HUD);
	temp.mesh = GeoGenerator::MakeRect(0.45, 0.1, GA_LEFT);
	temp.transform.Translate(vec3(-0.54, 0.9, 0.0), false);
	temp.particleOverlayMat.color = vec4(0, 0, 0, 0.5);
	Game::CreateOverlayObject(temp);

	temp = GameObject("TimerLabel", TAGS_HUD);
	temp.mesh = GeoGenerator::MakeRect(0.2, 0.2, GA_LEFT);
	temp.transform.Translate(vec3(-0.54, 0.9, -0.1), false);
	temp.particleOverlayMat.color = vec4(1.0, 1.0, 1.0, 1.0);
	temp.particleOverlayMat.mainTexture = MAP_TIME;
	Game::CreateOverlayObject(temp);

	temp = GameObject("Timer1", TAGS_HUD);
	temp.mesh = GeoGenerator::MakeRect(0.075, 0.075, GA_LEFT);
	temp.transform.Translate(vec3(-0.44, 0.9, -0.2), false);
	temp.particleOverlayMat.color = vec4(1.0, 1.0, 1.0, 1.0);
	temp.particleOverlayMat.mainTexture = MAP_ZERO;
	Game::CreateOverlayObject(temp);

	temp = GameObject("Timer2", TAGS_HUD);
	temp.mesh = GeoGenerator::MakeRect(0.075, 0.075, GA_LEFT);
	temp.transform.Translate(vec3(-0.40, 0.9, -0.3), false);
	temp.particleOverlayMat.color = vec4(1.0, 1.0, 1.0, 1.0);
	temp.particleOverlayMat.mainTexture = MAP_ZERO;
	Game::CreateOverlayObject(temp);

	temp = GameObject("Timer3", TAGS_HUD);
	temp.mesh = GeoGenerator::MakeRect(0.075, 0.075, GA_LEFT);
	temp.transform.Translate(vec3(-0.38, 0.9, -0.4), false);
	temp.particleOverlayMat.color = vec4(1.0, 1.0, 1.0, 1.0);
	temp.particleOverlayMat.mainTexture = MAP_ZERO;
	Game::CreateOverlayObject(temp);

	//Score
	temp = GameObject("ScoreBack", TAGS_HUD);
	temp.mesh = GeoGenerator::MakeRect(0.5, 0.1, GA_LEFT);
	temp.transform.Translate(vec3(0.35, 0.9, 0.0), false);
	temp.particleOverlayMat.color = vec4(0, 0, 0, 0.5);
	Game::CreateOverlayObject(temp);

	temp = GameObject("ScoreLabel", TAGS_HUD);
	temp.mesh = GeoGenerator::MakeRect(0.2, 0.2, GA_LEFT);
	temp.transform.Translate(vec3(0.36, 0.9, -0.1), false);
	temp.particleOverlayMat.color = vec4(1.0, 1.0, 1.0, 1.0);
	temp.particleOverlayMat.mainTexture = MAP_SCORE;
	Game::CreateOverlayObject(temp);

	temp = GameObject("Score1", TAGS_HUD);
	temp.mesh = GeoGenerator::MakeRect(0.075, 0.075, GA_LEFT);
	temp.transform.Translate(vec3(0.48, 0.9, -0.2), false);
	temp.particleOverlayMat.color = vec4(1.0, 1.0, 1.0, 1.0);
	temp.particleOverlayMat.mainTexture = MAP_ZERO;
	Game::CreateOverlayObject(temp);

	temp = GameObject("Score2", TAGS_HUD);
	temp.mesh = GeoGenerator::MakeRect(0.075, 0.075, GA_LEFT);
	temp.transform.Translate(vec3(0.50, 0.9, -0.3), false);
	temp.particleOverlayMat.color = vec4(1.0, 1.0, 1.0, 1.0);
	temp.particleOverlayMat.mainTexture = MAP_ZERO;
	Game::CreateOverlayObject(temp);

	temp = GameObject("Score3", TAGS_HUD);
	temp.mesh = GeoGenerator::MakeRect(0.075, 0.075, GA_LEFT);
	temp.transform.Translate(vec3(0.52, 0.9, -0.4), false);
	temp.particleOverlayMat.color = vec4(1.0, 1.0, 1.0, 1.0);
	temp.particleOverlayMat.mainTexture = MAP_ZERO;
	Game::CreateOverlayObject(temp);

	temp = GameObject("Score4", TAGS_HUD);
	temp.mesh = GeoGenerator::MakeRect(0.075, 0.075, GA_LEFT);
	temp.transform.Translate(vec3(0.54, 0.9, -0.5), false);
	temp.particleOverlayMat.color = vec4(1.0, 1.0, 1.0, 1.0);
	temp.particleOverlayMat.mainTexture = MAP_ZERO;
	Game::CreateOverlayObject(temp);

	//IF_DEF HIDE HUD
	vector<GameObject*> hudItems = FindGameObjectsWithTag(TAGS_HUD);
	for (int i = 0; i < hudItems.size(); i++)
		hudItems[i]->isVisible = false;
	//END_IF HIDE HUD

//OVERLAY GAMEOBJECTS - PAUSE
	temp = GameObject("PauseMenuBack", TAGS_PAUSE);
	temp.mesh = GeoGenerator::MakeRect(1.0, 0.40, GA_CENTER);
	temp.transform.Translate(vec3(0.0, 0.0, 0.0), false);
	temp.particleOverlayMat.color = vec4(0, 0, 0, 0.25);
	//Game::CreateOverlayObject(temp);

	temp = GameObject("PauseMenuButton", TAGS_PAUSE);
	temp.mesh = GeoGenerator::MakeRect(0.95, 0.35, GA_CENTER);
	temp.transform.Translate(vec3(0.0, 0.0, -0.1), false);
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
	temp.mesh = GeoGenerator::MakeRect(1.0, 0.40, GA_CENTER);
	temp.transform.Translate(vec3(0.0, 0.7, -0.5), false);
	temp.particleOverlayMat.color = vec4(0, 0, 0, 0.5);	
	//Game::CreateOverlayObject(temp);

	temp = GameObject("GameOver", TAGS_GAME_OVER);
	temp.mesh = GeoGenerator::MakeRect(0.95, 0.45, GA_CENTER);
	temp.transform.Translate(vec3(0.0, 0.7, -0.6), false);
	temp.particleOverlayMat.mainTexture = MAP_GAME_OVER;
	temp.particleOverlayMat.color = vec4(1, 0, 0, 0.5);
	Game::CreateOverlayObject(temp);

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
	cout << object.objectID << endl;
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
GameObject* Game::CreateAIObject(GameObject object)
{
	object.objectID = aiObjectList.size();
	object.Start();
	aiObjectList.push_back(object);
	return &aiObjectList[aiObjectList.size() - 1];
}

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
	GameObject object = overlayObjectList[objectID];
	overlayObjectList.erase(overlayObjectList.begin() + object.objectID);
	for (int i = 0; i < overlayObjectList.size(); i++)
	{
		if (overlayObjectList[i].objectID > object.objectID)
			overlayObjectList[i].objectID--;
	}
	object.RemoveComponents();
}
void Game::DestroyAIObjectAt(int objectID)
{
	GameObject object = aiObjectList[objectID];
	aiObjectList.erase(aiObjectList.begin() + object.objectID);
	for (int i = 0; i < aiObjectList.size(); i++)
	{
		if (aiObjectList[i].objectID > object.objectID)
			aiObjectList[i].objectID--;
	}
	object.RemoveComponents();
}

//THE FIND ENGINE
GameObject* Game::Find(string name)
{
	for (int i = 0; i < worldObjectList.size(); i++)
	{
		if (name == worldObjectList[i].name)
			return &worldObjectList[i];
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
	for (int i = 0; i < aiObjectList.size(); i++)
	{
		if (name == aiObjectList[i].name)
			return &aiObjectList[i];
	}

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