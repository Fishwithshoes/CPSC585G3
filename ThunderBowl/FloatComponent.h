#pragma once
#include "Component.h"

class FloatComponent : public Component
{
public:
	void Start();
	void Update();

	float speed = 2;
	float range = 0.1;

protected:

private:
	float current = 0;
};