#pragma once
#include "Component.h"
#include "GameObject.h"
#include "MachineGunComponent.h"
#include "PlayerComponent.h"
#include "Camera.h"
#include <PxPhysicsAPI.h>

class VehicleComponent :public Component
{
public:
	void Start();
	void Update();

	void OnCollision(Component::CollisionPair collisionPair, Component* collider);
	void UpdateParticles();

	float currentSpeed;
	float topSpeed = 125.0;
	float topReverse = 50.0;
	float acceleration = 1.0;

	float topTurn = 1.0;
	float turnAccel = 0.01;

	physx::PxRigidDynamic* physVehicle;
	physx::PxVehicleNoDrive* gVehicleNoDrive;
	Camera* followCam;
	physx::PxShape** wheelBuffer;
	vector<GameObject*> wheelVector;
	MachineGunComponent* vehicleMG;
	PlayerComponent* vehPlayer;

	physx::PxVec3 myStartPosition;

	int GetPlayerNum();
	void SetPlayerNum(int playerNumber);

protected:

private:
	int playerNum = 1;
	float currentTurn = 0;
	string wheelSprayNameLeft;
	string wheelSprayNameRight;
	bool enteredWaterPrev = false;
	float engineDelay = 0.0;
	float nextEngine = 0.0;
	float currentYaw = 0.0;
	bool hornReady = true;
};

