#include "BulletComponent.h"
#include "Physics.h"
#include "Game.h"

void BulletComponent::Start()
{

	Initialize();
	VehicleComponent* temp = &VehicleComponent();
	VehicleComponent* thisVeh = (VehicleComponent*) Game::Find("Player1")->GetComponent(temp);
	standardMat.diffuseColor = glm::vec3(1.0, 0.0, 0.0);
	transform.rendertype = RenderTypes::RT_QUAT;
	physx::PxPhysics* worldPhys = Physics::getGPhysics();
	physx::PxCooking* worldCook = Physics::getGCooking();
	physx::PxScene* worldScene = Physics::getGScene();

	bullet = Physics::createTestProjectile();
	bullet->userData = this;

	physx::PxVec3 position;
	position.x = transform.position.x;
	position.y = transform.position.y;
	position.z = transform.position.z + 2.0;
	bullet->setGlobalPose(physx::PxTransform(position));

	physx::PxVec3 forward;
	forward.x = transform.GetForward().x;
	forward.y = transform.GetForward().y;
	forward.z = transform.GetForward().z;
	bullet->setLinearVelocity(forward*(speed + thisVeh->physVehicle->getLinearVelocity().magnitude()));

	Finalize();
}

void BulletComponent::Update()
{
	Initialize();

	transform.position.x = bullet->getGlobalPose().p.x;
	transform.position.y = bullet->getGlobalPose().p.y;
	transform.position.z = bullet->getGlobalPose().p.z;

	lifeRemaining -= Time::getDeltaTime();

	Finalize();
	
	if (lifeRemaining <= 0.0) {			//Works for one bullet
		bullet->release();
		Game::DestroyWorldObjectAt(selfGameObjectID);
	}
}

void BulletComponent::OnCollision(Component::CollisionPair collisionPair) {
	cout << "Bullet Collision" << endl;
	//bullet->release();
	//Game::DestroyWorldObjectAt(selfGameObjectID);
}