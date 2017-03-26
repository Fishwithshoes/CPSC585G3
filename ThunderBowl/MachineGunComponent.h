#pragma once
#include "Component.h"
class MachineGunComponent : public Component
{
public:
	void Start();
	void Update();
	void FireMG();

	float bulletDelay = 0.05f;

	int currentBullet;
	float nextBullet = bulletDelay;
private:
	//GameObject* owner;
	Mesh bulletMesh;

};

