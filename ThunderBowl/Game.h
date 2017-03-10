#pragma once
#include "CommonIncludes.h"
#include "GameObject.h"
#include "ParticleSystem.h"

#include "FloatComponent.h"
#include "RotateComponent.h"
#include "VehicleComponent.h"
#include "EnemyComponent.h"
#include "MachineGunComponent.h"
#include "StaticComponent.h"
#include "PowerUpComponent.h"
#include "PlayerComponent.h"
#include "AINodeComponent.h"
#include "AIControlComponent1.h"
#include "HealthComponent.h"

#include "OceanComponent.h"

class Game
{
public:
	static GameObject skybox;//The environment skybox
	static vector<GameObject> staticObjectList;//For static mesh 3D objects that appear in the world
	static vector<GameObject> worldObjectList;//For dynamic mesh 3D objects that appear in the world
	static vector<ParticleSystem> particleObjectList;//For particle systems which spawn particles
	static vector<GameObject> overlayObjectList;//For menu and HUD elements to draw atop all else
	static vector<GameObject> aiObjectList; //For quick access to all AI nodes
	static vector<glm::vec3> plVehStartPositions; //For assigning player vehicle start positions
	static vector<glm::vec3> aiVehStartPositions; //For assigning AI vehicle start positions


	static void BuildWorld();//Reserves space in vectors and adds initial objects
	static void DestroyWorld();//Empties vectors, destroying all objects inside
	static void KillAllParticles();//Immediately destroys all ParticleSystem objects
	
	static GameObject*		CreateStaticObject		(GameObject object);
	static GameObject*		CreateWorldObject		(GameObject object);
	static ParticleSystem*	CreateParticleObject	(ParticleSystem object);
	static GameObject*		CreateOverlayObject		(GameObject object);
	static GameObject*		CreateAIObject			(GameObject object);

	static void DestroyStaticObjectAt	(int ObjectID);
	static void DestroyWorldObjectAt	(int ObjectID);
	static void DestroyParticleObjectAt	(int ObjectID);
	static void DestroyOverlayObjectAt	(int ObjectID);
	static void DestroyAIObjectAt		(int ObjectID);

	static GameObject* Find(string name);//Returns first one matching name
	static vector<GameObject*> FindGameObjectsWithTag(Tags tag);

protected:
	
private:
};
