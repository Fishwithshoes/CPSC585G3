#include "MissileComponent.h"
#include "Game.h"
#include "Audio.h"
#include "Physics.h"

PlayerComponent* MissileShooter;

 MissileComponent::MissileComponent(vec3 size, float ownerVelocity, string ownerName)
{
	missileSize = size;
	inheritedVelocity = ownerVelocity;
	missileOwnerName = ownerName;
}

void MissileComponent::Start()
{
	Initialize();

	missile = Physics::CreateMissile(missileSize);
	missile->userData = this;

	//Set up initial pose
	physx::PxVec3 startPos = physx::PxVec3(transform.position.x, transform.position.y, transform.position.z);	
	transform.rotation = transform.GetInverseRotation();	
	physx::PxQuat startRot = physx::PxQuat(transform.rotation.x, transform.rotation.y, transform.rotation.z, transform.rotation.w);
	missile->setGlobalPose(physx::PxTransform(startPos, startRot));

	//Set initial velocity
	vec3 forward = transform.GetForward();
	physx::PxVec3 initialVelocity = physx::PxVec3(forward.x, forward.y, forward.z);
	initialVelocity *= speed+inheritedVelocity;
	missile->setLinearVelocity(initialVelocity);

	//Particle trail
	ParticleSystem ps = ParticleSystem();
	ps.name = selfName + "Trail";
	ps.initialSpeed.min = 0;
	ps.initialSpeed.max = 0;
	ps.initialColor.alpha = vec4(vec3(0.5), 1);
	ps.initialColor.bravo = vec4(vec3(0.7), 1);
	ps.initialRadius.min = 0.7;
	ps.initialRadius.max = 1.3;
	ps.lifeSpan.min = 1.3;
	ps.lifeSpan.max = 1.8;
	ps.spawnPointVariance = vec3(0.5);
	ps.monochromatic = true;
	ps.mainTexture = MAP_SMOKE_PART;
	ps.spawnRate = 20;
	ps.destroySystemWhenEmpty = true;
	particleTrail = Game::CreateParticleObject(ps);
	particleTrailName = particleTrail->name;

	MoveTransform();

	Finalize();
}
void MissileComponent::Update()
{
	Initialize();

	MoveTransform();

	Finalize();

	lifeRemaining -= Time::getDeltaTime();
	if (lifeRemaining <= 0) {
		Explode();

		Physics::getGScene()->removeActor(*missile);
		missile->release();
		ParticleSystem* temp = (ParticleSystem*)Game::Find(particleTrailName);
		temp->spawnRate = 0;
		Game::DestroyWorldObjectAt(selfID);
	}
}

void MissileComponent::MoveTransform()
{
	//Match position to transform
	transform.position.x = missile->getGlobalPose().p.x;
	transform.position.y = missile->getGlobalPose().p.y;
	transform.position.z = missile->getGlobalPose().p.z;

	//Match rotation to transform
	transform.rotation.x = missile->getGlobalPose().q.x;
	transform.rotation.y = missile->getGlobalPose().q.y;
	transform.rotation.z = missile->getGlobalPose().q.z;
	transform.rotation.w = missile->getGlobalPose().q.w;

	transform.rotation = transform.GetInverseRotation();

	particleTrail->transform = transform;
}

void MissileComponent::OnCollision(CollisionPair collisionPair, Component* collider)
{
	Initialize();

	switch (collisionPair)
	{
	case CollisionPair::CP_VEHICLE_MISSILE:
		cout << "Missile collided with a vehicle" << endl;
		DirectHit(collider);
		break;
	default:
		cout << "Missile collided with an unknown object!" << endl;
		break;
	}
	Finalize();
}

void MissileComponent::DirectHit(Component* collider) {
	PlayerComponent* playerRef = &PlayerComponent();

	GameObject* hitDirectly = Game::Find(collider->getName());
	HealthComponent* health = &HealthComponent();
	health = (HealthComponent*)hitDirectly->GetComponent(health);
	health->currentHealth = 0.0;
	lifeRemaining = 0.0;
	MissileShooter = (PlayerComponent*)Game::Find(missileOwnerName)->GetComponent(playerRef);
	MissileShooter->playerScore += 100.00;
}

