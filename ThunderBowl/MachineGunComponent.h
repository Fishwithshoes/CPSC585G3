#pragma once
#include "Component.h"
class MachineGunComponent : public Component
{
public:
	void Start();
	void Update();

	float bulletDelay = 0.2f;
	int ammoCount = 0;

private:
	//GameObject* owner;
	int currentBullet;
	Mesh bulletMesh;
	float nextBullet = bulletDelay;
};

