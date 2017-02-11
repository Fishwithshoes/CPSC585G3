#pragma once
#include "Component.h"
#include <PxPhysicsAPI.h>

class VehicleComponent :public Component
{
public:
	void Start();
	void Update();

	float topSpeed = 200;
	float acceleration = 1.0;

	float topTurn = 1.0;
	float turnAccel = 0.01;

	physx::PxRigidDynamic* physVehicle;
	physx::PxVehicleNoDrive* gVehicleNoDrive;

protected:

private:
	float currentSpeed = 0;
	float currentTurn = 0;
};

