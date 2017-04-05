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

	void Start();
	void Update();
	void OnCollision(Component::CollisionPair collisionPair, Component* collider);
	void PlayerStateToConsole();
	void BurnCheck();
	void Burning();

	void StartParticles();
	void UpdateParticles();

private:
	bool switchWeaponPrev = false;
};

