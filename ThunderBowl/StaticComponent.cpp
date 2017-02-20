#include "StaticComponent.h"
#include "Game.h"
#include "Physics.h"

void StaticComponent::Start() 
{
	Initialize();

	transform.rendertype = RenderTypes::RT_QUAT;
	physx::PxPhysics* worldPhys = Physics::getGPhysics();
	physx::PxCooking* worldCook = Physics::getGCooking();
	physx::PxScene* worldScene = Physics::getGScene();

	statComp = Physics::createTestBox(2.0);

	physx::PxTransform offset = physx::PxTransform(1.0, 5.0, 5.0);
	statComp->setGlobalPose(offset);
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

	physx::PxQuat pq = statComp->getGlobalPose().q;

	mat4 newRot = glm::mat4_cast(glm::quat(pq.w, pq.x, pq.y, pq.z));
	newRot = glm::inverse(newRot);

	transform.rotationMatrix = newRot;

	Finalize();
}
