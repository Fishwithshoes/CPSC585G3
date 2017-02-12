#pragma once
#include "Component.h"
#include <PxPhysicsAPI.h>

class EnemyComponent :
	public Component
{
public:
	physx::PxVehicleNoDrive* enVehicleNoDrive;
	physx::PxRigidDynamic* enPhysVehicle;
	void Start();
	void Update();
};

