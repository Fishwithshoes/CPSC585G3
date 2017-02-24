#pragma once
#include "Component.h"
class HealthComponent :
	public Component
{
public:
	void Start();
	void Update();

	float currentHealth;
};

