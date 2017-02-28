#pragma once
#include "Component.h"
#include <PxPhysicsAPI.h>

class BulletComponent : public Component
{
public:
	physx::PxRigidDynamic* bullet;

	void Start();
	void Update();
	void OnCollision(Component::CollisionPair collisionPair);

	float speed = 200.0f;
	float lifeSpan = 5.0f;

private:
	//GameObject* self;
	float lifeRemaining = lifeSpan;
};

