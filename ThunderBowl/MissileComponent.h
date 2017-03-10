#pragma once
#include "Component.h"
#include <PxPhysicsAPI.h>
#include "ParticleSystem.h"
class MissileComponent :
	public Component
{
public:
	physx::PxRigidDynamic* missile;

	MissileComponent(vec3 size, float ownerVelocity, string ownerName);
	void Start();
	void Update();
	void MoveTransform();
	void OnCollision(CollisionPair collisionPair);
	void Explode();

	float speed = 100.0;
	float initialLife = 1.0;
protected:

private:
	vec3 missileSize;
	float inheritedVelocity;
	string missileOwnerName;
	float lifeRemaining = initialLife;
	ParticleSystem* particleTrail;
	string particleTrailName;

};

