#pragma once
#include "Component.h"
#include <PxPhysicsAPI.h>

class BulletComponent : public Component
{
public:
	physx::PxRigidDynamic* bullet;

	void Start();
	void Update();

	float speed = 300.0f;
	float lifeSpan = 3.0f;

private:
	//GameObject* self;
	float lifeRemaining = lifeSpan;
};

