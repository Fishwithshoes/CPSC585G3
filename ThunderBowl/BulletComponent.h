#pragma once
#include "Component.h"
#include <PxPhysicsAPI.h>

class BulletComponent : public Component
{
public:
	BulletComponent(string owner);
	physx::PxRigidDynamic* bullet;

	void Start();
	void Update();
	void OnCollision(Component::CollisionPair collisionPair, Component* collider);

	float speed;
	float lifeSpan;

	string ownerName;

private:
};

