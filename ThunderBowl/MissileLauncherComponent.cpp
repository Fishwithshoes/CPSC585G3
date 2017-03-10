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

	Finalize();
}
void MissileLauncherComponent::Update()
{
	Initialize();

	GameObject* self = Game::Find(selfName);

	nextLaunch -= Time::getDeltaTime();
	nextLaunch = Mathf::Clamp(nextLaunch, 0.0, launchDelay);

	if (nextLaunch <= 0)
	{
		if (self->tag == TAGS_HUMAN_PLAYER)
		{
			if (Input::GetXBoxButton(1, ButtonCode::XBOX_X))
			{
				VehicleComponent* vehicle = &VehicleComponent();
				vehicle = (VehicleComponent*)self->GetComponent(vehicle);
				FireMissile(vehicle->physVehicle->getLinearVelocity().magnitude());
			}
		}
		else
		{
			//AI can fire the missile launcher here
		}
	}

	Finalize();
}

void MissileLauncherComponent::FireMissile(float ownerVelocity)
{
	GameObject missile = GameObject("Missile" + selfName + to_string(currentMissile), TAGS_MISSILE);
	vec3 size = vec3(1.8, 1.8, 5.0);
	missile.mesh = GeoGenerator::MakeBox(size.x, size.y, size.z, false);
	missile.standardMat.diffuseColor = vec3(1.0, 0.5, 0.0);
	missile.transform = transform;
	missile.transform.position.y += 3.5;
	GameObject* ptr = Game::CreateWorldObject(missile);
	ptr->AddComponent(new MissileComponent(size, ownerVelocity, selfName));

	nextLaunch = launchDelay;
	currentMissile++;
	//Audio::Play2DSound(SFX_FireMissile, 0.2, 0);
}
