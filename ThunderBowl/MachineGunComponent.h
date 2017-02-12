#pragma once
#include "Component.h"
class MachineGunComponent : public Component
{
public:
	void Start();
	void Update();

	float bulletDelay = 0.2f;

private:
	//GameObject* owner;
	Mesh bulletMesh;
	float nextBullet = bulletDelay;
};

