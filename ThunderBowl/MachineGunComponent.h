#pragma once
#include "Component.h"
class MachineGunComponent : public Component
{
public:
	void Start();
	void Update();
	void FireMG();

	float bulletDelay = 0.2f;
	int ammoCount = 0;
	int maxAmmo = 200.0;

	int currentBullet;
	float nextBullet = bulletDelay;
private:
	//GameObject* owner;
	Mesh bulletMesh;

};

