#include "BulletComponent.h"
#include "PlayerComponent.h"
#include "Physics.h"
#include "Game.h"
#include "GameObject.h"

GameObject* selfGameObject;
PlayerComponent* shooter;
physx::PxPhysics* worldPhys;
physx::PxCooking* worldCook;
physx::PxScene* worldScene;

void BulletComponent::Start()
{
	Initialize();

	selfGameObject = Game::Find(selfName);
	speed = 200.0f;
	lifeSpan = 1.00f;
	worldPhys = Physics::getGPhysics();
	worldCook = Physics::getGCooking();
	worldScene = Physics::getGScene();

	VehicleComponent* temp = &VehicleComponent();
	VehicleComponent* thisVeh = (VehicleComponent*) Game::Find("Player0")->GetComponent(temp);

	bullet = Physics::createTestProjectile();
	bullet->userData = this;

	physx::PxVec3 position;
	position.x = transform.position.x;
	position.y = transform.position.y + 2.5;
	position.z = transform.position.z;
	bullet->setGlobalPose(physx::PxTransform(position));

	transform.rotation = transform.GetInverseRotation();

	vec3 newForward = transform.GetForward();

	physx::PxVec3 forward;
	forward.x = newForward.x;
	forward.y = newForward.y;
	forward.z = newForward.z;

	bullet->setLinearVelocity(forward*(speed + thisVeh->physVehicle->getLinearVelocity().magnitude()));

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
		bullet->putToSleep();
		bullet->setGlobalPose(physx::PxTransform(physx::PxVec3(0.0, 100.00, 0.0), physx::PxQuat(physx::PxIdentity)), false);
		worldScene->removeActor(*bullet);
		bullet->release();
		Game::DestroyWorldObjectAt(selfGameObject->objectID);
		//cout << "OBJECTIVELY SPEAKING: " << Game::worldObjectList.size() << endl;
	}
}

void BulletComponent::OnCollision(Component::CollisionPair collisionPair) {
	Initialize();

	PlayerComponent* playerRef = &PlayerComponent();

	switch (collisionPair) 
	{
	case(Component::CollisionPair::CP_VEHICLE_PROJECTILE):
		shooter = (PlayerComponent*)Game::Find(ownerName)->GetComponent(playerRef);
		shooter->playerScore += 10.0;
		break;
	}

	Finalize();
}