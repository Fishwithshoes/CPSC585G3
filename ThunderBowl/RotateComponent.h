#pragma once
#include "Component.h"
class RotateComponent :public Component
{
public:
	void Start();
	void Update();

	float speed = 3;

protected:

private:
};

