#pragma once
#include "Component.h"
#include "PlayerComponent.h"
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
	MachineGunComponent* oppVehicleMG;
	EnemyComponent* opponentComp;
	AIControlComponent1* aiController;

	physx::PxVec3 startPosition;
	physx::PxQuat startRotation;

	void Start();
	void Update();
	void OnCollision(Component::CollisionPair collisionPair);
	void MoveOnHeading();
	vec3 enCurrentPosition;
	vec3 enActualHeading;
};

