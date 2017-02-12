#include "VehicleComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "Physics.h"

Camera* followCam;

void VehicleComponent::Start()
{
	Initialize();
	cout << "I am a Vehicle Component!" << endl;
	transform.rendertype = RenderTypes::RT_QUAT;
	physx::PxPhysics* worldPhys = Physics::getGPhysics();
	physx::PxCooking* worldCook = Physics::getGCooking();
	physx::PxScene* worldScene = Physics::getGScene();

	

	Physics::VehicleDesc vehicleDesc = Physics::initVehicleDesc();
	gVehicleNoDrive = Physics::createVehicleNoDrive(vehicleDesc, worldPhys, worldCook);
	physx::PxTransform startTransform(physx::PxVec3(0, (vehicleDesc.chassisDims.y*0.5f + vehicleDesc.wheelRadius + 1.0f), 0), physx::PxQuat(physx::PxIdentity));
	Physics::setGVehicleNoDrive(gVehicleNoDrive);
	gVehicleNoDrive->getRigidDynamicActor()->setGlobalPose(startTransform);
	worldScene->addActor(*gVehicleNoDrive->getRigidDynamicActor());

	physVehicle = gVehicleNoDrive->getRigidDynamicActor();

	followCam = Renderer::GetCamera(0);
	followCam->transform.rendertype = RenderTypes::RT_QUAT;
	transform.position.x = physVehicle->getGlobalPose().p.x;
	transform.position.y = physVehicle->getGlobalPose().p.y;
	transform.position.z = physVehicle->getGlobalPose().p.z;
	Finalize();
}

void VehicleComponent::Update()
{
	Initialize();	

<<<<<<< HEAD
	if (Input::GetXBoxAxis(1,ButtonCode::XBOX_RIGHT_TRIGGER))
	{
		//physx::PxRigidBodyExt::addLocalForceAtLocalPos(*physVehicle, physx::PxVec3(0.0,0.0,50.0), physx::PxVec3(0.0,0.0,0.0));
		gVehicleNoDrive->setDriveTorque(0, 100.0f);
	}
	else if(Input::GetXBoxAxis(1, ButtonCode::XBOX_LEFT_TRIGGER))
	{
		//physx::PxRigidBodyExt::addLocalForceAtLocalPos(*physVehicle, physx::PxVec3(0.0, 0.0, -50.0), physx::PxVec3(0.0, 0.0, 0.0));
	}
=======
	//if (Input::GetXBoxAxis(1, ButtonCode::XBOX_RIGHT_TRIGGER))
	//{
	//	//physx::PxRigidBodyExt::addLocalForceAtLocalPos(*physVehicle, physx::PxVec3(0.0,0.0,50.0), physx::PxVec3(0.0,0.0,0.0));
	//	gVehicleNoDrive->setDriveTorque(0, 1000.0f);
	//}
	//else if(Input::GetXBoxAxis(1, ButtonCode::XBOX_LEFT_TRIGGER))
	//{
	//	//physx::PxRigidBodyExt::addLocalForceAtLocalPos(*physVehicle, physx::PxVec3(0.0, 0.0, -50.0), physx::PxVec3(0.0, 0.0, 0.0));
	//}
>>>>>>> refs/remotes/origin/master

		//physx::PxRigidBodyExt::addLocalForceAtLocalPos(*physVehicle, physx::PxVec3(5.0*(Input::GetXBoxAxis(1, ButtonCode::XBOX_JOY_LEFT_HORIZONTAL)), 0.0, 0.0), physx::PxVec3(0.0, 0.0, 100.0));


	if (Input::GetButton(ButtonCode::MIDDLE_MOUSE))
		Time::timeScale += Input::GetMouseDelta().x * Time::getDeltaTime();	

	transform.position.x = physVehicle->getGlobalPose().p.x;
	transform.position.y = physVehicle->getGlobalPose().p.y;
	transform.position.z = physVehicle->getGlobalPose().p.z;

	//NOTE INVERSIONS due to coordinate system

	physx::PxQuat pq = physVehicle->getGlobalPose().q;
	mat4 newRot = glm::mat4_cast(glm::quat(pq.w, pq.x, pq.y, pq.z));
	newRot = glm::inverse(newRot);

	followCam->transform.rotationMatrix = glm::inverse(newRot);

	glm::vec4 cameraOffset = glm::vec4(0.0, 5.0, -15.0, 0.0);
	cameraOffset = glm::inverse(newRot) * cameraOffset;
	cameraOffset = glm::vec4(transform.position, 0.0) + cameraOffset;
	followCam->transform.position = cameraOffset;

	transform.rotationMatrix = newRot;
	
	Finalize();
}