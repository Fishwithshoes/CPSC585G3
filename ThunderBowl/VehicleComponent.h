#pragma once
#include "Component.h"
#include <PxPhysicsAPI.h>

class VehicleComponent :public Component
{
public:
	void Start();
	void Update();
	void OnCollision(Component::CollisionPair collisionPair);

	float topSpeed = 200;
	float acceleration = 1.0;

	float topTurn = 1.0;
	float turnAccel = 0.01;

	int MGAmmo = 0;
	//other ammo counts here

	physx::PxRigidDynamic* physVehicle;
	physx::PxVehicleNoDrive* gVehicleNoDrive;

protected:

private:
	float currentSpeed = 0;
	float currentTurn = 0;
};

