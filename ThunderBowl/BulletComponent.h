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

	float speed;
	float lifeSpan;

private:
};

