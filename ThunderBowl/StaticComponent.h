#pragma once
#include "Component.h"
#include <PxPhysicsAPI.h>

class StaticComponent :
	public Component
{
public:
	physx::PxRigidStatic* statComp;
	bool validCollide;
	double deactivationTime;

	void Start();
	void Update();
	void OnCollision(Component::CollisionPair collisionPair);
};

