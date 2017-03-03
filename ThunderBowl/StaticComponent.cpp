#include "StaticComponent.h"
#include "Game.h"
#include "Physics.h"

void StaticComponent::Start() 
{
	Initialize();

	physx::PxPhysics* worldPhys = Physics::getGPhysics();
	physx::PxCooking* worldCook = Physics::getGCooking();
	physx::PxScene* worldScene = Physics::getGScene();
	physx::PxVec3 startPosition = physx::PxVec3(transform.position.x, transform.position.y, transform.position.z);

	statComp = Physics::createStaticRecPrism(8.0, 20.0, 8.0);
	statComp->userData = this;

	statComp->setGlobalPose(physx::PxTransform(startPosition, physx::PxQuat(physx::PxIdentity)));
	transform.position.x = statComp->getGlobalPose().p.x;
	transform.position.y = statComp->getGlobalPose().p.y;
	transform.position.z = statComp->getGlobalPose().p.z;

	Finalize();
}

void StaticComponent::Update()
{
	Initialize();


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
