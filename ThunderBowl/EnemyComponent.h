#pragma once
#include "Component.h"
#include "PlayerComponent.h"
#include "BulletComponent.h"
#include "MachineGunComponent.h"
#include "AIControlComponent1.h"
#include "GameObject.h"
#include <PxPhysicsAPI.h>

class EnemyComponent :
	public Component
{
public:
	physx::PxVehicleNoDrive* enVehicleNoDrive;
	physx::PxRigidDynamic* enPhysVehicle;
	physx::PxShape** enWheelBuffer;
	vector<GameObject*> enWheelVector;


	PlayerComponent* oppPlayer;
	BulletComponent* damagingBullet;

	MachineGunComponent* oppVehicleMG;
	EnemyComponent* opponentComp;
	AIControlComponent1* aiController;

	physx::PxVec3 startPosition;
	physx::PxQuat startRotation;

	void Start();
	void Update();
	void OnCollision(CollisionPair collisionPair, Component* collider);
	void MoveOnHeading();
	vec3 enCurrentPosition;
	vec3 enActualHeading;
};

