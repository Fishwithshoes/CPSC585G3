#pragma once
#include "Component.h"

class OceanComponent :public Component
{
public:
	void Start();
	void Update();

	float currentSpeed = 0.5;
	float waveHeight = 0.3;

protected:

private:
	float currentVertOffset = 0.0;
	vec2 currentBumpOffset = vec2(0.0, 0.0);
};