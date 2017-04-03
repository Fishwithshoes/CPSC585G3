#include "PowerUpComponent.h"
#include "Game.h"
#include "Physics.h"

PowerUpComponent::PowerUpComponent(GameWeapons typeIn)
{
	type = typeIn;
}

void PowerUpComponent::Start()
{
	Initialize();

	validCollide = true;
	deactivationTime = 0.0;

	physx::PxPhysics* worldPhys = Physics::getGPhysics();
	physx::PxCooking* worldCook = Physics::getGCooking();
	physx::PxScene* worldScene = Physics::getGScene();
	physx::PxVec3 startPosition = physx::PxVec3(transform.position.x, transform.position.y, transform.position.z);

	statComp = Physics::createPowerUp(2.0);
	statComp->setGlobalPose(physx::PxTransform(startPosition, physx::PxQuat(physx::PxIdentity))); //set global position based on vec created in Game
	statComp->userData = this;

	transform.position.x = statComp->getGlobalPose().p.x;
	transform.position.y = statComp->getGlobalPose().p.y;
	transform.position.z = statComp->getGlobalPose().p.z;

	standardMat.selfIllumLevel = 0.5;
	standardMat.diffuseColor = vec3(1.0);

	switch (type)
	{
	case GW_MACHINE_GUN:
		standardMat.diffuseMap = MAP_MACHINE_GUN_ICON;
		break;
	case GW_MISSILE_LAUNCHER:
		standardMat.diffuseMap = MAP_MISSILE_LAUNCHER_ICON;
		break;
	case GW_FLAMETHROWER:
		standardMat.diffuseMap = MAP_FLAMETHROWER_ICON;
		break;
	default:
		cout << "ERROR: Can't setup illegal type at PowerUpComponent Start()!" << endl;
		break;
	}

	Finalize();
}

void PowerUpComponent::Update()
{
	Initialize();

	if (!validCollide) 
	{
		transform.scale = vec3(0.0);
		deactivationTime += Time::getDeltaTime();
		if (deactivationTime >= 3.0) 
		{
			validCollide = true;
			deactivationTime = 0.0;
		}
	}
	else 
	{
		if (transform.scale.x < 1.0)
		{
			transform.scale += vec3(2.0) * Time::timeScale * Time::getDeltaTime();
		}
		else if (transform.scale.x > 1.0)
		{
			transform.scale = vec3(1);
		}
	}

	transform.position.x = statComp->getGlobalPose().p.x;
	transform.position.y = statComp->getGlobalPose().p.y;
	transform.position.z = statComp->getGlobalPose().p.z;

	physx::PxQuat rotQuat = statComp->getGlobalPose().q;

	//transform.rotation.x = rotQuat.x;
	//transform.rotation.y = rotQuat.y;
	//transform.rotation.z = rotQuat.z;
	//transform.rotation.w = rotQuat.w;

	Finalize();
}

void PowerUpComponent::OnCollision(Component::CollisionPair collisionPair, Component* collider) 
{
	cout << "PU Collision" << endl;
	validCollide = false;
	//Note: Ammo collection now handled in PlayerComponent OnCollision(args...)
}

bool PowerUpComponent::CheckCollide() 
{
	return validCollide;
}
