#include "VehicleComponent.h"
#include "Renderer.h"
#include "Physics.h"
#include "GeoGenerator.h"
#include "Game.h"
#include "Audio.h"


void VehicleComponent::Start()
{
	Initialize();
	cout << "I am a Vehicle Component!" << endl;
	//transform.rendertype = RenderTypes::RT_QUAT;
	physx::PxPhysics* worldPhys = Physics::getGPhysics();
	physx::PxCooking* worldCook = Physics::getGCooking();
	physx::PxScene* worldScene = Physics::getGScene();	
	myStartPosition = physx::PxVec3(transform.position.x, transform.position.y, transform.position.z);
	
	Physics::VehicleDesc vehicleDesc = Physics::initVehicleDesc();
	gVehicleNoDrive = Physics::createVehicleNoDrive(vehicleDesc, worldPhys, worldCook);
	physx::PxTransform startTransform(physx::PxVec3(0, (vehicleDesc.chassisDims.y*0.5f + vehicleDesc.wheelRadius + 1.0f), 0), physx::PxQuat(physx::PxIdentity));
	Physics::addPlVehicleNoDrive(gVehicleNoDrive);
	gVehicleNoDrive->getRigidDynamicActor()->setGlobalPose(startTransform);

	worldScene->addActor(*gVehicleNoDrive->getRigidDynamicActor());
	
	physVehicle = gVehicleNoDrive->getRigidDynamicActor();
	physVehicle->setGlobalPose(physx::PxTransform(myStartPosition, physx::PxQuat(physx::PxIdentity))); //set global position based on vec created in Game
	PlayerComponent* player = &PlayerComponent();
	player = (PlayerComponent*)Game::Find(selfName)->GetComponent(player);
	physVehicle->userData = player;

	physx::PxU32 wheelBufferSize = gVehicleNoDrive->mWheelsSimData.getNbWheels() * sizeof(physx::PxShape*);
	wheelBuffer = new physx::PxShape*[wheelBufferSize];
	physVehicle->getShapes(wheelBuffer, wheelBufferSize);

	for (int i = 0; i < 4; i++) 
	{
		GameObject temp = GameObject();
		physx::PxTransform currWheel = wheelBuffer[i]->getLocalPose();
		temp.mesh = GeoGenerator::MakeCylinder(0.5, 0.5, 0.6, 16, false); //change to take in physx values
		//if(i==0)
		//	temp.standardMat.diffuseColor = vec3(1, 0, 0);
		//if(i==1)
		//	temp.standardMat.diffuseColor = vec3(0, 1, 0);
		temp.standardMat.diffuseColor = vec3(0);
		temp.standardMat.roughness = 0.5;
		temp.standardMat.metalness = 0.2;
		temp.standardMat.isMetallic = false;
		wheelVector.push_back(Game::CreateWorldObject(temp));
	}

	followCam = Renderer::GetCamera(0);
	//followCam->transform.rendertype = RenderTypes::RT_QUAT;
	followCam->mode = Camera::Modes::MODE_GAME;
	transform.position.x = physVehicle->getGlobalPose().p.x;
	transform.position.y = physVehicle->getGlobalPose().p.y;
	transform.position.z = physVehicle->getGlobalPose().p.z;

	//cout << physVehicle->getLinearVelocity().magnitude() << endl;
	//currentSpeed = physVehicle->getLinearVelocity().magnitude();

	//IF_DEF Wheel Spray particle systems

	ParticleSystem ps = ParticleSystem();
	ps.name = selfName + "WheelSprayLeft";
	ps.initialSpeed.min = 3;
	ps.initialSpeed.max = 4;
	ps.coneAngle = 0;
	ps.gravityScale = 2.0;
	ps.initialColor.alpha = vec4(0.2, 0.1, 0.0, 1);
	ps.initialColor.bravo = vec4(0.3, 0.15, 0.0, 1);
	ps.initialRadius.min = 1.1;
	ps.initialRadius.max = 1.4;
	ps.lifeSpan.min = 0.4;
	ps.lifeSpan.max = 0.5;
	ps.spawnPointVariance = vec3(0.5);
	ps.monochromatic = false;
	ps.mainTexture = MAP_SMOKE_PART;
	ps.spawnRate = 0;
	ps.destroySystemWhenEmpty = false;
	ps.useSystemLifespan = false;
	Game::CreateParticleObject(ps);
	wheelSprayNameLeft = ps.name;

	ps.name = selfName + "WheelSprayRight";
	Game::CreateParticleObject(ps);
	wheelSprayNameRight = ps.name;

	UpdateParticles();
	//END_IF Wheel Spray particle systems

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

	physx::PxReal maxTorque = 6000.0;
	physx::PxReal brakeTorque = 3000.0;
	physx::PxReal turnTemper = 0.10;

	//Gas
	if (gVehicleNoDrive->computeForwardSpeed() > topSpeed) {
		gVehicleNoDrive->setDriveTorque(0, 0.0);
		gVehicleNoDrive->setDriveTorque(1, 0.0);
	}
	else {
		gVehicleNoDrive->setDriveTorque(0, Input::GetXBoxAxis(1, ButtonCode::XBOX_RIGHT_TRIGGER)*maxTorque);
		gVehicleNoDrive->setDriveTorque(1, Input::GetXBoxAxis(1, ButtonCode::XBOX_RIGHT_TRIGGER)*maxTorque);
	}
	
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

	if (Input::GetXBoxButton(1, ButtonCode::XBOX_A))
	{
		physVehicle->setLinearVelocity(physx::PxVec3(0.0, 0.0, 0.0));
		physVehicle->setAngularVelocity(physx::PxVec3(0.0, 0.0, 0.0));
		gVehicleNoDrive->setDriveTorque(0, 0.0);
		gVehicleNoDrive->setDriveTorque(1, 0.0);
		physVehicle->setGlobalPose(physx::PxTransform(physVehicle->getGlobalPose().p, physx::PxQuat(physx::PxIdentity)));
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

	//followCam->transform.rotationMatrix = glm::inverse(newRot);

	if (followCam->mode == Camera::Modes::MODE_GAME)
	{
		followCam->transform.rotation.x = rotQuat.x*0.5;
		// rotQuat.x;
		followCam->transform.rotation.y = rotQuat.y;
		followCam->transform.rotation.z = rotQuat.z*0.5;
		//rotQuat.z;
		followCam->transform.rotation.w = rotQuat.w;

		followCam->transform.rotation = normalize(followCam->transform.rotation);
		followCam->transform.rotation = followCam->transform.GetInverseRotation();


		glm::vec4 cameraOffset = glm::vec4(0.0, 5.0, -15.0, 0.0);
		cameraOffset = newRot * cameraOffset;
		cameraOffset = glm::vec4(transform.position, 0.0) + cameraOffset;
		followCam->transform.position = cameraOffset;
	}
	//transform.rotationMatrix = newRot;
	//glm::vec4 forward = glm::vec4(0.0, 0.0, 1.0, 0.0);
	//forward = glm::inverse(newRot) * forward;

	//transform.forward.x = forward.x;
	//transform.forward.y = forward.y;
	//transform.forward.z = forward.z;

	if (transform.position.y < -20)
	{
		physVehicle->setGlobalPose(physx::PxTransform(myStartPosition, physx::PxQuat(physx::PxIdentity)));
		physVehicle->setAngularVelocity(physx::PxVec3(0, 0, 0));
		physVehicle->setLinearVelocity(physx::PxVec3(0, 0, 0));
	}

	//IFNDEF_SPEEDOMETER
	GameObject* speedNeedle = Game::Find("SpeedometerNeedle");
	speedNeedle->transform.rotation = vec4(0, 0, 0, 1);
	float speed = physVehicle->getLinearVelocity().magnitude() * 3.6;
	float angle = -0.002 * Mathf::PI * speed; //At full the needle points to 500 km/h
	speedNeedle->transform.Rotate(Transform::Forward(), angle, false);
	//ENDIF_SPEEDOMETER

	//IF_DEF Wheel Spray Particles
	UpdateParticles();
	//END_IF Wheel Spray Particles

	Finalize();
}

void VehicleComponent::OnCollision(Component::CollisionPair collisionPair, Component* collider) 
{
	Initialize();

	//Note: Ammo and damage now handled in PlayerComponent OnCollision() to avoid duplicate logic

	//MachineGunComponent* mgRef = &MachineGunComponent();
	//HealthComponent* playerHealth = &HealthComponent();
	//PlayerComponent* playerRef = &PlayerComponent();
	//
	//switch (collisionPair) 
	//{
	//case(Component::CollisionPair::CP_VEHICLE_POWERUP):
	//	Audio::Play2DSound(SFX_Powerup, Random::rangef(0.20, 0.50), 0.0);
	//	playerRef = (PlayerComponent*)Game::Find(selfName)->GetComponent(playerRef);
	//	playerRef->playerScore += 10.0;
	//	playerRef->machineGunAmmo += 50;
	//	if (playerRef->machineGunAmmo > playerRef->MAX_MACHINE_GUN_AMMO)
	//		playerRef->machineGunAmmo = playerRef->MAX_MACHINE_GUN_AMMO;
	//	break;
	//case(Component::CollisionPair::CP_VEHICLE_PROJECTILE):
	//	Audio::Play2DSound(SFX_Hit, Random::rangef(0.20, 0.50), 0.0);
	//	playerHealth = (HealthComponent*)Game::Find(selfName)->GetComponent(playerHealth);
	//	playerHealth->currentHealth -= 10.0;
	//	break;
	//}

	Finalize();
}

void VehicleComponent::UpdateParticles()
{
	//Get 'em
	ParticleSystem* wheelSprayLeft = (ParticleSystem*)Game::Find(wheelSprayNameLeft);
	ParticleSystem* wheelSprayRight = (ParticleSystem*)Game::Find(wheelSprayNameRight);

	Transform t = transform;
	t.rotation = t.GetInverseRotation();

	//Move 'em
	wheelSprayLeft->transform = t;
	wheelSprayLeft->transform.Translate(-t.GetRight()*2.0f, false);
	wheelSprayLeft->transform.Translate(-t.GetForward()*3.0f, false);
	wheelSprayLeft->transform.Translate(-t.GetUp()*1.0f, false);
	wheelSprayLeft->transform.Rotate(t.GetUp(), Mathf::PI, false);
	wheelSprayLeft->transform.Rotate(t.GetRight(), Mathf::PI*0.4, false);

	wheelSprayRight->transform = t;
	wheelSprayRight->transform.Translate(t.GetRight()*2.0f, false);
	wheelSprayRight->transform.Translate(-t.GetForward()*3.0f, false);
	wheelSprayRight->transform.Translate(-t.GetUp()*1.0f, false);
	wheelSprayRight->transform.Rotate(t.GetUp(), Mathf::PI, false);
	wheelSprayRight->transform.Rotate(t.GetRight(), Mathf::PI*0.4, false);

	//Change 'em
	vector<ParticleSystem*> systems;
	systems.push_back(wheelSprayLeft);
	systems.push_back(wheelSprayRight);

	float speed = physVehicle->getLinearVelocity().magnitude();
	for (int i = 0; i < systems.size(); i++)
	{
		systems[i]->spawnRate = speed * 0.5;
		systems[i]->spawnRate = Mathf::Clamp(systems[i]->spawnRate, 0.0, 20.0);

		//systems[i]->initialSpeed.min = speed * 1.0;
		//systems[i]->initialSpeed.max = speed * 1.3;

		systems[i]->initialRadius.min = 0.4 + speed*0.008;
		systems[i]->initialRadius.max = 0.7 + speed*0.008;

		if (systems[i]->transform.position.y < 8.0)
		{
			systems[i]->initialColor.alpha = vec4(0.8, 0.9, 1.0, 1.0);
			systems[i]->initialColor.bravo = vec4(0.6, 0.7, 0.9, 1.0);
		}
		else
		{
			systems[i]->initialColor.alpha = vec4(0.2, 0.1, 0.0, 1);
			systems[i]->initialColor.bravo = vec4(0.3, 0.15, 0.0, 1);
		}
	}

	if (transform.position.y < 8.5 && !enteredWaterPrev)
	{
		Audio::Play2DSound(SFX_Splash, Random::rangef(0.4, 0.5), 0.0);
		ParticleSystem ps = ParticleSystem();
		ps.name = selfName + "Splash";
		ps.transform = t;
		ps.transform.Translate(t.GetForward()*2.0f, false);
		ps.initialSpeed.min = 50;
		ps.initialSpeed.max = 50;
		ps.accelerationScale = 0.91;
		ps.coneAngle = 360;
		ps.gravityScale = 68;
		ps.initialColor.alpha = vec4(0.8, 0.9, 1.0, 1.0);
		ps.initialColor.bravo = vec4(0.6, 0.7, 0.9, 1.0);
		ps.initialRadius.min = 1.0;
		ps.initialRadius.max = 1.4;
		ps.lifeSpan.min = 0.4;
		ps.lifeSpan.max = 0.5;
		ps.spawnPointVariance = vec3(0.5);
		ps.monochromatic = false;
		ps.mainTexture = MAP_SMOKE_PART;
		ps.spawnRate = 0.0;
		ps.spawnPointVariance = vec3(1.5);
		ps.destroySystemWhenEmpty = true;
		ParticleSystem *splashPtr = Game::CreateParticleObject(ps);
		splashPtr->AddParticleBurst(8, 0.0);
		enteredWaterPrev = true;
	}
	if (transform.position.y > 10.0f)
	{
		enteredWaterPrev = false;
	}
}
