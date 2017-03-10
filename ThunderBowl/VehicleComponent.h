#pragma once
#include "Component.h"
#include <PxPhysicsAPI.h>

class VehicleComponent :public Component
{
public:
	void Start();
	void Update();
	void OnCollision(Component::CollisionPair collisionPair);
	void UpdateParticleSystems();

	float currentSpeed;
	float topSpeed = 125.0;
	float acceleration = 1.0;

	float topTurn = 1.0;
	float turnAccel = 0.01;

	physx::PxRigidDynamic* physVehicle;
	physx::PxVehicleNoDrive* gVehicleNoDrive;

protected:

private:
	float currentTurn = 0;
	string wheelSprayNameLeft;
	string wheelSprayNameRight;
};

