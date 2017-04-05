#include "PlayerComponent.h"
#include "VehicleComponent.h"
#include "Game.h"
#include "GameManager.h"
#include "Audio.h"
#include "Renderer.h"

//VehicleComponent* playerVeh;
void PlayerComponent::Start() 
{
	Initialize();

	playerScore = 0;
	oldScore = playerScore;

	machineGunAmmo = 50;// GameManager::START_MG_AMMO;
	missileLauncherAmmo = 4;// GameManager::START_MISSILE_AMMO;
	flamethrowerAmmo = 10.0; // GameManager::START_FLAMETHROWER_AMMO;

	StartParticles();

	Finalize();
}

void PlayerComponent::StartParticles()
{
	
}

void PlayerComponent::Update() 
{
	Initialize();

	if (oldScore != playerScore) 
	{
		PlayerStateToConsole();
		oldScore = playerScore;
	}

	//SBurnCheck();

	//function to resolve damage to player

	//function to resolve score for:
		//damage to enemy
		//destroying enemy
		//driving without taking damage
		//driving at high speed

	GameObject* self = Game::Find(selfName);
	HealthComponent* playerHealth = &HealthComponent();
	playerHealth = (HealthComponent*)self->GetComponent(playerHealth);

	//Switch Weapon - Human Players
	if (self->tag == TAGS_HUMAN_PLAYER)
	{
		VehicleComponent* vehicle = &VehicleComponent();
		vehicle = (VehicleComponent*)self->GetComponent(vehicle);
		GameObject* weaponIcon = Game::Find("WeaponIcon" + vehicle->GetPlayerNum());
		if (Input::GetXBoxButton(vehicle->GetPlayerNum(), ButtonCode::XBOX_Y) && !switchWeaponPrev)
		{
			switch (currentWeapon)
			{
			case GW_MACHINE_GUN:
				currentWeapon = GW_MISSILE_LAUNCHER;
				weaponIcon->particleOverlayMat.mainTexture = MAP_MISSILE_LAUNCHER_ICON;
				break;
			case GW_MISSILE_LAUNCHER:
				currentWeapon = GW_FLAMETHROWER;
				weaponIcon->particleOverlayMat.mainTexture = MAP_FLAMETHROWER_ICON;
				//weaponIcon->particleOverlayMat.color.w = 1.0;
				break;
			case GW_FLAMETHROWER:
				currentWeapon = GW_MACHINE_GUN;
				weaponIcon->particleOverlayMat.mainTexture = MAP_MACHINE_GUN_ICON;
				//weaponIcon->particleOverlayMat.color.w = 1.0;
				break;
			default:
				cout << "ERROR: Illegal weapon detected at PlayerComponent.Update()" << endl;
				break;
			}
			Audio::Play2DSound(SFX_SwitchWeapon, 1, 0);
			switchWeaponPrev = true;
		}
		if (!Input::GetXBoxButton(vehicle->GetPlayerNum(), ButtonCode::XBOX_Y))
			switchWeaponPrev = false;
	}

	//Fire Weapon - Human Players
	if (self->tag == TAGS_HUMAN_PLAYER)
	{
		VehicleComponent* vehicle = &VehicleComponent();
		vehicle = (VehicleComponent*)self->GetComponent(vehicle);
		if (Input::GetXBoxButton(vehicle->GetPlayerNum(), ButtonCode::XBOX_RIGHT_STICK))
		{
			MachineGunComponent* mgRef = &MachineGunComponent();
			MissileLauncherComponent* ml = &MissileLauncherComponent();
			switch (currentWeapon)
			{
			case GW_MACHINE_GUN:
				mgRef = (MachineGunComponent*)Game::Find(selfName)->GetComponent(mgRef);
				mgRef->FireMG();
				break;
			case GW_MISSILE_LAUNCHER:
				ml = (MissileLauncherComponent*)Game::Find(selfName)->GetComponent(ml);
				ml->FireMissile();
				break;
			case GW_FLAMETHROWER:
				break;
			default:
				cout << "ERROR: Illegal weapon detected at PlayerComponent.Update()" << endl;
				break;
			}
		}
	}

	//HUD display of health, ammo, score - Human Players Only
	if (self->tag == TAGS_HUMAN_PLAYER)
	{
		VehicleComponent* vehicle = &VehicleComponent();
		vehicle = (VehicleComponent*)self->GetComponent(vehicle);
		GameObject* health = Game::Find("HealthMeter"+vehicle->GetPlayerNum());
		health->transform.scale.x = playerHealth->currentHealth / 100;
		vec3 color;
		if (playerHealth->currentHealth > 50.0)
			color = vec3(1-(playerHealth->currentHealth-50.0)*0.02, 1, 0);
		else
			color = vec3(1, playerHealth->currentHealth*0.02, 0);
		health->particleOverlayMat.color = vec4(color, 1.0);

		GameObject* ammo = Game::Find("AmmoMeter"+vehicle->GetPlayerNum());
		float ratio;
		switch (currentWeapon)
		{
		case GW_MACHINE_GUN:
			ratio = (float)machineGunAmmo / 200.0;// GameManager::MAX_MACHINE_GUN_AMMO;
			break;
		case GW_MISSILE_LAUNCHER:
			ratio = (float)missileLauncherAmmo / 4.0;// (float)GameManager::MAX_MISSILE_LAUNCHER_AMMO;
			break;
		case GW_FLAMETHROWER:
			ratio = flamethrowerAmmo / 10.0;// GameManager::MAX_FLAMETHROWER_AMMO;
			break;
		default:
			cout << "ERROR: Illegal weapon detected at PlayerComponent.Update()" << endl;
			break;
		}
		ammo->transform.scale.x = ratio;

		string strScore = to_string(playerScore);
		GameObject* score1 = Game::Find("Score1"+vehicle->GetPlayerNum());
		GameObject* score2 = Game::Find("Score2"+vehicle->GetPlayerNum());
		GameObject* score3 = Game::Find("Score3"+vehicle->GetPlayerNum());
		GameObject* score4 = Game::Find("Score4"+vehicle->GetPlayerNum());
		GameObject* score5 = Game::Find("Score5"+vehicle->GetPlayerNum());

		for (int i = strScore.length() - 1; i >= 0; i--)
		{
			switch (i)
			{
			case 0:
				score5->particleOverlayMat.mainTexture = MAP_ZERO + (strScore[-1 + strScore.length()] - 48);
				break;
			case 1:
				score4->particleOverlayMat.mainTexture = MAP_ZERO + (strScore[-2 + strScore.length()] - 48);
				break;
			case 2:
				score3->particleOverlayMat.mainTexture = MAP_ZERO + (strScore[-3 + strScore.length()] - 48);
				break;
			case 3:
				score2->particleOverlayMat.mainTexture = MAP_ZERO + (strScore[-4 + strScore.length()] - 48);
				break;
			case 4:
				score1->particleOverlayMat.mainTexture = MAP_ZERO + (strScore[-5 + strScore.length()] - 48);
				break;
			default:
				cout << "What are ya doin' matey? Score be too large!" << endl;
				break;
			}
		}
	}

	//Headlights
	{
		Transform t = transform;
		t.rotation = t.GetInverseRotation();
		float speed;
		physx::PxVec3 v;
		if (self->tag == TAGS_HUMAN_PLAYER)
		{
			VehicleComponent* vehicle = &VehicleComponent();
			vehicle = (VehicleComponent*)self->GetComponent(vehicle);
			v = vehicle->physVehicle->getLinearVelocity();
		}
		else
		{
			EnemyComponent* enemy = &EnemyComponent();
			enemy = (EnemyComponent*)self->GetComponent(enemy);
			v = enemy->enPhysVehicle->getLinearVelocity();
		}
		speed = v.magnitude();
		vec3 dir = normalize(vec3(v.x, v.y, v.z));
		if (dot(dir, t.GetForward()) > 0.0)
			speed = -speed;
		t.position = t.position + (4.2f - speed*0.016f)*t.GetForward() + 0.6f*t.GetUp();
		PointLight light;
		light.Color = vec4(vec3(1), 10);
		light.Pos = vec4(t.position - 1.0f*t.GetRight(), 1.8);
		HealthComponent* health = &HealthComponent();
		health = (HealthComponent*)self->GetComponent(health);
		Renderer::AddPointLight(light);
		light.Pos = vec4(t.position + 1.0f*t.GetRight(), 1.8);
		if (health->currentHealth > 33.33)
			Renderer::AddPointLight(light);
	}

	UpdateParticles();

	Finalize();
}

