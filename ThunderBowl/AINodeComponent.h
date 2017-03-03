#pragma once
#include "Component.h"
class AINodeComponent :
	public Component
{
public:
	void Start();
	void Update();

	vec3 nodeCurrentPosition;
};