void MissileComponent::Explode()
{
	vector<GameObject*> players = Game::FindGameObjectsWithTag(TAGS_AI_PLAYER);
	vector<GameObject*> playeri = Game::FindGameObjectsWithTag(TAGS_HUMAN_PLAYER);

	GameObject* owner = Game::Find(missileOwnerName);
	PlayerComponent* ownerPlayer = &PlayerComponent();
	ownerPlayer = (PlayerComponent*)owner->GetComponent(ownerPlayer);

	players.insert(players.end(), playeri.begin(), playeri.end());
	for (int i = 0; i < players.size(); i++)
	{
		if (players[i]->name != missileOwnerName)
		{
			float distance = glm::distance(transform.position, players[i]->transform.position);
			if (distance < 50)
			{
				vec3 missileToPlayer = players[i]->transform.position - transform.position;
				missileToPlayer.y += distance * 2;//Go way below to launch in the air!
				missileToPlayer = normalize(missileToPlayer);
				physx::PxVec3 explosionForce;
				explosionForce = physx::PxVec3(missileToPlayer.x, missileToPlayer.y, missileToPlayer.z);
				explosionForce = physx::PxVec3(0, 1, 0);
				explosionForce *= 40000 - distance * 100;

				//Do damage to Health Component
				HealthComponent* victimHealth = &HealthComponent();
				victimHealth = (HealthComponent*)players[i]->GetComponent(victimHealth);
				float damage = 100 - distance * 2;
				victimHealth->currentHealth -= damage;

				if (victimHealth->currentHealth <= 0.0)//Points for making a kill
				{
					ownerPlayer->playerScore += 100;
				}
				else//Points for damage
				{
					ownerPlayer->playerScore += damage*0.5;
				}

				//Add score to Player Component
				PlayerComponent* playerRef = &PlayerComponent();
				MissileShooter = (PlayerComponent*)Game::Find(missileOwnerName)->GetComponent(playerRef);
				MissileShooter->playerScore += 100 - distance * 2;;

				//Add force at rigid body position
				if (players[i]->tag == TAGS_AI_PLAYER)
				{
					EnemyComponent* enemy = &EnemyComponent();
					enemy = (EnemyComponent*)players[i]->GetComponent(enemy);
					enemy->enPhysVehicle->addForce(explosionForce, physx::PxForceMode::eIMPULSE);		//BOTH THROW NPWRITECHECK WARNING
					enemy->enPhysVehicle->addTorque(explosionForce, physx::PxForceMode::eIMPULSE);

				}
				else
				{
					VehicleComponent* vehicle = &VehicleComponent();
					vehicle = (VehicleComponent*)players[i]->GetComponent(vehicle);
					vehicle->physVehicle->addForce(explosionForce, physx::PxForceMode::eIMPULSE);
					vehicle->physVehicle->addTorque(explosionForce, physx::PxForceMode::eIMPULSE);
				}
			}
		}
	}

	
	Audio::Play2DSound(SFX_ExplodeMissile, Random::rangef(0.1,0.2), 0);

	ParticleSystem ps = ParticleSystem();
	ps.initialSpeed.min = 115;
	ps.initialSpeed.max = 118;
	ps.accelerationScale = 0.88;
	ps.initialColor.alpha = vec4(vec3(1.0), 1);
	ps.initialColor.bravo = vec4(vec3(0.7), 1);
	ps.initialRadius.min = 3.3;
	ps.initialRadius.max = 3.9;
	ps.lifeSpan.min = 1.3;
	ps.lifeSpan.max = 1.8;
	ps.spawnPointVariance = vec3(0.5);
	ps.monochromatic = true;
	ps.mainTexture = MAP_FLAME13_PART;
	ps.textures = {MAP_FLAME09_PART, MAP_FLAME13_PART, MAP_FLAME13_PART, MAP_SMOKE_PART};
	ps.spawnRate = 0;
	ps.destroySystemWhenEmpty = true;
	ps.gravityScale = -15;
	ps.transform = transform;
	ps.initialFogLevel.min = 0;
	ps.initialFogLevel.max = 0;
	ParticleSystem* ptr = Game::CreateParticleObject(ps);
	ptr->AddParticleBurst(30, 0);

	lifeRemaining = 0.0;
}
