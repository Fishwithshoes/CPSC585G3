#pragma once
#include "Component.h"
class BulletComponent : public Component
{
public:
	void Start();
	void Update();

	float speed = 100.0f;
	float lifeSpan = 3.0f;

private:
	//GameObject* self;
	float lifeRemaining = lifeSpan;
};

