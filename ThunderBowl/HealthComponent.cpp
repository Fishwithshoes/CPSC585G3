#include "HealthComponent.h"
#include "Game.h"
#include "Physics.h"
#include "BulletComponent.h"
#include "Audio.h"

void HealthComponent::Start()
{
	Initialize();
	currentHealth = 100.0;
	respawnTime = 20.00;
	standardMat.diffuseColor = glm::vec3(0.0, 1.0, 0.0);
	Finalize();
}

void HealthComponent::Update()
{
	Initialize();
	/*vector<GameObject*> currBullets = Game::FindGameObjectsWithTag(Tags::TAGS_PROJECTILE);
	for (int i = 0; i < currBullets.size(); i++) 
	{
		if (glm::distance(currBullets[i]->transform.position, transform.position) < 1.75) 
		{
			//currentHealth -= 25.0;
			Audio::Play2DSound(SFX_Hit, Random::rangef(0.20, 0.50), 0.0);

			BulletComponent* bullet = &BulletComponent();
			bullet = (BulletComponent*)Game::Find(currBullets[i]->name)->GetComponent(bullet);

			Physics::getGScene()->removeActor(*bullet->bullet);
			Game::DestroyWorldObjectAt(currBullets[i]->objectID);
		}
	}*/

	standardMat.diffuseColor = glm::vec3(0.0, currentHealth/100.0, 0.0);

	if (currentHealth <= 0.0)
	{
		EnemyComponent* enemy = &EnemyComponent();
		enemy = (EnemyComponent*)Game::Find(selfName)->GetComponent(enemy);
		enemy->enPhysVehicle->setGlobalPose(physx::PxTransform(physx::PxVec3(0.0, 1000.0, 0.0), physx::PxIdentity));
		respawnTime -= Time::getDeltaTime();
		if (respawnTime <= 0.0) {


		enemy->enPhysVehicle->setGlobalPose(physx::PxTransform(enemy->startPosition, physx::PxIdentity));
		enemy->enPhysVehicle->setAngularVelocity(physx::PxVec3(0, 0, 0));
		enemy->enPhysVehicle->setLinearVelocity(physx::PxVec3(0, 0, 0));

		currentHealth = 100;
		respawnTime = 20.0;
		}
	}
	Finalize();
}

bool HealthComponent::isDead() {
	if (currentHealth <= 0.0) {
		return true;
	}
	return false;
}
