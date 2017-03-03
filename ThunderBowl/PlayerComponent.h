#pragma once
#include "Component.h"
class PlayerComponent :
	public Component
{
public:
	double playerHealth;
	double oldHealth;
	int playerScore;
	int oldScore;

	float burnDetectionTime = 2.5;
	float burnTick = 0.5;
	bool burning = false;

	void Start();
	void Update();
	void PlayerStateToConsole();
	void BurnCheck();
	void Burning();
};

