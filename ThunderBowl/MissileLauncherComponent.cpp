#include "MissileLauncherComponent.h"
#include "MissileComponent.h"
#include "Game.h"
#include "Audio.h"
#include "GeoGenerator.h"
#include "Physics.h"
#include "VehicleComponent.h"

void MissileLauncherComponent::Start()
{
	Initialize();

	cout << "I am a MissileLauncherComponent!" << endl;
	launchDelay = 3.0;

	Finalize();
}
void MissileLauncherComponent::Update()
{
	Initialize();

	GameObject* self = Game::Find(selfName);

	nextLaunch -= Time::getDeltaTime();
	nextLaunch = Mathf::Clamp(nextLaunch, 0.0, launchDelay);

	Finalize();
}

void MissileLauncherComponent::FireMissile()
{
	GameObject* self = Game::Find(selfName);
	float ownerVelocity = 0.0f;
	if (self->tag == TAGS_HUMAN_PLAYER)
	{
		VehicleComponent* vehicle = &VehicleComponent();
		vehicle = (VehicleComponent*)self->GetComponent(vehicle);
		ownerVelocity = vehicle->physVehicle->getLinearVelocity().magnitude();
	}
	else
	{
		EnemyComponent* vehicle = &EnemyComponent();
		vehicle = (EnemyComponent*)self->GetComponent(vehicle);
		ownerVelocity = vehicle->enPhysVehicle->getLinearVelocity().magnitude();
	}

	PlayerComponent* player = &PlayerComponent();
	player = (PlayerComponent*)self->GetComponent(player);
	int ammo = player->missileLauncherAmmo;

	if (nextLaunch <= 0 && ammo > 0)
	{
		GameObject missile = GameObject("Missile" + selfName + to_string(currentMissile), TAGS_MISSILE);
		vec3 size = vec3(1.8, 1.8, 5.0);
		missile.mesh = GeoGenerator::MakeBox(size.x, size.y, size.z, false);
		missile.standardMat.diffuseColor = vec3(1.0, 0.5, 0.0);
		missile.transform = transform;
		missile.transform.position.y += 3.5;
		GameObject* ptr = Game::CreateWorldObject(missile);
		ptr->AddComponent(new MissileComponent(size, ownerVelocity, selfName));
	
		PlayerComponent* player = &PlayerComponent();
		player = (PlayerComponent*)self->GetComponent(player);
		player->missileLauncherAmmo--;
		if (player->missileLauncherAmmo < 0)
			player->missileLauncherAmmo = 0;
	
		nextLaunch = launchDelay;
		currentMissile++;
		Audio::Play2DSound(SFX_FireMissile, 0.5, 0);
	}
}
