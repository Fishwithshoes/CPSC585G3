#pragma once
#include "Component.h"

class FloatComponent : public Component
{
public:
	void Start();
	void Update();

	float speed = 4;
	float range = 0.5;

protected:

private:
	float current = 0;
};