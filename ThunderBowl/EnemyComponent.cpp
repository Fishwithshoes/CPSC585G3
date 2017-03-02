#include "EnemyComponent.h"
#include "PlayerComponent.h"
#include "Game.h"
#include "Physics.h"

PlayerComponent* oppPlayer;

void EnemyComponent::Start()
{
	Initialize();


	standardMat.diffuseColor = vec3(0.0, 0.0, 1.0);
	physx::PxPhysics* worldPhys = Physics::getGPhysics();
	physx::PxCooking* worldCook = Physics::getGCooking();
	physx::PxScene* worldScene = Physics::getGScene();
	physx::PxVec3 startPosition = physx::PxVec3(transform.position.x, transform.position.y, transform.position.z);

	Physics::VehicleDesc vehicleDesc = Physics::initVehicleDesc();
	enVehicleNoDrive = Physics::createVehicleNoDrive(vehicleDesc, worldPhys, worldCook);
	physx::PxTransform startTransform(physx::PxVec3(0, (vehicleDesc.chassisDims.y*0.5f + vehicleDesc.wheelRadius + 1.0f), 0), physx::PxQuat(physx::PxIdentity));
	Physics::setEnVehicleNoDrive(enVehicleNoDrive);
	enVehicleNoDrive->getRigidDynamicActor()->setGlobalPose(startTransform);
	worldScene->addActor(*enVehicleNoDrive->getRigidDynamicActor());

	enPhysVehicle = enVehicleNoDrive->getRigidDynamicActor();
	enPhysVehicle->setGlobalPose(physx::PxTransform(startPosition, physx::PxQuat(physx::PxIdentity))); //set global position based on vec created in Game
	enPhysVehicle->userData = this;

	Finalize();
}

void EnemyComponent::Update()
{
	Initialize();

	transform.position.x = enPhysVehicle->getGlobalPose().p.x;
	transform.position.y = enPhysVehicle->getGlobalPose().p.y;
	transform.position.z = enPhysVehicle->getGlobalPose().p.z;

	//NOTE INVERSIONS due to coordinate system

	physx::PxQuat rotQuat = enPhysVehicle->getGlobalPose().q;

	transform.rotation.x = rotQuat.x;
	transform.rotation.y = rotQuat.y;
	transform.rotation.z = rotQuat.z;
	transform.rotation.w = rotQuat.w;

	Finalize();
}

void EnemyComponent::OnCollision(Component::CollisionPair collisionPair) {
	Initialize();

	//MachineGunComponent* mgRef = &MachineGunComponent();
	PlayerComponent* oppRef = &PlayerComponent();
	switch (collisionPair) {
	case(Component::CollisionPair::CP_VEHICLE_POWERUP):
		//vehicleMG = (MachineGunComponent*)Game::Find(selfName)->GetComponent(mgRef);
		//vehicleMG->ammoCount += 100;
		oppPlayer = (PlayerComponent*)Game::Find(selfName)->GetComponent(oppRef);
		oppPlayer->playerScore += 10.0;
		break;
	case(Component::CollisionPair::CP_VEHICLE_PROJECTILE):
		oppPlayer = (PlayerComponent*)Game::Find(selfName)->GetComponent(oppRef);
		oppPlayer->playerHealth -= 25.0;
		break;
	}

	Finalize();
}