#pragma once
#include "Component.h"
#include <PxPhysicsAPI.h>

class PowerUpComponent : public Component
{
public:
	PowerUpComponent(GameWeapons typeIn);

	//physx::PxRigidDynamic* statComp;
	physx::PxRigidStatic* statComp;
	bool validCollide;
	double deactivationTime;

	void Start();
	void Update();
	void OnCollision(Component::CollisionPair collisionPair, Component* collider);
	bool CheckCollide();

	GameWeapons type = GW_MACHINE_GUN;
};

