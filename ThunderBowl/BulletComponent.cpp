#include "BulletComponent.h"
#include "PlayerComponent.h"
#include "HealthComponent.h"
#include "Physics.h"
#include "Game.h"
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
	if (selfGameObject->tag == TAGS_HUMAN_PLAYER) {
		VehicleComponent* temp = &VehicleComponent();
		thisVeh = (VehicleComponent*)Game::Find(ownerName)->GetComponent(temp);
	}
	else {
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
	position.y = transform.position.y;// +2.5;
	position.z = transform.position.z;
	bullet->setGlobalPose(physx::PxTransform(position));

	transform.rotation = transform.GetInverseRotation();

	vec3 newForward = transform.GetForward();

	physx::PxVec3 forward;
	forward.x = newForward.x;
	forward.y = newForward.y;
	forward.z = newForward.z;

	if (selfGameObject->tag == TAGS_HUMAN_PLAYER) {
		bullet->setLinearVelocity(forward*(speed + thisVeh->physVehicle->getLinearVelocity().magnitude()));
	}
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

	Finalize();
	
	if (lifeSpan <= 0.0)
	{	
		selfGameObject = Game::Find(selfName);
		//cout << selfGameObject->objectID << " died" << endl;
		worldScene->removeActor(*bullet);
		bullet->release();
		Game::DestroyWorldObjectAt(selfGameObject->objectID);
		//cout << "OBJECTIVELY SPEAKING: " << Game::worldObjectList.size() << endl;
	}
}

void BulletComponent::OnCollision(Component::CollisionPair collisionPair, Component* collider) {
	Initialize();

	PlayerComponent* playerRef = &PlayerComponent();
	HealthComponent* targetHealthRef = &HealthComponent();

	switch (collisionPair) 
	{
	case(Component::CollisionPair::CP_VEHICLE_PROJECTILE):
		Audio::Play2DSound(SFX_Hit, Random::rangef(0.20, 0.50), 0.0);
		MGShooter = (PlayerComponent*)Game::Find(ownerName)->GetComponent(playerRef);
		MGShooter->playerScore += 10.0;
		targetHealth = (HealthComponent*)Game::Find(collider->getName())->GetComponent(targetHealthRef);
		targetHealth->currentHealth -= 25.0;

		break;
	}

	if (targetHealth->isDead()) {
		MGShooter->playerScore += 100.0;
	}
	Finalize();
}