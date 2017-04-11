#include "VehicleComponent.h"
#include "Renderer.h"
#include "Physics.h"
#include "GeoGenerator.h"
#include "Game.h"
#include "GameManager.h"
#include "Audio.h"

void VehicleComponent::Start()
{
	Initialize();
	cout << "I am a Vehicle Component! My controller is " << playerNum << endl;
	//transform.rendertype = RenderTypes::RT_QUAT;
	physx::PxPhysics* worldPhys = Physics::getGPhysics();
	physx::PxCooking* worldCook = Physics::getGCooking();
	physx::PxScene* worldScene = Physics::getGScene();	
	myStartPosition = physx::PxVec3(transform.position.x, transform.position.y, transform.position.z);
	myStartRotation = physx::PxQuat(transform.rotation.x, transform.rotation.y, transform.rotation.z, transform.rotation.w);

	
	Physics::VehicleDesc vehicleDesc = Physics::initVehicleDesc();
	gVehicleNoDrive = Physics::createVehicleNoDrive(vehicleDesc, worldPhys, worldCook);
	physx::PxTransform startTransform(physx::PxVec3(0, (vehicleDesc.chassisDims.y*0.5f + vehicleDesc.wheelRadius + 1.0f), 0), physx::PxQuat(physx::PxIdentity));
	Physics::addPlVehicleNoDrive(gVehicleNoDrive);
	gVehicleNoDrive->getRigidDynamicActor()->setGlobalPose(startTransform);

	worldScene->addActor(*gVehicleNoDrive->getRigidDynamicActor());
	
	physVehicle = gVehicleNoDrive->getRigidDynamicActor();
	physVehicle->setGlobalPose(physx::PxTransform(myStartPosition, myStartRotation)); //set global position based on vec created in Game
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
		if (i % 2 == 0)
			temp.staticGeo = SG_LEFT_WHEEL;
		else
			temp.staticGeo = SG_RIGHT_WHEEL;
		temp.tag = TAGS_VEHICLE_WHEEL;
		temp.standardMat.diffuseMap = MAP_WHEEL_DIFFUSE;
		temp.standardMat.roughnessMap = MAP_WHEEL_ROUGHNESS;
		temp.standardMat.metalnessMap = MAP_WHEEL_METALNESS;
		temp.standardMat.normalMap = MAP_WHEEL_NORMAL;
		temp.standardMat.roughness = 1.0;
		temp.standardMat.metalness = 1.0;
		temp.standardMat.isMetallic = true;
		wheelVector.push_back(Game::CreateStaticObject(temp));
	}

	followCam = Renderer::GetCamera(playerNum-1);
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

	followCam = Renderer::GetCamera(playerNum - 1);
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

	physx::PxReal maxTorque = 20000.0;
	physx::PxReal brakeTorque = 20000.0;
	physx::PxReal turnTemper = 0.1 * (1.0+Input::GetXBoxAxis(playerNum, ButtonCode::XBOX_LEFT_TRIGGER)*1.0);

	//Gas
	if (gVehicleNoDrive->computeForwardSpeed() > topSpeed) {
		gVehicleNoDrive->setDriveTorque(0, 0.0);
		gVehicleNoDrive->setDriveTorque(1, 0.0);
	}
	else {
		gVehicleNoDrive->setDriveTorque(0, Input::GetXBoxAxis(playerNum, ButtonCode::XBOX_RIGHT_TRIGGER)*maxTorque);
		gVehicleNoDrive->setDriveTorque(1, Input::GetXBoxAxis(playerNum, ButtonCode::XBOX_RIGHT_TRIGGER)*maxTorque);
	}
	
	//Steering
	gVehicleNoDrive->setSteerAngle(2, Input::GetXBoxAxis(playerNum, ButtonCode::XBOX_JOY_LEFT_HORIZONTAL)*turnTemper);
	gVehicleNoDrive->setSteerAngle(3, Input::GetXBoxAxis(playerNum, ButtonCode::XBOX_JOY_LEFT_HORIZONTAL)*turnTemper);

	//Brake
	//gVehicleNoDrive->setBrakeTorque(0, Input::GetXBoxAxis(playerNum, ButtonCode::XBOX_LEFT_TRIGGER)*brakeTorque);
	//gVehicleNoDrive->setBrakeTorque(1, Input::GetXBoxAxis(playerNum, ButtonCode::XBOX_LEFT_TRIGGER)*brakeTorque);

	//Alt-Brake
	if (gVehicleNoDrive->computeForwardSpeed() < -topReverse)
	{
		gVehicleNoDrive->setDriveTorque(0, 0.0);
		gVehicleNoDrive->setDriveTorque(1, 0.0);
	}

	if (Input::GetXBoxAxis(playerNum, ButtonCode::XBOX_LEFT_TRIGGER) > 0.0f)
	{
		gVehicleNoDrive->setDriveTorque(0, -Input::GetXBoxAxis(playerNum, ButtonCode::XBOX_LEFT_TRIGGER)*brakeTorque);
		gVehicleNoDrive->setDriveTorque(1, -Input::GetXBoxAxis(playerNum, ButtonCode::XBOX_LEFT_TRIGGER)*brakeTorque);
		//Tail / brake lights
		if (Input::GetXBoxAxis(playerNum, ButtonCode::XBOX_LEFT_TRIGGER) > 0.1f)
		{
			Transform t = transform;
			t.rotation = t.GetInverseRotation();
			float speed = physVehicle->getLinearVelocity().magnitude();
			PointLight light;
			physx::PxVec3 v = physVehicle->getLinearVelocity();
			vec3 dir = normalize(vec3(v.x, v.y, v.z));
			if (dot(dir, t.GetForward()) > 0.0)
			{
				speed = -speed;
				light.Color = vec4(1, 0, 0, 7);
			}
			else
			{
				light.Color = vec4(vec3(0.4), 7);
			}
			t.position = t.position - (3.3f + speed*0.016f)*t.GetForward() + 0.8f*t.GetUp();
			light.Pos = vec4(t.position - 1.2f*t.GetRight(), 1.8);
			HealthComponent* health = &HealthComponent();
			health = (HealthComponent*)Game::Find(selfName)->GetComponent(health);
			if(health->currentHealth > 66.67)
				Renderer::AddPointLight(light);
			light.Pos = vec4(t.position + 1.2f*t.GetRight(), 1.8);
			Renderer::AddPointLight(light);
		}
	}

	if (Input::GetXBoxButton(playerNum, ButtonCode::XBOX_A))
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

	HealthComponent* myHealth = &HealthComponent();
	myHealth = (HealthComponent*)Game::Find(selfName)->GetComponent(myHealth);

	if (followCam->mode == Camera::Modes::MODE_GAME && myHealth->currentHealth > 0.0)
	{
		//followCam->transform.rotation.x = rotQuat.x*0.5;
		//// rotQuat.x;
		//followCam->transform.rotation.y = rotQuat.y*1.0;
		//followCam->transform.rotation.z = rotQuat.z*0.5;
		////rotQuat.z;
		//followCam->transform.rotation.w = rotQuat.w;
		//
		//followCam->transform.rotation = normalize(followCam->transform.rotation);
		//followCam->transform.rotation = followCam->transform.GetInverseRotation();
		//
		//glm::vec4 cameraOffset = glm::vec4(0.0,	4.0, -12.0, 0.0);
		//cameraOffset = newRot * cameraOffset;
		//cameraOffset = glm::vec4(transform.position, 0.0) + cameraOffset;
		//followCam->transform.position = cameraOffset;

		//Transform parent = transform;
		//parent.rotation = parent.GetInverseRotation();
		followCam->transform = transform;
		//CAMERA JOCKEY
		vec4 standardRot = transform.GetInverseRotation();
		followCam->transform.rotation = normalize(
			vec4(standardRot.x*0.8, standardRot.y*1.0f, standardRot.z*0.8, standardRot.w));

		//CAMERA SMOOTH FOLLOW (YAW ONLY)
		float desiredYaw = Mathf::PI * 0.2 * Input::GetXBoxAxis(playerNum, ButtonCode::XBOX_JOY_LEFT_HORIZONTAL) *
			(Input::GetXBoxAxis(playerNum, ButtonCode::XBOX_LEFT_TRIGGER)*1.8 + 0.5);
		float snappiness = 8.0 - 6.0*abs(Input::GetXBoxAxis(playerNum, ButtonCode::XBOX_JOY_LEFT_HORIZONTAL));
		if (currentYaw != desiredYaw)
		{
			currentYaw += (desiredYaw - currentYaw) * Time::getDeltaTime() * snappiness;
		}
		followCam->transform.Rotate(followCam->transform.GetUp(), currentYaw, false);

		//CAMERA OFFSET
		followCam->transform.Translate(followCam->transform.GetForward() * -12.0f, false);
		followCam->transform.Translate(followCam->transform.GetUp() * 4.0f, false);

		if (Input::GetXBoxButton(playerNum, ButtonCode::XBOX_X))
		{
			followCam->transform.Rotate(followCam->transform.GetUp(), Mathf::PI, false);
			followCam->transform.Translate(followCam->transform.GetForward() * -24.0f, false);
		}
	}
	//transform.rotationMatrix = newRot;
	//glm::vec4 forward = glm::vec4(0.0, 0.0, 1.0, 0.0);
	//forward = glm::inverse(newRot) * forward;

	//transform.forward.x = forward.x;
	//transform.forward.y = forward.y;
	//transform.forward.z = forward.z;

	if (transform.position.y < -20)
	{
		HealthComponent* health = &HealthComponent();
		health = (HealthComponent*)Game::Find(selfName)->GetComponent(health);
		health->currentHealth = 0.0;
	}

	//IFNDEF_SPEEDOMETER
	GameObject* speedNeedle = Game::Find("SpeedometerNeedle"+playerNum);
	speedNeedle->transform.rotation = vec4(0, 0, 0, 1);
	speedNeedle->transform.scale = vec3(1);
	float speed = physVehicle->getLinearVelocity().magnitude() * 3.6f;
	float angle = -0.002f * Mathf::PI * speed; //At full the needle points to 500 km/h
	float difference = abs(Mathf::PI*0.5f - abs(angle))/(Mathf::PI*0.5f);
	cout << "D " << difference << endl;
	float scale = (1.0f - difference) * 0.5f;
	if (myHealth->currentHealth > 0.0)
	{
		speedNeedle->transform.Rotate(Transform::Forward(), angle, false);
		speedNeedle->transform.scale.x = 1.0 + scale;
	}
	//ENDIF_SPEEDOMETER
	if(followCam->mode == Camera::Modes::MODE_GAME)
		followCam->SetVerticalFOV(60 + speed*0.08);

	Input::SetControllerVibration(playerNum, 0.06 + speed*0.0001, 0.0);

	//IF_DEF Wheel Spray Particles
	UpdateParticles();
	//END_IF Wheel Spray Particles

	//using the game timer play engine sound so it doesn't overplay
	float time = GameManager::GetGameTime();
	time = time * 1000;
	int time2 = (int)time;
	time = time - time2;
	time2 = (int)(time * 10);

	if (time2 == 9)
	{
		/*
		float throttleOpen = Input::GetXBoxAxis(playerNum, ButtonCode::XBOX_RIGHT_TRIGGER);

		if (throttleOpen > 0.1)
		{		
			//gear 1
			if (speed < 5)
			{
				Audio::Play2DSound(SFX_Engine1, 0.2, 0.0);
			}
			else if (speed < 10)
			{
				Audio::Play2DSound(SFX_Engine2, 0.2, 0.0);
			}
			else if (speed < 15)
			{
				Audio::Play2DSound(SFX_Engine3, 0.2, 0.0);
			}
			else if (speed < 20)
			{
				Audio::Play2DSound(SFX_Engine4, 0.2, 0.0);
			}
			else if (speed < 25)
			{
				Audio::Play2DSound(SFX_Engine5, 0.2, 0.0);
			}
			else if (speed < 30)
			{
				Audio::Play2DSound(SFX_Engine6, 0.2, 0.0);
			}

			//gear 2
			else if (speed < 30)
			{
				Audio::Play2DSound(SFX_Engine1, 0.2, 0.0);
			}
			else if (speed < 40)
			{
				Audio::Play2DSound(SFX_Engine2, 0.2, 0.0);
			}
			else if (speed < 50)
			{
				Audio::Play2DSound(SFX_Engine3, 0.2, 0.0);
			}
			else if (speed < 60)
			{
				Audio::Play2DSound(SFX_Engine4, 0.2, 0.0);
			}
			else if (speed < 70)
			{
				Audio::Play2DSound(SFX_Engine5, 0.2, 0.0);
			}
			else if (speed < 80)
			{
				Audio::Play2DSound(SFX_Engine6, 0.2, 0.0);
			}

			//gear 3
			else if (speed < 100)
			{
				Audio::Play2DSound(SFX_Engine1, 0.2, 0.0);
			}
			else if (speed < 120)
			{
				Audio::Play2DSound(SFX_Engine2, 0.2, 0.0);
			}
			else if (speed < 140)
			{
				Audio::Play2DSound(SFX_Engine3, 0.2, 0.0);
			}
			else if (speed < 160)
			{
				Audio::Play2DSound(SFX_Engine4, 0.2, 0.0);
			}
			else if (speed < 180)
			{
				Audio::Play2DSound(SFX_Engine5, 0.2, 0.0);
			}
			else if (speed < 200)
			{
				Audio::Play2DSound(SFX_Engine6, 0.2, 0.0);
			}

			//gear 4
			else if (speed < 200)
			{
				Audio::Play2DSound(SFX_Engine1, 0.2, 0.0);
			}
			else if (speed < 220)
			{
				Audio::Play2DSound(SFX_Engine2, 0.2, 0.0);
			}
			else if (speed < 240)
			{
				Audio::Play2DSound(SFX_Engine3, 0.2, 0.0);
			}
			else if (speed < 260)
			{
				Audio::Play2DSound(SFX_Engine4, 0.2, 0.0);
			}
			else if (speed < 280)
			{
				Audio::Play2DSound(SFX_Engine5, 0.2, 0.0);
			}
			else if (speed < 300)
			{
				Audio::Play2DSound(SFX_Engine6, 0.2, 0.0);
			}


		}
		else
		{ 
		*/
		if (speed < 75)
		{
			Audio::Play2DSound(SFX_Engine1, 0.1, 0.0);
		}
		else if (speed < 150)
		{
			Audio::Play2DSound(SFX_Engine2, 0.1, 0.0);
		}
		else if (speed < 225)
		{
			Audio::Play2DSound(SFX_Engine3, 0.1, 0.0);
		}
		else if (speed < 300)
		{
			Audio::Play2DSound(SFX_Engine4, 0.1, 0.0);
		}
		else if (speed < 375)
		{
			Audio::Play2DSound(SFX_Engine5, 0.1, 0.0);
		}
		else
		{
			Audio::Play2DSound(SFX_Engine6, 0.1, 0.0);
		}
		
	}
	nextEngine = engineDelay;

	//}
	//END_IF ENGINE LOOP SOUND

	if (Input::GetXBoxButton(playerNum, ButtonCode::XBOX_LEFT_STICK) && hornReady)
	{
		Audio::Play2DSound(SFX_Horn, Random::rangef(0.2, 0.3), 0.0);
		hornReady = false;
	}
	if (!Input::GetXBoxButton(playerNum, ButtonCode::XBOX_LEFT_STICK) && !hornReady)
		hornReady = true;

	Finalize();
}

