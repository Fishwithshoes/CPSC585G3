#include "MachineGunComponent.h"
#include "Game.h"
#include "GeoGenerator.h"
#include "BulletComponent.h"

void MachineGunComponent::Start()
{
	Initialize();

	bulletMesh = GeoGenerator::MakeSphere(0.1, 4, 8, false);

	Finalize();
}
void MachineGunComponent::Update()
{
	Initialize();

	GameObject* owner = Game::Find("Player1");

	if (Input::GetXBoxButton(1, ButtonCode::XBOX_B))
	{
		if (nextBullet <= 0.0f)
		{
			//Spawn bullet
			cout << "Fire!" << endl;
			GameObject temp = GameObject("Bullet", Tags::TAGS_PROJECTILE);
			temp.mesh = bulletMesh;
			temp.transform = transform;
			GameObject* bullet = Game::CreateWorldObject(temp);
			bullet->AddComponent(new BulletComponent());

			nextBullet = bulletDelay;
		}
	}

	nextBullet -= Time::getDeltaTime();
	nextBullet = Mathf::Clamp(nextBullet, 0.0f, bulletDelay);

	Finalize();
}