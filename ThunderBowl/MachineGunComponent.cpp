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

	GameObject* owner = Game::Find(selfName);

	nextBullet -= Time::getDeltaTime();
	nextBullet = Mathf::Clamp(nextBullet, 0.0f, bulletDelay);

	GameObject* ammoMeter = Game::Find("AmmoMeter");
	ammoMeter->transform.scale.x = (float)ammoCount / maxAmmo;

	Finalize();
}

void MachineGunComponent::FireMG() {
	if (nextBullet <= 0.0f && ammoCount > 0)
	{
		//Spawn bullet
		cout << "Fire!" << endl;
		currentBullet++;
		ammoCount -= 1;
		GameObject temp = GameObject(selfName + "Bullet" + to_string(currentBullet), Tags::TAGS_PROJECTILE);
		temp.mesh = bulletMesh;
		temp.transform = transform;
		GameObject* bullet = Game::CreateWorldObject(temp);
		bullet->AddComponent(new BulletComponent());
		BulletComponent* bulletTempRef = &BulletComponent();
		bulletTempRef = (BulletComponent*)Game::Find(selfName + "Bullet" + to_string(currentBullet))->GetComponent(bulletTempRef);
		bulletTempRef->ownerName = selfName;

		nextBullet = bulletDelay;
	}
}