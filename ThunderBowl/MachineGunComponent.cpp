#include "MachineGunComponent.h"
#include "Game.h"
#include "GeoGenerator.h"
#include "BulletComponent.h"

void MachineGunComponent::Start()
{
	Initialize();

	bulletMesh = GeoGenerator::MakeSphere(0.1, 4, 8, false);

	//owner = Game::Find("Player1");

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
			GameObject* bullet = Game::CreateWorldObject(GameObject(bulletMesh, "Bullet", Tags::TAGS_PROJECTILE));
			bullet->transform = owner->transform;
			bullet->AddComponent(new BulletComponent());

			nextBullet = bulletDelay;
		}
	}

	nextBullet -= Time::getDeltaTime();
	nextBullet = Mathf::Clamp(nextBullet, 0.0f, bulletDelay);

	Finalize();
}