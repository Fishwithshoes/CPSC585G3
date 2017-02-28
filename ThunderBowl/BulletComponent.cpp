#include "BulletComponent.h"
#include "Physics.h"
#include "Game.h"
#include "GameObject.h"

GameObject* selfGameObject;
physx::PxPhysics* worldPhys;
physx::PxCooking* worldCook;
physx::PxScene* worldScene;

void BulletComponent::Start()
{

	Initialize();

	selfGameObject = Game::Find(selfName);
	speed = 50.0f;
	lifeSpan = 1.00f;
	worldPhys = Physics::getGPhysics();
	worldCook = Physics::getGCooking();
	worldScene = Physics::getGScene();

	VehicleComponent* temp = &VehicleComponent();
	VehicleComponent* thisVeh = (VehicleComponent*) Game::Find("Player1")->GetComponent(temp);
	standardMat.diffuseColor = glm::vec3(1.0, 0.0, 0.0);


	bullet = Physics::createTestProjectile();
	bullet->userData = this;

	physx::PxVec3 position;
	position.x = transform.position.x;
	position.y = transform.position.y + 2.0;
	position.z = transform.position.z;
	bullet->setGlobalPose(physx::PxTransform(position));

	transform.rotation = transform.GetInverseRotation();

	vec3 newForward = transform.GetForward();

	physx::PxVec3 forward;
	forward.x = newForward.x;
	forward.y = newForward.y;
	forward.z = newForward.z;

	bullet->setLinearVelocity(forward*(speed + thisVeh->physVehicle->getLinearVelocity().magnitude()));
	cout << speed + thisVeh->physVehicle->getLinearVelocity().magnitude() << endl;

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
		cout << selfGameObject->objectID << " died" << endl;
		//worldScene->removeActor(*bullet);
		bullet->release();
		Game::DestroyWorldObjectAt(selfGameObject->objectID);
	}
}

void BulletComponent::OnCollision(Component::CollisionPair collisionPair) {
	cout << "Bullet Collision" << endl;
	/*bullet->release();
	selfGameObject = Game::Find(selfName);
	Game::DestroyWorldObjectAt(selfGameObject->objectID);*/
}