#include "EnemyComponent.h"
#include "Game.h"
#include "Physics.h"

void EnemyComponent::Start()
{
	Initialize();
	transform.rendertype = RenderTypes::RT_QUAT;
	physx::PxPhysics* worldPhys = Physics::getGPhysics();
	physx::PxCooking* worldCook = Physics::getGCooking();
	physx::PxScene* worldScene = Physics::getGScene();
	physx::PxReal offset = 5.0;



	Physics::VehicleDesc vehicleDesc = Physics::initVehicleDesc();
	enVehicleNoDrive = Physics::createVehicleNoDrive(vehicleDesc, worldPhys, worldCook);
	physx::PxTransform startTransform(physx::PxVec3(0 + offset, (vehicleDesc.chassisDims.y*0.5f + vehicleDesc.wheelRadius + 1.0f), 0 + offset), physx::PxQuat(physx::PxIdentity));
	Physics::setEnVehicleNoDrive(enVehicleNoDrive);
	enVehicleNoDrive->getRigidDynamicActor()->setGlobalPose(startTransform);
	worldScene->addActor(*enVehicleNoDrive->getRigidDynamicActor());

	enPhysVehicle = enVehicleNoDrive->getRigidDynamicActor();

	transform.position.x = enPhysVehicle->getGlobalPose().p.x;
	transform.position.y = enPhysVehicle->getGlobalPose().p.y;
	transform.position.z = enPhysVehicle->getGlobalPose().p.z;

	Finalize();
}

void EnemyComponent::Update()
{
	Initialize();

	transform.position.x = enPhysVehicle->getGlobalPose().p.x;
	transform.position.y = enPhysVehicle->getGlobalPose().p.y;
	transform.position.z = enPhysVehicle->getGlobalPose().p.z;

	//NOTE INVERSIONS due to coordinate system

	physx::PxQuat pq = enPhysVehicle->getGlobalPose().q;
	mat4 newRot = glm::mat4_cast(glm::quat(pq.w, pq.x, pq.y, pq.z));
	newRot = glm::inverse(newRot);

	transform.rotationMatrix = newRot;

	Finalize();
}
