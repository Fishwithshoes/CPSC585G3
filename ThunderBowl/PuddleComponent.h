#pragma once
#include "Component.h"
class PuddleComponent : public Component
{
public:
	void Start();
	void Update();

	float rippleSpeed = 3.5;
	float rippleHeight = 1.5;

protected:
	
private:
	int passoverIndex = 1;
	int passoverLimit = 10;

	float currentRippleOffset = 0.0;
	vec2 currentBumpOffset = vec2(0, 0);
};

