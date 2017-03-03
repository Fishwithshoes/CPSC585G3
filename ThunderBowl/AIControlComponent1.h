#pragma once
#include "Component.h"
class AIControlComponent1 :
	public Component
{
public:
	enum AI_State
	{
		AIS_GO_FOR_AMMO,
		AIS_GO_FOR_PLAYER,
	};

	AI_State currentState = AIS_GO_FOR_AMMO;

	vec3 currentHeading;

	void Start();
	void Update();
};

