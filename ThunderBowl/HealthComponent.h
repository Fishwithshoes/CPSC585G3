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
	float respawnTime;
};

