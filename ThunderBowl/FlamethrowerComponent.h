#pragma once
#include "Component.h"
#include "GameObject.h"
class FlamethrowerComponent : public Component
{
public:
	void Start();
	void Update();
	void UpdateParticles();

protected:

private:
	Transform t;
	string fireStreamName;
};
