#include "MachineGunComponent.h"
#include "Audio.h"
#include "Game.h"
#include "GeoGenerator.h"
#include "BulletComponent.h"

void MachineGunComponent::Start()
{
	Initialize();

	bulletMesh = GeoGenerator::MakeSphere(0.15, 4, 8, false);

	Finalize();
}
int foobar = 0;
void MachineGunComponent::Update()
{
	Initialize();

	GameObject* owner = Game::Find(selfName);

	nextBullet -= Time::getDeltaTime();
	nextBullet = Mathf::Clamp(nextBullet, 0.0f, bulletDelay);

	GameObject* ammoMeter = Game::Find("AmmoMeter");
	ammoMeter->transform.scale.x = (float)ammoCount / maxAmmo;

	//IF_DEF BULLET DESTRUCTION DEBUG
	//if (Time::getElapsedFrames() % 2 == 0)
	//{
	//	if (Input::GetXBoxButton(1, ButtonCode::XBOX_X) && selfName == "Player1")
	//	{
	//		//Spawn Billet
	//		GameObject billet = GameObject("Billet", TAGS_DEBUG_0);
	//		billet.mesh = GeoGenerator::MakeSphere(1, 4, 8, false);
	//		billet.standardMat.diffuseMap = MAP_ZERO + foobar;
	//		billet.standardMat.tileUV = vec2(5,5);
	//		billet.transform = transform;
	//		billet.transform.Translate(vec3(Random::rangef(-1, 1), 2.0, Random::rangef(-1, 1)), false);
	//		GameObject* myPtr = Game::CreateWorldObject(billet);
	//		myPtr->AddComponent(new FloatComponent());
	//
	//		foobar++;
	//		if (foobar > 9)
	//			foobar = 0;
	//	}
	//	vector<GameObject*> billets = Game::FindGameObjectsWithTag(TAGS_DEBUG_0);
	//
	//	if (billets.size() > 100)
	//	{
	//		Game::DestroyWorldObjectAt(billets[0]->objectID);
	//	}
	//}
	//END_IF BULLET DESTRUCTION DEBUG

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
		temp.transform.Translate(vec3(0,-0.5,0), false);
		temp.standardMat.roughness = 0.3;
		temp.standardMat.metalness = 1.0;
		temp.standardMat.isMetallic = true;
		GameObject* bullet = Game::CreateWorldObject(temp);
		bullet->AddComponent(new BulletComponent());
		BulletComponent* bulletTempRef = &BulletComponent();
		bulletTempRef = (BulletComponent*)Game::Find(selfName + "Bullet" + to_string(currentBullet))->GetComponent(bulletTempRef);
		bulletTempRef->ownerName = selfName;

		Audio::Play2DSound(SFX_MG, Random::rangef(0.20, 0.40), 0.0);
		nextBullet = bulletDelay;
	}
}