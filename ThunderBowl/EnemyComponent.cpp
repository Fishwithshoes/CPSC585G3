#include "EnemyComponent.h"
#include "BulletComponent.h"
#include "GeoGenerator.h"
#include "Game.h"
#include "Physics.h"
#include "Audio.h"


void EnemyComponent::Start()
{
	Initialize();


	physx::PxPhysics* worldPhys = Physics::getGPhysics();
	physx::PxCooking* worldCook = Physics::getGCooking();
	physx::PxScene* worldScene = Physics::getGScene();
	startPosition = physx::PxVec3(transform.position.x, transform.position.y, transform.position.z);
	startRotation = physx::PxQuat(transform.rotation.x, transform.rotation.y, transform.rotation.z, transform.rotation.w);

	Physics::VehicleDesc vehicleDesc = Physics::initVehicleDesc();
	enVehicleNoDrive = Physics::createVehicleNoDrive(vehicleDesc, worldPhys, worldCook);
	physx::PxTransform startTransform(physx::PxVec3(0, (vehicleDesc.chassisDims.y*0.5f + vehicleDesc.wheelRadius + 1.0f), 0), physx::PxQuat(physx::PxIdentity));
	//Physics::setEnVehicleNoDrive(enVehicleNoDrive);
	Physics::addEnVehicleNoDrive(enVehicleNoDrive);
	enVehicleNoDrive->getRigidDynamicActor()->setGlobalPose(startTransform);
	worldScene->addActor(*enVehicleNoDrive->getRigidDynamicActor());

	enPhysVehicle = enVehicleNoDrive->getRigidDynamicActor();
	enPhysVehicle->setGlobalPose(physx::PxTransform(startPosition, startRotation)); //set global position based on vec created in Game
	enPhysVehicle->userData = this;

	physx::PxU32 wheelBufferSize = enVehicleNoDrive->mWheelsSimData.getNbWheels() * sizeof(physx::PxShape*);
	enWheelBuffer = new physx::PxShape*[wheelBufferSize];
	enPhysVehicle->getShapes(enWheelBuffer, wheelBufferSize);

	for (int i = 0; i < 4; i++) {
		GameObject temp = GameObject();
		physx::PxTransform currWheel = enWheelBuffer[i]->getLocalPose();
		temp.mesh = GeoGenerator::MakeCylinder(0.5, 0.5, 0.4, 8, false); //change to take in physx values
		enWheelVector.push_back(Game::CreateWorldObject(temp));
	}

	Finalize();
}

void EnemyComponent::Update()
{
	Initialize();

	for (int i = 0; i < 4; i++) {
		physx::PxTransform currWheel = enWheelBuffer[i]->getLocalPose();

		//Rotate local translation offset vectors
		physx::PxVec3 adjustedPosition = enPhysVehicle->getGlobalPose().q.rotate(enWheelBuffer[i]->getLocalPose().p);

		//Combine chassis and wheel rotation
		physx::PxQuat totalWheelRotation = enPhysVehicle->getGlobalPose().q * enWheelBuffer[i]->getLocalPose().q;

		//Apply rotation to transform.rotation
		enWheelVector[i]->transform.rotation.x = totalWheelRotation.x;
		enWheelVector[i]->transform.rotation.y = totalWheelRotation.y;
		enWheelVector[i]->transform.rotation.z = totalWheelRotation.z;
		enWheelVector[i]->transform.rotation.w = totalWheelRotation.w;

		//Apply position to transform.position
		enWheelVector[i]->transform.position.x = enPhysVehicle->getGlobalPose().p.x + adjustedPosition.x;
		enWheelVector[i]->transform.position.y = enPhysVehicle->getGlobalPose().p.y + adjustedPosition.y;
		enWheelVector[i]->transform.position.z = enPhysVehicle->getGlobalPose().p.z + adjustedPosition.z;
		//totalWheelRotation.w;

	}

	transform.position.x = enPhysVehicle->getGlobalPose().p.x;
	transform.position.y = enPhysVehicle->getGlobalPose().p.y;
	transform.position.z = enPhysVehicle->getGlobalPose().p.z;

	enCurrentPosition = transform.position;

	//NOTE INVERSIONS due to coordinate system

	physx::PxQuat rotQuat = enPhysVehicle->getGlobalPose().q;

	transform.rotation.x = rotQuat.x;
	transform.rotation.y = rotQuat.y;
	transform.rotation.z = rotQuat.z;
	transform.rotation.w = rotQuat.w;

	if (transform.position.y < -20)
	{
		enPhysVehicle->setGlobalPose(physx::PxTransform(startPosition, startRotation));
		enPhysVehicle->setAngularVelocity(physx::PxVec3(0, 0, 0));
		enPhysVehicle->setLinearVelocity(physx::PxVec3(0, 0, 0));
	}

	MoveOnHeading();

	Finalize();
}

