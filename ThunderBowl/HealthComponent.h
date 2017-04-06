#pragma once
#include "Component.h"
class HealthComponent :
	public Component
{
public:
	void Start();
	void Update();
	bool isDead();

	float currentHealth;
	
private:
	void UpdateParticles();
	bool shouldRespawn = false;
	float respawnTime;
	int damageModel = 0;//0,1,2 Damge models
	string damagedSmokeName;
};

