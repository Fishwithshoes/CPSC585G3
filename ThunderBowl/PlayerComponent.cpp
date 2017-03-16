#include "PlayerComponent.h"
#include "VehicleComponent.h"
#include "Game.h"

//VehicleComponent* playerVeh;
void PlayerComponent::Start() 
{
	Initialize();

	playerHealth = 100.0;
	oldHealth = playerHealth;
	playerScore = 0;
	oldScore = playerScore;

	machineGunAmmo = 40;
	missileLauncherAmmo = 2;
	flamethrowerAmmo = 10.0;

	Finalize();
}

void PlayerComponent::Update() 
{
	Initialize();

	if (oldHealth != playerHealth || oldScore != playerScore) 
	{
		PlayerStateToConsole();
		oldHealth = playerHealth;
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

	//Switch Weapon - Human Players
	if (self->tag == TAGS_HUMAN_PLAYER)
	{
		if (Input::GetXBoxButton(1, ButtonCode::XBOX_Y) && !switchWeaponPrev)
		{
			switch (currentWeapon)
			{
			case GW_MACHINE_GUN:
				currentWeapon = GW_MISSILE_LAUNCHER;
				break;
			case GW_MISSILE_LAUNCHER:
				currentWeapon = GW_FLAMETHROWER;
				break;
			case GW_FLAMETHROWER:
				currentWeapon = GW_MACHINE_GUN;
				break;
			default:
				cout << "ERROR: Illegal weapon detected at PlayerComponent.Update()" << endl;
				break;
			}

			switchWeaponPrev = true;
		}
		if (!Input::GetXBoxButton(1, ButtonCode::XBOX_Y))
			switchWeaponPrev = false;
	}

	//Fire Weapon - Human Players
	if (self->tag == TAGS_HUMAN_PLAYER)
	{
		if (Input::GetXBoxButton(1, ButtonCode::XBOX_RIGHT_STICK))
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
		GameObject* health = Game::Find("HealthMeter");
		health->transform.scale.x = playerHealth / 100;

		GameObject* ammo = Game::Find("AmmoMeter");
		float ratio;
		switch (currentWeapon)
		{
		case GW_MACHINE_GUN:
			ratio = (float)machineGunAmmo / MAX_MACHINE_GUN_AMMO;
			break;
		case GW_MISSILE_LAUNCHER:
			ratio = (float)missileLauncherAmmo / (float)MAX_MISSILE_LAUNCHER_AMMO;
			break;
		case GW_FLAMETHROWER:
			ratio = flamethrowerAmmo / MAX_FLAMETHROWER_AMMO;
			break;
		default:
			cout << "ERROR: Illegal weapon detected at PlayerComponent.Update()" << endl;
			break;
		}
		ammo->transform.scale.x = ratio;

		string strScore = to_string(playerScore);
		GameObject* score1 = Game::Find("Score1");
		GameObject* score2 = Game::Find("Score2");
		GameObject* score3 = Game::Find("Score3");
		GameObject* score4 = Game::Find("Score4");

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

	Finalize();
}

void PlayerComponent::PlayerStateToConsole() 
{
	cout << selfName << endl;
	cout << "Player Health: " << playerHealth << endl;
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