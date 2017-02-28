#include "VehicleComponent.h"
#include "Renderer.h"
#include "Physics.h"
#include "GameObject.h"
#include "GeoGenerator.h"
#include "MachineGunComponent.h"
#include "Game.h"


Camera* followCam;
physx::PxShape** wheelBuffer;
vector<GameObject*> wheelVector;
MachineGunComponent* vehicleMG;

void VehicleComponent::Start()
{
	Initialize();
	cout << "I am a Vehicle Component!" << endl;
	//transform.rendertype = RenderTypes::RT_QUAT;
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
	physVehicle->userData = this;

	physx::PxU32 wheelBufferSize = gVehicleNoDrive->mWheelsSimData.getNbWheels() * sizeof(physx::PxShape*);
	wheelBuffer = new physx::PxShape*[wheelBufferSize];
	physVehicle->getShapes(wheelBuffer, wheelBufferSize);

	for (int i = 0; i < 4; i++) {
		GameObject temp = GameObject();
		physx::PxTransform currWheel = wheelBuffer[i]->getLocalPose();
		temp.mesh = GeoGenerator::MakeCylinder(0.5, 0.5, 0.4, 8, false);
		if(i==0)
			temp.standardMat.diffuseColor = vec3(1, 0, 0);
		if(i==1)
			temp.standardMat.diffuseColor = vec3(0, 1, 0);
		wheelVector.push_back(Game::CreateWorldObject(temp));
	}

	followCam = Renderer::GetCamera(0);
	//followCam->transform.rendertype = RenderTypes::RT_QUAT;
	followCam->mode = Camera::Modes::MODE_GAME;
	transform.position.x = physVehicle->getGlobalPose().p.x;
	transform.position.y = physVehicle->getGlobalPose().p.y;
	transform.position.z = physVehicle->getGlobalPose().p.z;
	Finalize();
}

void VehicleComponent::Update()
{
	Initialize();

	for (int i = 0; i < 4; i++) {
		physx::PxTransform currWheel = wheelBuffer[i]->getLocalPose();

		//Rotate local translation offset vectors
		physx::PxVec3 adjustedPosition = physVehicle->getGlobalPose().q.rotate(wheelBuffer[i]->getLocalPose().p);
		
		//Combine chassis and wheel rotation
		physx::PxQuat totalWheelRotation = physVehicle->getGlobalPose().q * wheelBuffer[i]->getLocalPose().q;

		//Apply rotation to transform.rotation
		wheelVector[i]->transform.rotation.x = totalWheelRotation.x;
		wheelVector[i]->transform.rotation.y = totalWheelRotation.y;
		wheelVector[i]->transform.rotation.z = totalWheelRotation.z;
		wheelVector[i]->transform.rotation.w = totalWheelRotation.w;

		//Apply position to transform.position
		wheelVector[i]->transform.position.x = physVehicle->getGlobalPose().p.x + adjustedPosition.x;
		wheelVector[i]->transform.position.y = physVehicle->getGlobalPose().p.y + adjustedPosition.y;
		wheelVector[i]->transform.position.z = physVehicle->getGlobalPose().p.z + adjustedPosition.z;
		//totalWheelRotation.w;

	}

	physx::PxReal maxTorque = 1500.0;
	physx::PxReal brakeTorque = 400.0;
	physx::PxReal turnTemper = 0.1;

	//Gas
	gVehicleNoDrive->setDriveTorque(0, Input::GetXBoxAxis(1, ButtonCode::XBOX_RIGHT_TRIGGER)*maxTorque);
	gVehicleNoDrive->setDriveTorque(1, Input::GetXBoxAxis(1, ButtonCode::XBOX_RIGHT_TRIGGER)*maxTorque);
	
	//Steering
	gVehicleNoDrive->setSteerAngle(2, Input::GetXBoxAxis(1, ButtonCode::XBOX_JOY_LEFT_HORIZONTAL)*turnTemper);
	gVehicleNoDrive->setSteerAngle(3, Input::GetXBoxAxis(1, ButtonCode::XBOX_JOY_LEFT_HORIZONTAL)*turnTemper);

	//Brake
	//gVehicleNoDrive->setBrakeTorque(0, Input::GetXBoxAxis(1, ButtonCode::XBOX_LEFT_TRIGGER)*brakeTorque);
	//gVehicleNoDrive->setBrakeTorque(1, Input::GetXBoxAxis(1, ButtonCode::XBOX_LEFT_TRIGGER)*brakeTorque);

	//Alt-Brake
	if (Input::GetXBoxAxis(1, ButtonCode::XBOX_LEFT_TRIGGER) > 0.0f)
	{
		
		gVehicleNoDrive->setDriveTorque(0, -Input::GetXBoxAxis(1, ButtonCode::XBOX_LEFT_TRIGGER)*brakeTorque);
		gVehicleNoDrive->setDriveTorque(1, -Input::GetXBoxAxis(1, ButtonCode::XBOX_LEFT_TRIGGER)*brakeTorque);
	}

	//if (Input::GetButton(ButtonCode::MIDDLE_MOUSE))
		//Time::timeScale += Input::GetMouseDelta().x * Time::getDeltaTime();	

	transform.position.x = physVehicle->getGlobalPose().p.x;
	transform.position.y = physVehicle->getGlobalPose().p.y;
	transform.position.z = physVehicle->getGlobalPose().p.z;

	//NOTE INVERSIONS due to coordinate system

	physx::PxQuat rotQuat = physVehicle->getGlobalPose().q;
	transform.rotation.x = rotQuat.x;
	transform.rotation.y = rotQuat.y;
	transform.rotation.z = rotQuat.z;
	transform.rotation.w = rotQuat.w;


	mat4 newRot = glm::mat4_cast(glm::quat(rotQuat.w, rotQuat.x, rotQuat.y, rotQuat.z));
	newRot = glm::inverse(newRot);

	//followCam->transform.rotationMatrix = glm::inverse(newRot);

	followCam->transform.rotation.x = rotQuat.x;
	followCam->transform.rotation.y = rotQuat.y;
	followCam->transform.rotation.z = rotQuat.z;
	followCam->transform.rotation.w = rotQuat.w;

	followCam->transform.rotation = followCam->transform.GetInverseRotation();

	glm::vec4 cameraOffset = glm::vec4(0.0, 5.0, -15.0, 0.0);
	cameraOffset = glm::inverse(newRot) * cameraOffset;
	cameraOffset = glm::vec4(transform.position, 0.0) + cameraOffset;
	followCam->transform.position = cameraOffset;

	//transform.rotationMatrix = newRot;
	//glm::vec4 forward = glm::vec4(0.0, 0.0, 1.0, 0.0);
	//forward = glm::inverse(newRot) * forward;

	//transform.forward.x = forward.x;
	//transform.forward.y = forward.y;
	//transform.forward.z = forward.z;

	Finalize();
}
void VehicleComponent::OnCollision(Component::CollisionPair collisionPair) {
	cout << "Veh Collision" << endl;
	MachineGunComponent* temp = &MachineGunComponent();
	vehicleMG = (MachineGunComponent*)Game::Find(selfName)->GetComponent(temp);
	switch (collisionPair) {
	case(Component::CollisionPair::CP_VEHICLE_POWERUP):
		cout << "pickup" << endl;
		vehicleMG->ammoCount += 100;
		break;
	}
}