void VehicleComponent::OnCollision(Component::CollisionPair collisionPair, Component* collider) 
{
	Initialize();

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

	HealthComponent* health = &HealthComponent();
	health = (HealthComponent*)Game::Find(selfName)->GetComponent(health);

	float speed = physVehicle->getLinearVelocity().magnitude();
	for (int i = 0; i < systems.size(); i++)
	{
		systems[i]->spawnRate = speed * 0.5;
		systems[i]->spawnRate = Mathf::Clamp(systems[i]->spawnRate, 0.0, 20.0);

		if (health->currentHealth <= 0.0)
			systems[i]->spawnRate = 0.0;

		//systems[i]->initialSpeed.min = speed * 1.0;
		//systems[i]->initialSpeed.max = speed * 1.3;

		systems[i]->initialRadius.min = 0.4 + speed*0.008;
		systems[i]->initialRadius.max = 0.7 + speed*0.008;

		if (systems[i]->transform.position.y < 8.8)
		{
			systems[i]->initialColor.alpha = vec4(0.8, 0.9, 1.0, 1.0);
			systems[i]->initialColor.bravo = vec4(0.6, 0.7, 0.9, 1.0);

			if (GameManager::isBloodMoon)
			{
				systems[i]->initialColor.alpha = vec4(1.0, 0.2, 0.2, 1);
				systems[i]->initialColor.bravo = vec4(1.0, 0.4, 0.4, 1);
			}
		}
		else
		{
			systems[i]->initialColor.alpha = vec4(0.2, 0.1, 0.0, 1);
			systems[i]->initialColor.bravo = vec4(0.3, 0.15, 0.0, 1);
		}
	}

	if (transform.position.y < 9.5 && !enteredWaterPrev)
	{

		Audio::Play2DSound(SFX_Splash, Random::rangef(0.1, 0.25), 0.0);

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

		if (GameManager::isBloodMoon)
		{
			ps.initialColor.alpha = vec4(1.0, 0.2, 0.2, 1);
			ps.initialColor.bravo = vec4(1.0, 0.4, 0.4, 1);
		}

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

//Get controller number of this vehicle for firing weapons
int VehicleComponent::GetPlayerNum()
{
	return playerNum;
}

//Set the controller number 1-4 for this vehicle
void VehicleComponent::SetPlayerNum(int playerNumber)
{
	playerNum = playerNumber;
}
