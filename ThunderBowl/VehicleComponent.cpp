#include "VehicleComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "Physics.h"


void VehicleComponent::Start()
{
	Initialize();
	cout << "I am a Vehicle Component!" << endl;
	physVehicle = Physics::createTestBox(2.0);
	physVehicle->setMass(1.0);
	transform.position.x = physVehicle->getGlobalPose().p.x;
	transform.position.y = physVehicle->getGlobalPose().p.y;
	transform.position.z = physVehicle->getGlobalPose().p.z;


	Finalize();
}

void VehicleComponent::Update()
{
	Initialize();
	Camera* followCam = Renderer::GetCamera(0);
	followCam->transform.position = transform.position;
	followCam->transform.position.y += 3.0;
	followCam->transform.position.z -= 10.0;
	//followCam->transform.rotation = transform.rotation;
	//followCam->transform.position.z += 5.0;
	
	if (Input::GetButton(ButtonCode::KEY_UP))
	{
		physx::PxRigidBodyExt::addLocalForceAtLocalPos(*physVehicle, physx::PxVec3(0.0,0.0,50.0), physx::PxVec3(0.0,0.0,0.0));
	}
	else if(Input::GetButton(ButtonCode::KEY_DOWN))
	{
		physx::PxRigidBodyExt::addLocalForceAtLocalPos(*physVehicle, physx::PxVec3(0.0, 0.0, -50.0), physx::PxVec3(0.0, 0.0, 0.0));
	}

	if (Input::GetButton(ButtonCode::KEY_LEFT))
	{
		//physx::PxRigidBodyExt::addLocalForceAtLocalPos(*physVehicle, physx::PxVec3(-50.0, 0.0, 0.0), physx::PxVec3(0.0, 0.0, 0.0));
	}
	else if(Input::GetButton(ButtonCode::KEY_RIGHT))
	{
		//physx::PxRigidBodyExt::addLocalForceAtLocalPos(*physVehicle, physx::PxVec3(50.0, 0.0, 0.0), physx::PxVec3(0.0, 0.0, 0.0));
	}

	if (Input::GetButton(ButtonCode::MIDDLE_MOUSE))
		Time::timeScale += Input::GetMouseDelta().x * Time::getDeltaTime();	

	transform.position.x = physVehicle->getGlobalPose().p.x;
	transform.position.y = physVehicle->getGlobalPose().p.y;
	transform.position.z = physVehicle->getGlobalPose().p.z;

	physx::PxReal qW = physVehicle->getGlobalPose().q.w;
	physx::PxReal qX = physVehicle->getGlobalPose().q.x;
	physx::PxReal qY = physVehicle->getGlobalPose().q.y;
	physx::PxReal qZ = physVehicle->getGlobalPose().q.z;

	/*double qTotal = qX + qY + qZ;
	double pX = qX / qTotal;
	double pY = qY / qTotal;
	double pZ = qZ / qTotal;
	double fX = pX*qW;
	double fY = pY*qW;
	double fZ = pY*qW;

	glm::vec3 eulerRotation = glm::vec3(fX, fY, fZ);
	if (Time::getElapsedFrames() % 100 == 0) {
		cout << "calculated sum: " << fX + fY + fZ << " w: " << qW << endl;
	}
	//transform.Translate((float)currentSpeed * Time::getDeltaTime() * transform.GetForward());
	//transform.Rotate((float)currentTurn * Time::getDeltaTime() * 0.1f*currentSpeed * transform.GetUp());*/
	
	Finalize();
}