void PlayerComponent::OnCollision(Component::CollisionPair collisionPair, Component* collider)
{
	MachineGunComponent* mgRef = &MachineGunComponent();
	HealthComponent* playerHealth = &HealthComponent();
	PowerUpComponent* powerUp = (PowerUpComponent*)collider;

	switch (collisionPair)
	{
	case(Component::CollisionPair::CP_VEHICLE_POWERUP):
		Audio::Play2DSound(SFX_Powerup, Random::rangef(0.20, 0.50), 0.0);
		playerScore += 10.0;
		switch (powerUp->type)
		{
		case GW_MACHINE_GUN:
			machineGunAmmo += 50;
			if (machineGunAmmo > 200.00)// GameManager::MAX_MACHINE_GUN_AMMO)
				machineGunAmmo = 200.00;// GameManager::MAX_MACHINE_GUN_AMMO;
			break;
		case GW_MISSILE_LAUNCHER:
			missileLauncherAmmo += 2;
			if (missileLauncherAmmo > 4.0)//GameManager::MAX_MISSILE_LAUNCHER_AMMO)
				missileLauncherAmmo = 4.0;// GameManager::MAX_MISSILE_LAUNCHER_AMMO;
			break;
		case GW_FLAMETHROWER:
			flamethrowerAmmo += 5;
			if (flamethrowerAmmo > 10.0)//GameManager::MAX_FLAMETHROWER_AMMO)
			flamethrowerAmmo = 10.0; //GameManager::MAX_FLAMETHROWER_AMMO;
			break;
		default:
			cout << "ERROR: Can't add ammo to illegal weapon at PlayerComponent OnColllision()!" << endl;
			break;
		}
		break;
	case(Component::CollisionPair::CP_VEHICLE_PROJECTILE):
		Audio::Play2DSound(SFX_Hit, Random::rangef(0.20, 0.50), 0.0);
		playerHealth = (HealthComponent*)Game::Find(selfName)->GetComponent(playerHealth);
		playerHealth->currentHealth -= 10.0;
		break;
	default:
		break;
	}
}

void PlayerComponent::UpdateParticles()
{
	
}

void PlayerComponent::PlayerStateToConsole() 
{
	cout << selfName << endl;
	cout << "Player Score: " << playerScore << endl;
	cout << endl;
}

/*void PlayerComponent::BurnCheck() {
	VehicleComponent* vehRef = &VehicleComponent();
	playerVeh = (VehicleComponent*)Game::Find(selfName)->GetComponent(vehRef);
	if (!burning) {
		double currentVelocity = playerVeh->currentSpeed;
		cout << currentVelocity << endl;
		if (currentVelocity >= 100.00)
			burnDetectionTime -= Time::getDeltaTime();
		else
			burnDetectionTime = 2.5;
	}

	if (burnDetectionTime <= 0.0) {
		burning = true;
		cout << selfName << " is Burning" << endl;
	}

}*/