void EnemyComponent::OnCollision(Component::CollisionPair collisionPair, Component* collider) {
	Initialize();

	MachineGunComponent* mgRef = &MachineGunComponent();
	PlayerComponent* oppRef = &PlayerComponent();
	EnemyComponent* enemyRef = &EnemyComponent();

	HealthComponent* playerHealth = &HealthComponent();
	//BulletComponent* bulletRef = &BulletComponent();

	opponentComp = (EnemyComponent*)Game::Find(selfName)->GetComponent(enemyRef);

	switch (collisionPair) 
	{
	case(Component::CollisionPair::CP_VEHICLE_POWERUP):
		Audio::Play2DSound(SFX_Powerup, Random::rangef(0.20, 0.50), 0.0);
		oppVehicleMG = (MachineGunComponent*)Game::Find(selfName)->GetComponent(mgRef);
		oppVehicleMG->ammoCount += 100;
		oppPlayer = (PlayerComponent*)Game::Find(selfName)->GetComponent(oppRef);
		oppPlayer->playerScore += 10.0;
		break;
	case(Component::CollisionPair::CP_VEHICLE_PROJECTILE):
		Audio::Play2DSound(SFX_Hit, Random::rangef(0.20, 0.50), 0.0);
		playerHealth = (HealthComponent*)Game::Find(selfName)->GetComponent(playerHealth);
		playerHealth->currentHealth -= 10.0;
		break;
	}

	Finalize();
}

void EnemyComponent::MoveOnHeading() {
	physx::PxReal maxTorque = 1000.0;
	physx::PxReal brakeTorque = 1000.0;
	physx::PxReal turnTemper = 0.25;
	AIControlComponent1* tempController = &AIControlComponent1();
	aiController = (AIControlComponent1*)Game::Find(selfName)->GetComponent(tempController);
	vec3 requiredHeading = glm::normalize(aiController->currentHeading);
	vec3 actualHeading = glm::normalize(transform.GetForward());
	actualHeading.x = -actualHeading.x;
	//cout << "R: " << requiredHeading.x << " " << requiredHeading.y << " " << requiredHeading.z << endl;
	//cout << "A: " << actualHeading.x << " " << actualHeading.y << " " << actualHeading.z << endl;
	//cout << "D: " << glm::dot(actualHeading, requiredHeading) << endl;
	enVehicleNoDrive->setSteerAngle(2, clamp(1.0 - glm::dot(actualHeading, requiredHeading), 0.0, 1.0)*turnTemper);
	enVehicleNoDrive->setSteerAngle(3, clamp(1.0 - glm::dot(actualHeading, requiredHeading), 0.0, 1.0)*turnTemper);

	enVehicleNoDrive->setDriveTorque(0, maxTorque);
	enVehicleNoDrive->setDriveTorque(1, maxTorque);
	//standardMat.diffuseColor = vec3(0.0, glm::dot(actualHeading, requiredHeading), 0.0);
}