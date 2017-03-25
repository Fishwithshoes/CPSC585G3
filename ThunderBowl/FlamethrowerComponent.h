#pragma once
#include "Component.h"
#include "GameObject.h"
class FlamethrowerComponent : public Component
{
public:
	void Start();
	void Update();
	void UpdateParticles();

	float maxFuel = 100.0;

protected:

private:
	Transform t;
	string fireStreamName;
	float fuelRemaining = maxFuel;
};
