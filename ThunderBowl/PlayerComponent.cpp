#include "PlayerComponent.h"
#include "VehicleComponent.h"
#include "Game.h"
#include "GameManager.h"
#include "Audio.h"

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
	t = transform;
	t.rotation = t.GetInverseRotation();
	ParticleSystem ps = ParticleSystem();
	ps.name = selfName + "DamagedSmoke";
	ps.transform = t;
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
				break;
			case GW_FLAMETHROWER:
				currentWeapon = GW_MACHINE_GUN;
				weaponIcon->particleOverlayMat.mainTexture = MAP_MACHINE_GUN_ICON;
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

		for (int i = strScore.length() - 1; i >= 0; i--)
		{
			switch (i)
			{
			case 0:
				score4->particleOverlayMat.mainTexture = MAP_ZERO + (strScore[-1 + strScore.length()] - 48);
				break;
			case 1:
				score3->particleOverlayMat.mainTexture = MAP_ZERO + (strScore[-2 + strScore.length()] - 48);
				break;
			case 2:
				score2->particleOverlayMat.mainTexture = MAP_ZERO + (strScore[-3 + strScore.length()] - 48);
				break;
			case 3:
				score1->particleOverlayMat.mainTexture = MAP_ZERO + (strScore[-4 + strScore.length()] - 48);
				break;
			default:
				cout << "What are ya doin' matey? Score be too large!" << endl;
				break;
			}
		}
	}

	UpdateParticles(playerHealth->currentHealth);

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

void PlayerComponent::UpdateParticles(float currentHealth)
{
	t = transform;
	t.rotation = t.GetInverseRotation();
	ParticleSystem* damagedSmoke = (ParticleSystem*)Game::Find(damagedSmokeName);
	damagedSmoke->transform = t;
	
	
	damagedSmoke->spawnRate = 5.0 - currentHealth*0.05;
	damagedSmoke->initialColor.alpha = vec4(vec3(0.5 - currentHealth*0.003), 1.0);
	damagedSmoke->initialColor.bravo = vec4(vec3(0.7 - currentHealth*0.003), 1.0);
	damagedSmoke->initialRadius.min = 1.4 - currentHealth*0.014;
	damagedSmoke->initialRadius.max = 1.8 - currentHealth*0.018;
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