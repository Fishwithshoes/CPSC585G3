#include "BulletComponent.h"
#include "PlayerComponent.h"
#include "HealthComponent.h"
#include "Physics.h"
#include "Game.h"
#include "GameManager.h"
#include "GameObject.h"
#include "Audio.h"

GameObject* selfGameObject;
PlayerComponent* MGShooter;
HealthComponent* targetHealth;
physx::PxPhysics* worldPhys;
physx::PxCooking* worldCook;
physx::PxScene* worldScene;

BulletComponent::BulletComponent(string owner) {
	ownerName = owner;
}

void BulletComponent::Start()
{
	Initialize();

	VehicleComponent* thisVeh;
	EnemyComponent* aiVeh;

	selfGameObject = Game::Find(ownerName);
	if (selfGameObject->tag == TAGS_HUMAN_PLAYER) 
	{
		VehicleComponent* temp = &VehicleComponent();
		thisVeh = (VehicleComponent*)Game::Find(ownerName)->GetComponent(temp);
	}
	else 
	{
		EnemyComponent* temp = &EnemyComponent();
		aiVeh = (EnemyComponent*)Game::Find(ownerName)->GetComponent(temp);
	}


	speed = 200.0f;
	lifeSpan = 1.00f;
	worldPhys = Physics::getGPhysics();
	worldCook = Physics::getGCooking();
	worldScene = Physics::getGScene();

	bullet = Physics::createTestProjectile();
	bullet->userData = this;

	physx::PxVec3 position;
	position.x = transform.position.x;
	position.y = transform.position.y;
	position.z = transform.position.z;
	bullet->setGlobalPose(physx::PxTransform(position));

	vec3 newForward = transform.GetForward();

	physx::PxVec3 forward;
	forward.x = newForward.x;
	forward.y = newForward.y;
	forward.z = newForward.z;

	if (selfGameObject->tag == TAGS_HUMAN_PLAYER)
		bullet->setLinearVelocity(forward*(speed + thisVeh->physVehicle->getLinearVelocity().magnitude()));
	else
		bullet->setLinearVelocity(forward*(speed + aiVeh->enPhysVehicle->getLinearVelocity().magnitude()));

	Finalize();
}

void BulletComponent::Update()
{
	Initialize();

	selfGameObject = Game::Find(selfName);

	transform.position.x = bullet->getGlobalPose().p.x;
	transform.position.y = bullet->getGlobalPose().p.y;
	transform.position.z = bullet->getGlobalPose().p.z;

	lifeSpan -= Time::getDeltaTime();

	//IF_DEF SPLASH
	if (transform.position.y < 7.5 && !splashed)
	{
		Audio::Play2DSound(SFX_Splish, Random::rangef(0.3, 0.4), 0.0);
		ParticleSystem ps = ParticleSystem();
		ps.name = selfName + "Splish";
		ps.transform = transform;
		ps.transform.rotation = normalize(vec4(1, 0, 0, 1));
		ps.transform.Translate(transform.GetForward()*2.0f, false);
		ps.initialSpeed.min = 28;
		ps.initialSpeed.max = 35;
		ps.coneAngle = 0;
		ps.gravityScale = 92;
		ps.initialColor.alpha = vec4(0.8, 0.9, 1.0, 1.0);
		ps.initialColor.bravo = vec4(0.6, 0.7, 0.9, 1.0);
		if (GameManager::isBloodMoon)
		{
			ps.initialColor.alpha = vec4(1.0, 0.2, 0.2, 1);
			ps.initialColor.bravo = vec4(1.0, 0.4, 0.4, 1);
		}
		ps.initialRadius.min = 0.3;
		ps.initialRadius.max = 0.6;
		ps.lifeSpan.min = 0.4;
		ps.lifeSpan.max = 0.5;
		ps.monochromatic = false;
		ps.mainTexture = MAP_SMOKE_PART;
		ps.spawnRate = 0.0;
		ps.spawnPointVariance = vec3(0.2);
		ps.destroySystemWhenEmpty = true;
		ParticleSystem *sparkPtr = Game::CreateParticleObject(ps);
		sparkPtr->AddParticleBurst(1, 0.0);
		splashed = true;
	}
	//END_IF SPLASH

	Finalize();
	
	if (lifeSpan <= 0.0)
	{	
		selfGameObject = Game::Find(selfName);
		//cout << selfGameObject->objectID << " died" << endl;
		worldScene->removeActor(*bullet);
		bullet->release();
		Game::DestroyStaticObjectAt(selfGameObject->objectID);
		//cout << "OBJECTIVELY SPEAKING: " << Game::worldObjectList.size() << endl;
	}
}

void BulletComponent::OnCollision(Component::CollisionPair collisionPair, Component* collider) 
{
	Initialize();

	PlayerComponent* playerRef = &PlayerComponent();
	HealthComponent* targetHealthRef = &HealthComponent();

	switch (collisionPair)
	{
	case(Component::CollisionPair::CP_VEHICLE_PROJECTILE):

		if (ownerName != collider->getName())//Thou shalt not shoot thyself!
		{
			//IF_DEF SPARKS
			ParticleSystem ps = ParticleSystem();
			ps.name = selfName + "Spark";
			ps.transform = transform;
			ps.transform.Translate(transform.GetForward()*2.0f, false);
			ps.initialSpeed.min = 30;
			ps.initialSpeed.max = 36;
			ps.coneAngle = 360;
			ps.gravityScale = 82;
			ps.initialColor.alpha = vec4(1.0, 1.0, 1.0, 1.0);
			ps.initialColor.bravo = vec4(0.8, 0.8, 1.0, 1.0);
			ps.initialRadius.min = 1.2;
			ps.initialRadius.max = 1.6;
			ps.lifeSpan.min = 0.4;
			ps.lifeSpan.max = 0.5;
			ps.monochromatic = false;
			ps.mainTexture = MAP_DEFAULT_PART;
			ps.textures =
			{ MAP_SPARK01_PART, MAP_SPARK02_PART, MAP_SPARK03_PART, MAP_SPARK04_PART,
				MAP_SPARK05_PART, MAP_SPARK06_PART, MAP_SPARK07_PART, MAP_SPARK08_PART };
			ps.spawnRate = 0.0;
			ps.spawnPointVariance = vec3(0.0);
			ps.destroySystemWhenEmpty = true;
			ParticleSystem *sparkPtr = Game::CreateParticleObject(ps);
			sparkPtr->AddParticleBurst(3, 0.0);
			//END_IF SPARKS

			Audio::Play2DSound(SFX_Hit, Random::rangef(0.20, 0.50), 0.0);
			MGShooter = (PlayerComponent*)Game::Find(ownerName)->GetComponent(playerRef);
			MGShooter->playerScore += 10.0;
			targetHealth = (HealthComponent*)Game::Find(collider->getName())->GetComponent(targetHealthRef);
			targetHealth->currentHealth -= 10.0;

			if (targetHealth->isDead())
			{
				MGShooter->playerScore += 100.0;
			}
		}

		break;
	}
	Finalize();
}