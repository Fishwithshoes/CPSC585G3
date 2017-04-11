#include "MachineGunComponent.h"
#include "Audio.h"
#include "Game.h"
#include "GeoGenerator.h"
#include "BulletComponent.h"
#include "Renderer.h"

void MachineGunComponent::Start()
{
	Initialize();

	//Find it now in Renderer::LoadStaticGeo(args...)
	bulletMesh = GeoGenerator::MakeSphere(0.20, 4, 8, false);

	Finalize();
}
int foobar = 0;
void MachineGunComponent::Update()
{
	Initialize();

	GameObject* owner = Game::Find(selfName);
	//cout << "selfname: " << selfName << endl;

	nextBullet -= Time::getDeltaTime();
	nextBullet = Mathf::Clamp(nextBullet, 0.0f, bulletDelay);

	//GameObject* ammoMeter = Game::Find("AmmoMeter");
	//ammoMeter->transform.scale.x = (float)ammoCount / maxAmmo;

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

void MachineGunComponent::FireMG() 
{
	int ammo = 0;
	GameObject* self = Game::Find(selfName);
	//if (self->tag == TAGS_HUMAN_PLAYER)
	//{
		PlayerComponent* player = &PlayerComponent();
		player = (PlayerComponent*)self->GetComponent(player);
		ammo = player->machineGunAmmo;
	/*}
	else {
		PlayerComponent* enemy = &PlayerComponent();
		enemy = (PlayerComponent*)self->GetComponent(enemy);
		ammo = enemy->machineGunAmmo;
	}*/

	HealthComponent* health = &HealthComponent();
	health = (HealthComponent*)self->GetComponent(health);

	if (nextBullet <= 0.0f && ammo > 0 && health->currentHealth > 0.0)
	{
		//Spawn bullet
		//cout << "Fire!" << endl;
		currentBullet++;
		ammo -= 1;
		PlayerComponent* player = &PlayerComponent();
		player = (PlayerComponent*)self->GetComponent(player);
		player->machineGunAmmo--;
		if (player->machineGunAmmo < 0)
			player->machineGunAmmo = 0;

		GameObject temp = GameObject(selfName + "Bullet" + to_string(currentBullet), Tags::TAGS_PROJECTILE);
		//temp.mesh = bulletMesh;
		temp.staticGeo = SG_MG_BULLET;
		temp.transform = transform;
		temp.transform.rotation = temp.transform.GetInverseRotation();
		if (self->tag == TAGS_HUMAN_PLAYER)
		{
			VehicleComponent* vehicle = &VehicleComponent();
			vehicle = (VehicleComponent*)self->GetComponent(vehicle);

			float vertical = Input::GetXBoxAxis(vehicle->GetPlayerNum(), ButtonCode::XBOX_JOY_RIGHT_VERTICAL);
			float horizontal = Input::GetXBoxAxis(vehicle->GetPlayerNum(), ButtonCode::XBOX_JOY_RIGHT_HORIZONTAL);

			float theta = -Mathf::PI * 0.15 * horizontal;
			float phi = Mathf::PI * 0.1 * vertical;

			temp.transform.Rotate(temp.transform.GetUp(), theta, false);
			temp.transform.Rotate(temp.transform.GetRight(), phi, false);
		}
		else {

			AIControlComponent1* enemyController = &AIControlComponent1();
			enemyController = (AIControlComponent1*)self->GetComponent(enemyController);

			vec3 firingVector = enemyController->getFiringVector();
			vec3 tempVector = normalize(firingVector*vec3(1.0, 0.0, 1.0));
			glm::normalize(tempVector);
			Transform t = transform;
			//t.z = -transform.z;
			t.rotation = t.GetInverseRotation();
			float theta = acos(glm::dot(glm::normalize(t.GetForward()*vec3(1.0,0.0,1.0)), tempVector));
			if (dot(t.GetRight(), tempVector) > 0.0)//rotate right
				theta = -theta;
			//cout << "theta: " << theta << endl;

			tempVector = firingVector*vec3(0.0, 1.0, 0.0);
			glm::normalize(tempVector);
			//float phi = acos(glm::dot(transform.GetUp(), firingVector))*0.3f * firingVector.y < 0.0 ? -1.0 : 1.0;
			//cout << "phi: " << phi << endl;


			temp.transform.Rotate(temp.transform.GetUp(), theta, false);
			//temp.transform.Rotate(temp.transform.GetRight(), phi, false);
		}
		temp.transform.Translate(temp.transform.GetUp() * 0.8f, false);
		temp.transform.Translate(temp.transform.GetForward() * 7.5f, false);
		temp.standardMat.roughness = 1.0;
		temp.standardMat.metalness = 0.0;
		temp.standardMat.diffuseLevel = 0.0;
		temp.standardMat.selfIllumLevel = 2.0;
		temp.standardMat.selfIllumColor = vec3(1.0, 0.5, 0);

		temp.standardMat.isMetallic = true;
		GameObject* bullet = Game::CreateStaticObject(temp);
		bullet->AddComponent(new BulletComponent(selfName));
		BulletComponent* bulletComp = &BulletComponent("");
		bulletComp = (BulletComponent*)bullet->GetComponent(bulletComp);
		bulletComp->ownerName = selfName;
		//BulletComponent* bulletTempRef = &BulletComponent();
		//bulletTempRef = (BulletComponent*)Game::Find(selfName + "Bullet" + to_string(currentBullet))->GetComponent(bulletTempRef);
		//bulletTempRef->ownerName = selfName;

		//IF_DEF MUZZLE FLASH
		ParticleSystem ps = ParticleSystem();
		ps.transform = transform;
		ps.transform.rotation = ps.transform.GetInverseRotation();
		ps.transform.Translate(ps.transform.GetUp()*0.8f, false);
		ps.transform.Translate(ps.transform.GetForward()*7.0f, false);
		ps.initialRadius.min = 0.4;
		ps.initialRadius.max = 0.7;
		ps.initialColor.alpha = vec4(vec3(1), 1.0);
		ps.initialColor.bravo = vec4(vec3(1), 1.0);
		ps.mainTexture = MAP_FLASH_PART;
		ps.initialSpeed.min = 0;
		ps.initialSpeed.max = 0;
		ps.gravityScale = 0.0;
		ps.lifeSpan.min = 0.04;
		ps.lifeSpan.max = 0.04;
		ps.spawnRate = 0.0;
		ps.destroySystemWhenEmpty = true;
		ParticleSystem* flashPtr = Game::CreateParticleObject(ps);
		flashPtr->AddParticleBurst(1, 0.0);
		PointLight light;
		light.Color = vec4(1.0, 0.7, 0.1, 10.0);
		light.Pos = vec4(ps.transform.position, 8.0);
		Renderer::AddPointLight(light);
		//END_IF MUZZLE FLASH

		Audio::Play2DSound(SFX_MG, Random::rangef(0.30, 0.45), 0.0);
		nextBullet = bulletDelay;
	}
}