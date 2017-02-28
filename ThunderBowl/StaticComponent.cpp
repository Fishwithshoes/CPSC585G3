/*#include "StaticComponent.h"
#include "Game.h"
#include "Physics.h"

void StaticComponent::Start() 
{
	Initialize();

	validCollide = true;
	deactivationTime = 0.0;

	physx::PxPhysics* worldPhys = Physics::getGPhysics();
	physx::PxCooking* worldCook = Physics::getGCooking();
	physx::PxScene* worldScene = Physics::getGScene();

	statComp = Physics::createPowerUp(2.0);
	statComp->userData = this;

	physx::PxTransform offset = physx::PxTransform(1.0, 1.0, 5.0);
	statComp->setGlobalPose(offset);
	transform.position.x = statComp->getGlobalPose().p.x;
	transform.position.y = statComp->getGlobalPose().p.y;
	transform.position.z = statComp->getGlobalPose().p.z;

	Finalize();
}

void StaticComponent::Update()
{
	Initialize();

	if (!validCollide) {
		standardMat.diffuseColor = vec3(1.0, 0.0, 0.0);
		transform.scale = vec3(0.0);
		deactivationTime += Time::getDeltaTime();
		if (deactivationTime >= 1.0) {
			validCollide = true;
			deactivationTime = 0.0;
		}
	}
	else {
		standardMat.diffuseColor = vec3(0.0, 1.0, 0.0);
		transform.scale = vec3(1.0);
	}

	transform.position.x = statComp->getGlobalPose().p.x;
	transform.position.y = statComp->getGlobalPose().p.y;
	transform.position.z = statComp->getGlobalPose().p.z;

	physx::PxQuat rotQuat = statComp->getGlobalPose().q;

	transform.rotation.x = rotQuat.x;
	transform.rotation.y = rotQuat.y;
	transform.rotation.z = rotQuat.z;
	transform.rotation.w = rotQuat.w;

	Finalize();
}

void StaticComponent::OnCollision(Component::CollisionPair collisionPair) {
	cout << "Stat Collision" << endl;
	validCollide = false;
}

bool StaticComponent::CheckCollide() {
	return validCollide;
}*/
