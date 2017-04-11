#pragma once
#include "Component.h"
class HealthComponent :
	public Component
{
public:
	void Start();
	void Update();
	bool isDead();
	bool isNewSpawn();
	float currentHealth;
	bool newSpawn;
	
private:
	void UpdateParticles();
	bool shouldRespawn = false;
	float respawnTime;
	float newSpawnTimer;
	int damageModel = 0;//0,1,2 Damge models
	string damagedSmokeName;
};

