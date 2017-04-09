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
	void UTurn(vec3 inHeading);
	void ReverseOut();

	void maintainUpright();

	bool performUTurn;

	physx::PxReal maxTorque;
	physx::PxReal brakeTorque;
	physx::PxReal turnTemper;

	vec3 enCurrentPosition;
	vec3 enActualHeading;
	vec3 uTurnHeading;
};

