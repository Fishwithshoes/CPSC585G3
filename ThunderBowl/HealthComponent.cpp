#include "HealthComponent.h"
#include "Game.h"
#include "GameManager.h"
#include "Physics.h"
#include "BulletComponent.h"
#include "Audio.h"

void HealthComponent::Start()
{
	Initialize();
	//currentHealth = GameManager::MAX_HEALTH;
	//respawnTime = GameManager::RESPAWN_TIME;
	currentHealth = 100.0;
	respawnTime = 20.0;
	standardMat.diffuseColor = glm::vec3(0.0, 1.0, 0.0);
	Finalize();
}

void HealthComponent::Update()
{
	Initialize();

	//vector<GameObject*> currBullets = Game::FindGameObjectsWithTag(Tags::TAGS_PROJECTILE);
	//for (int i = 0; i < currBullets.size(); i++) 
	//{
	//	if (glm::distance(currBullets[i]->transform.position, transform.position) < 1.75) 
	//	{
	//		currentHealth -= 25.0;
	//		Audio::Play2DSound(SFX_Hit, Random::rangef(0.20, 0.50), 0.0);
	//
	//		BulletComponent* bullet = &BulletComponent();
	//		bullet = (BulletComponent*)Game::Find(currBullets[i]->name)->GetComponent(bullet);
	//
	//		Physics::getGScene()->removeActor(*bullet->bullet);
	//		Game::DestroyWorldObjectAt(currBullets[i]->objectID);
	//	}
	//}
	//

	standardMat.diffuseColor = glm::vec3(0.0, currentHealth/100.0, 0.0);

	if (currentHealth <= 0.0)
	{
		//transform.position = glm::vec3(0.0, -1005.0, 0.0);
		GameObject* self = Game::Find(selfName);
		if (self->tag == TAGS_AI_PLAYER)
		{
			EnemyComponent* enemy = &EnemyComponent();
			enemy = (EnemyComponent*)self->GetComponent(enemy);

			enemy->enPhysVehicle->setGlobalPose(physx::PxTransform(physx::PxVec3(0, 10, 0), physx::PxQuat(0, 0, 0, 1)));
			enemy->enPhysVehicle->setAngularVelocity(physx::PxVec3(0, 0, 0));
			enemy->enPhysVehicle->setLinearVelocity(physx::PxVec3(0, 0, 0));
		}
		else
		{
			VehicleComponent* human = &VehicleComponent();
			human = (VehicleComponent*)self->GetComponent(human);

			human->physVehicle->setGlobalPose(physx::PxTransform(physx::PxVec3(0, 10, 0), physx::PxQuat(0, 0, 0, 1)));
			human->physVehicle->setAngularVelocity(physx::PxVec3(0, 0, 0));
			human->physVehicle->setLinearVelocity(physx::PxVec3(0, 0, 0));
		}
	
		currentHealth = 100;

		Audio::Play2DSound(SFX_ExplodeMissile, 1.0, 0.0);

		//IF_DEF EXPLOSION PARTICLES
		ParticleSystem ps = ParticleSystem();
		ps.name = selfName + "Explode";
		ps.initialSpeed.min = 45;
		ps.initialSpeed.max = 48;
		ps.accelerationScale = 0.88;
		ps.initialColor.alpha = vec4(vec3(1.0), 1);
		ps.initialColor.bravo = vec4(vec3(0.7), 1);
		ps.initialRadius.min = 2.3;
		ps.initialRadius.max = 2.9;
		ps.lifeSpan.min = 1.3;
		ps.lifeSpan.max = 1.8;
		ps.spawnPointVariance = vec3(0.5);
		ps.monochromatic = true;
		ps.mainTexture = MAP_FLAME13_PART;
		ps.textures = { MAP_FLAME09_PART, MAP_FLAME13_PART, MAP_FLAME13_PART, MAP_SMOKE_PART };
		ps.spawnRate = 0;
		ps.destroySystemWhenEmpty = true;
		ps.gravityScale = -15;
		ps.transform = transform;
		ps.initialFogLevel.min = 0;
		ps.initialFogLevel.max = 0;
		ParticleSystem* ptr = Game::CreateParticleObject(ps);
		ptr->AddParticleBurst(10, 0);
		//END_IF EXPLOSION PARTICLES

		respawnTime = 20.0;
	}
	Finalize();
}

bool HealthComponent::isDead() 
{
	if (currentHealth <= 0.0)
		return true;
	else
		return false;
}
