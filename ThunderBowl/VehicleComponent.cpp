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
	physVehicle = Physics::createTestBox(2.0);
	physVehicle->setMass(3.0);
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

	if (Input::GetXBoxAxis(1,ButtonCode::XBOX_RIGHT_TRIGGER))
	{
		physx::PxRigidBodyExt::addLocalForceAtLocalPos(*physVehicle, physx::PxVec3(0.0,0.0,50.0), physx::PxVec3(0.0,0.0,0.0));
	}
	else if(Input::GetXBoxAxis(1, ButtonCode::XBOX_LEFT_TRIGGER))
	{
		physx::PxRigidBodyExt::addLocalForceAtLocalPos(*physVehicle, physx::PxVec3(0.0, 0.0, -50.0), physx::PxVec3(0.0, 0.0, 0.0));
	}

		physx::PxRigidBodyExt::addLocalForceAtLocalPos(*physVehicle, physx::PxVec3(5.0*(Input::GetXBoxAxis(1, ButtonCode::XBOX_JOY_LEFT_HORIZONTAL)), 0.0, 0.0), physx::PxVec3(0.0, 0.0, 100.0));


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

	glm::vec4 cameraOffset = glm::vec4(0.0, 5.0, -10.0, 0.0);
	cameraOffset = glm::inverse(newRot) * cameraOffset;
	cameraOffset = glm::vec4(transform.position, 0.0) + cameraOffset;
	followCam->transform.position = cameraOffset;


	transform.rotationMatrix = newRot;
	
	Finalize();
}