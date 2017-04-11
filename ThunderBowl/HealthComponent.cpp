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
	newSpawn = true;
	newSpawnTimer = 100.0;
	//standardMat.diffuseColor = glm::vec3(0.0, 1.0, 0.0);

	ParticleSystem ps = ParticleSystem();
	ps.name = selfName + "DamagedSmoke";
	ps.initialSpeed.min = 0.6;
	ps.initialSpeed.max = 0.8;
	ps.gravityScale = -5.0;
	ps.initialColor.alpha = vec4(vec3(0.5), 1);
	ps.initialColor.bravo = vec4(vec3(0.7), 1);
	ps.initialRadius.min = 1.4;
	ps.initialRadius.max = 1.8;
	ps.lifeSpan.min = 1.4;
	ps.lifeSpan.max = 1.8;
	ps.spawnPointVariance = vec3(0.5);
	ps.monochromatic = true;
	ps.mainTexture = MAP_SMOKE_PART;
	ps.spawnRate = 0.0;
	ps.destroySystemWhenEmpty = false;
	ps.useSystemLifespan = false;
	Game::CreateParticleObject(ps);
	damagedSmokeName = ps.name;
	//cout << damagedSmokeName << " created!" << endl;

	Finalize();
}

void HealthComponent::Update()
{
	Initialize();
	if (newSpawn) {
		if (selfName == "Player0") {
			cout << selfName << " new spawn" << endl;
			cout << newSpawnTimer << endl;
		}
		newSpawnTimer -= Time::getDeltaTime();
		if (newSpawnTimer <= 0.0) {
			newSpawn = false;
			newSpawnTimer = 100.0;
		}
	}

	GameObject* self = Game::Find(selfName);

	if (currentHealth <= 66.67 && damageModel == 0)
	{
		self->staticGeo = SG_CAR_MED;
		damageModel = 1;
		Audio::Play2DSound(SFX_Dent, 0.4, 0.0);
	}

	if (currentHealth <= 33.33 && damageModel == 1)
	{
		self->staticGeo = SG_CAR_LOW;
		damageModel = 2;
		Audio::Play2DSound(SFX_Dent, 0.4, 0.0);
	}

	if (currentHealth <= 0.0 && !shouldRespawn)
	{
		PlayerComponent* player = &PlayerComponent();
		player = (PlayerComponent*)self->GetComponent(player);
		if (player->lastDamaging != player)
			player->lastDamaging->playerScore += 100;

		self->isVisible = false;
		if (self->tag == TAGS_AI_PLAYER)
		{
			EnemyComponent* enemy = &EnemyComponent();
			enemy = (EnemyComponent*)self->GetComponent(enemy);

			enemy->enPhysVehicle->setGlobalPose(physx::PxTransform(physx::PxVec3(0, 1000, 0), physx::PxIdentity));

			for (int i = 0; i < enemy->enWheelVector.size(); i++)
				enemy->enWheelVector[i]->isVisible = false;
		}
		else
		{
			VehicleComponent* human = &VehicleComponent();
			human = (VehicleComponent*)self->GetComponent(human);

			human->physVehicle->setGlobalPose(physx::PxTransform(physx::PxVec3(0, 1000, 0), physx::PxIdentity));

			for (int i = 0; i < human->wheelVector.size(); i++)
				human->wheelVector[i]->isVisible = false;
		}

		Audio::Play2DSound(SFX_ExplodeCar, 1.0, 0.0);

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

		respawnTime = 1.5;
		shouldRespawn = true;
	}

	respawnTime -= Time::getDeltaTime();
	if (respawnTime <= 0.0 && shouldRespawn)
	{
		//transform.position = glm::vec3(0.0, -1005.0, 0.0);
		self->isVisible = true;
		self->staticGeo = SG_CAR;
		damageModel = 0;
		newSpawn = true;

		PlayerComponent* player = &PlayerComponent();
		player = (PlayerComponent*)self->GetComponent(player);
		player->lastDamaging = player;

		player->machineGunAmmo = 100;// GameManager::START_MG_AMMO;
		player->missileLauncherAmmo = 1;// GameManager::START_MISSILE_AMMO;
		player->flamethrowerAmmo = 5.0; // GameManager::START_FLAMETHROWER_AMMO;

		if (self->tag == TAGS_AI_PLAYER)
		{
			EnemyComponent* enemy = &EnemyComponent();
			enemy = (EnemyComponent*)self->GetComponent(enemy);

			AIControlComponent1* enemyController = &AIControlComponent1();
			enemyController = (AIControlComponent1*)self->GetComponent(enemyController);

			enemy->enPhysVehicle->setGlobalPose(physx::PxTransform(enemy->startPosition, enemy->startRotation));
			enemy->enPhysVehicle->setAngularVelocity(physx::PxVec3(0, 0, 0));
			enemy->enPhysVehicle->setLinearVelocity(physx::PxVec3(0, 0, 0));
			enemyController->resetCurrent();

			for (int i = 0; i < enemy->enWheelVector.size(); i++)
				enemy->enWheelVector[i]->isVisible = true;
		}
		else
		{
			VehicleComponent* human = &VehicleComponent();
			human = (VehicleComponent*)self->GetComponent(human);

			human->physVehicle->setGlobalPose(physx::PxTransform(human->myStartPosition, physx::PxQuat(0, 0, 0, 1)));
			human->physVehicle->setAngularVelocity(physx::PxVec3(0, 0, 0));
			human->physVehicle->setLinearVelocity(physx::PxVec3(0, 0, 0));

			for (int i = 0; i < human->wheelVector.size(); i++)
				human->wheelVector[i]->isVisible = true;
		}

		currentHealth = 100;
		shouldRespawn = false;
	}

	UpdateParticles();

	Finalize();
}

void HealthComponent::UpdateParticles()
{
	Transform t = transform;
	t.rotation = t.GetInverseRotation();
	ParticleSystem* damagedSmoke = (ParticleSystem*)Game::Find(damagedSmokeName);
	damagedSmoke->transform = t;
	damagedSmoke->spawnRate = currentHealth > 0.0 ? 10.0 - currentHealth*0.1 : 0.0;
	damagedSmoke->initialColor.alpha = vec4(vec3(0.5 - currentHealth*0.003), 1.0);
	damagedSmoke->initialColor.bravo = vec4(vec3(0.7 - currentHealth*0.003), 1.0);
	damagedSmoke->initialRadius.min = 1.4 - currentHealth*0.014;
	damagedSmoke->initialRadius.max = 1.8 - currentHealth*0.018;
}

bool HealthComponent::isDead() 
{
	if (currentHealth <= 0.0)
		return true;
	else
		return false;
}

bool HealthComponent::isNewSpawn()
{
	return newSpawn;
}
