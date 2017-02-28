#include "Game.h"
#include "GeoGenerator.h"
#include "Loader.h"

GameObject Game::skybox = GameObject();
vector<GameObject> Game::staticObjectList = {};
vector<GameObject> Game::worldObjectList = {};
vector<ParticleSystem> Game::particleObjectList = {};
vector<GameObject> Game::overlayObjectList = {};

void Game::BuildWorld()
{
	//Pre-allocate space in vectors
	staticObjectList.reserve(100);
	worldObjectList.reserve(1000);
	particleObjectList.reserve(100);
	overlayObjectList.reserve(50);

	//Skybox
	skybox.mesh = GeoGenerator::MakeSphere(5000, 16, 32, true);
	skybox.particleOverlayMat.mainTexture = MAP_ENV;

	//Add initial World GameObjects
	GameObject temp;//Change props then create with this
	GameObject *ptr;//Assign the result of creation to this and then add components

	ParticleSystem tempP;//Change props then create particle systemes with this
	ParticleSystem *ptrP;//Add components just as with basic gameObjects with this

	/*temp = GameObject();
	temp.mesh = GeoGenerator::MakeCylinder(1, 1, 2, 32);
	temp.transform.Translate(vec3(3, 2, 0));
	temp.name = "Flumpty";
	ptr = Game::CreateWorldObject(temp);*/

	//ifndef Car Cacophony
	temp = GameObject();
	temp.mesh = GeoGenerator::MakeBox(2, 2, 2, false);
	temp.name = "Player1";
	ptr = Game::CreateWorldObject(temp);
	ptr->AddComponent(new VehicleComponent());
	ptr->AddComponent(new MachineGunComponent());

	GameObject temp2 = GameObject();
	temp2.mesh = GeoGenerator::MakeBox(2, 2, 2, false);
	ptr = Game::CreateWorldObject(temp2);
	ptr->AddComponent(new PowerUpComponent());

	GameObject temp3 = GameObject();
	temp3.mesh = GeoGenerator::MakeBox(2, 2, 2, false);
	ptr = Game::CreateWorldObject(temp3);
	ptr->AddComponent(new EnemyComponent());

	/*GameObject temp3 = GameObject();
	temp3.mesh = GeoGenerator::MakeBox(2, 2, 2, false);
	ptr = Game::CreateWorldObject(temp2);
	ptr->AddComponent(new EnemyComponent());*/
	//ptr->AddComponent(new HealthComponent());
	
	temp = GameObject();
	temp.mesh = GeoGenerator::MakePlane(100, 100, 4, 4);
	temp.standardMat.diffuseMap = MAP_CHASSIS_DIFFUSE;
	temp.standardMat.bumpLevel = 2;
	temp.standardMat.normalMap = MAP_CHASSIS_NORMAL;
	temp.standardMat.tileUV = vec2(12,12);
	ptr = Game::CreateWorldObject(temp);

	temp = GameObject();
	temp.mesh = GeoGenerator::MakeSphere(1, 32, 64, false);
	temp.name = "BigGoldAndShiny";
	temp.transform.scale = vec3(5);
	temp.transform.position = vec3(10,5,10);
	temp.standardMat.diffuseColor = vec3(1.0, 1.0, 1.0);
	temp.standardMat.roughness = 0.5;
	temp.standardMat.metalness = 1.00;
	temp.standardMat.isMetallic = true;
	ptr = Game::CreateWorldObject(temp);

	tempP = ParticleSystem();
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
	Game::CreateParticleObject(tempP);
	
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

	temp = GameObject("Ocean", TAGS_DECORATION);
	temp.staticGeo = SG_OCEAN;
	temp.transform.position.y = -2;
	temp.standardMat.diffuseColor = vec3(0.0, 1.0, 1.0)*0.5f;
	temp.standardMat.roughness = 0.0;
	temp.standardMat.metalness = 0.10;
	temp.standardMat.normalMap = MAP_WATER_NORMAL;
	temp.standardMat.bumpLevel = 0.2;
	temp.standardMat.tileUV = vec2(10, 10);
	ptr = Game::CreateStaticObject(temp);
	ptr->AddComponent(new OceanComponent());

	temp = GameObject("Puddle", TAGS_DECORATION);
	temp.staticGeo = SG_PUDDLE;
	temp.transform.position.y = 1;
	temp.standardMat.diffuseColor = vec3(0.3, 0.3, 0.5)*0.5f;
	temp.standardMat.roughness = 0.0;
	temp.standardMat.metalness = 0.20;
	temp.standardMat.normalMap = MAP_WATER_NORMAL;
	temp.standardMat.bumpLevel = 0.2;
	temp.standardMat.tileUV = vec2(2, 2);
	//ptr = Game::CreateStaticObject(temp);
	//ptr->AddComponent(new PuddleComponent());

	//Add initial Overlay GameObjects
	temp = GameObject();
	temp.mesh = GeoGenerator::MakeCircle(1, 64, 0.25);
	temp.transform.Translate(vec3(0, 0.75, 0));
	temp.particleOverlayMat.mainTexture = MAP_JERRY;
	Game::CreateOverlayObject(temp);
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

//DESTROYERS
void Game::DestroyStaticObjectAt(int objectID)
{
	GameObject object = staticObjectList[objectID];
	staticObjectList.erase(staticObjectList.begin() + object.objectID);
	for (int i = 0; i < staticObjectList.size(); i++)
	{
		if (staticObjectList[i].objectID > object.objectID)
			staticObjectList[i].objectID--;
	}
	object.RemoveComponents();
}
void Game::DestroyWorldObjectAt(int objectID)
{
	GameObject object = worldObjectList[objectID];
	worldObjectList.erase(worldObjectList.begin() + object.objectID);
	for (int i = 0; i < worldObjectList.size(); i++)
	{
		if (worldObjectList[i].objectID > object.objectID)
			worldObjectList[i].objectID--;
	}
	object.RemoveComponents();
}
void Game::DestroyParticleObjectAt(int objectID)
{
	ParticleSystem object = particleObjectList[objectID];
	particleObjectList.erase(particleObjectList.begin() + object.objectID);
	for (int i = 0; i < particleObjectList.size(); i++)
	{
		if (particleObjectList[i].objectID > object.objectID)
			particleObjectList[i].objectID--;
	}
	object.RemoveComponents();
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