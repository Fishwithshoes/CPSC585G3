#pragma once
#include "Component.h"
class PlayerComponent :
	public Component
{
public:
	int playerScore;
	int oldScore;

	float burnDetectionTime = 2.5;
	float burnTick = 0.5;
	bool burning = false;

	GameWeapons currentWeapon = GW_MACHINE_GUN;
	int machineGunAmmo;
	int missileLauncherAmmo;
	float flamethrowerAmmo;

	const int MAX_MACHINE_GUN_AMMO = 200;
	const int MAX_MISSILE_LAUNCHER_AMMO = 4;
	const float MAX_FLAMETHROWER_AMMO = 10.0;

	void Start();
	void Update();
	void PlayerStateToConsole();
	void BurnCheck();
	void Burning();
private:
	bool switchWeaponPrev = false;
};

