#pragma once
#include "Component.h"
class VehicleComponent :public Component
{
public:
	void Start();
	void Update();

	float topSpeed = 200;
	float acceleration = 1.0;

	float topTurn = 1.0;
	float turnAccel = 0.01;

protected:

private:
	float currentSpeed = 0;
	float currentTurn = 0;
};

