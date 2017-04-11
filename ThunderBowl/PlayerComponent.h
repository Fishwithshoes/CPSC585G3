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

	vec3 playerCurrentPosition;
	PlayerComponent* lastDamaging;

	void Start();
	void Update();
	void OnCollision(Component::CollisionPair collisionPair, Component* collider);
	void PlayerStateToConsole();

	void StartParticles();
	void UpdateParticles();

private:
	bool switchWeaponPrev = false;
};

