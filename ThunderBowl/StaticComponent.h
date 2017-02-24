#pragma once
#include "Component.h"
#include <PxPhysicsAPI.h>

class StaticComponent :
	public Component
{
public:
	physx::PxRigidDynamic* statComp;
	void Start();
	void Update();
	void OnCollision(Component::CollisionPair collisionPair);
};

